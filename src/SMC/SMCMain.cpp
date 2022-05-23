/* Copyright (C) 2020  Emil F. L. Aagreen <emil.fulei@gmail.com>,
 *                     Frederik A. Jensen <frederik@arnfeldt-jensen.dk>,
 *                     Mikkel T. Jensen <mikjen97@gmail.com>
 *                     Rasmus-Emil N. Herum <rasmus.emil.herum@gmail.com>
 *                     Tobias B. S. Hansen <tbsh18@student.aau.dk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <memory>
#include "SMC/SMCMain.h"

#include "SMC/SuccessorGeneration/SMCSuccessorGenerator.h"

#include "PetriEngine/PQL/PQL.h"
#include "PetriEngine/PQL/Contexts.h"
#include "PetriEngine/PQL/Evaluation.h"
#include "PetriEngine/PQL/PredicateCheckers.h"
#include "SMC/Stubborn/SMCStubbornSet.h"

#include <vector>

using namespace PetriEngine;

namespace SMC
{
    bool SMCRun(SMCSuccessorGenerator &sgen,
                const PetriNet *net,
                const PQL::Condition_ptr &query,
                int max_depth,
                int SMCit,
                std::shared_ptr<SMC::SMCStubbornSet> stubset,
                bool *stubborn,
                int64_t &prepTime,
                int64_t &fireTime,
                int64_t &evalTime,
                int &evalCount,
                int &outdepth)
    {
        uint32_t tindex = 0;
        
        Structures::State write(net->makeInitialMarking());
        sgen.prepare(&write);
        sgen.reset();
        
        PQL::EvaluationContext context(write.marking(), net);
        
        // begin eval timer
        auto eval_begin = std::chrono::high_resolution_clock::now();

        auto queryeval = PQL::evaluate(query.get(), context) == PQL::Condition::RTRUE;

        // end eval timer
        auto eval_end = std::chrono::high_resolution_clock::now();
        evalTime += std::chrono::duration_cast<std::chrono::microseconds>(eval_end - eval_begin).count();
        evalCount++;

        // if query is true initially
        if(queryeval)
        {
            return true;
        }

        while(current_depth < max_depth && sgen.next(write, tindex, fireTime))
        {
            context.setMarking(write.marking());

            if(!SMCit || stubborn[tindex])
            {   
                evalCount++;
                // begin eval timer
                auto eval_begin = std::chrono::high_resolution_clock::now();

                auto queryeval = PQL::evaluate(query.get(), context) == PQL::Condition::RTRUE;

                // end eval timer
                auto eval_end = std::chrono::high_resolution_clock::now();
                evalTime += std::chrono::duration_cast<std::chrono::microseconds>(eval_end - eval_begin).count();

                if(queryeval)
                {                   
                    return true;
                }
                
                // update Am(phi)
                if (SMCit == 1)
                {
                    // begin update timer
                    auto begin = std::chrono::high_resolution_clock::now();

                    stubset->prepare(&write);

                    // end update timer
                    auto end = std::chrono::high_resolution_clock::now();
                    prepTime += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
                }
            }
            current_depth++;
            outdepth++;
        }
        outdepth++;
        return false;
    }

    double SMCMain(const PetriNet *net,
                         options_t &options,
                         const PQL::Condition_ptr &query)
    {
        int total_runs = 0;
        int successful_runs = 0;

        SMCSuccessorGenerator sgen(*net);

        Structures::State initialwrite(net->makeInitialMarking());

        auto stubset = std::make_shared<SMCStubbornSet>(*net, query);
        auto stubborn = stubset->stubborn();
        auto SMCit = options.smcit;
        int64_t preparationTime = 0;

        if(SMCit){
            // Am(phi)
            if(SMCit == 1){
                stubset->SMC::SMCStubbornSet::setInterestingVisitor<PetriEngine::InterestingTransitionVisitor>();
            }
            // A(phi)
            else{
                stubset->SMC::SMCStubbornSet::setInterestingSMCVisitor<PetriEngine::InterestingSMCTransitionVisitor>();
            }

            // begin set preparation timer
            auto begin = std::chrono::high_resolution_clock::now();

            stubset->prepare(&initialwrite);

            // end set preparation timer
            auto end = std::chrono::high_resolution_clock::now();
            preparationTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            int64_t evalTime = 0;
            int evalCount = 0;
            int outdepth = 0;
        }
        for (int r = 1; r <= options.smcruns; ++r)
        {   
            int64_t fireTime = 0;

            if (SMCRun(sgen, net, query, options.smcdepth, SMCit, stubset, stubborn, preparationTime, fireTime, evalTime, evalCount, outdepth))
            {
                std::cout << "grep,preptime," << preparationTime << ",firetime," << fireTime << ",evaltime," << evalTime << ",evalcount," << evalCount << ",steps," << outdepth;
                return (double)r;
                successful_runs++;
            }
            total_runs++;

            // reset Am(phi) to initial
            if(SMCit == 1){
                // begin reset timer
                auto begin = std::chrono::high_resolution_clock::now();

                stubset->prepare(&initialwrite);
                
                // end reset timer
                auto end = std::chrono::high_resolution_clock::now();
                preparationTime += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            }
        }
        std::cout << "grep,preptime," << preparationTime << ",firetime," << fireTime << ",evaltime," << evalTime << ",evalcount," << evalCount << ",steps," << outdepth;
        return (((double)successful_runs)/((double)total_runs))*100.;
    }
}

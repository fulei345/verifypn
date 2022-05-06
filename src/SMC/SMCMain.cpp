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
#include "SMC/Heuristic/SMCDistanceHeuristic.h"

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
                std::vector<int> potency)
    {
        int current_depth = 0;
        uint32_t tindex = 0;
        uint32_t last_heuristic = 0;
        
        Structures::State write(net->makeInitialMarking());
        sgen.prepare(&write);
        sgen.reset();

        PQL::EvaluationContext context(write.marking(), net);
        SMC::SMCDistanceHeuristic heuristic(net, query);


        // Evaluate
        // Prepare

        do{
            context.setMarking(write.marking());

            // Heuristic
            if(SMCit)
            {
                uint32_t h = heuristic.eval(write, tindex);

                if(!current_depth)
                {
                    last_heuristic = h;
                }
            
                if(h < last_heuristic)
                {
                    potency[tindex] += 1;
                }
                else if(h > last_heuristic && potency[tindex] > 1)
                {
                    potency[tindex] -= 1;
                }

                last_heuristic = h;
            
                std::cout << "heurstic: " << h  << " tindex:" << tindex << std::endl;

                for(int i = 0; i < net->numberOfTransitions(); i++)
                {
                    std::cout << "pot " << i << ": " << potency[i] << std::endl;
                }

            // for(int i = 0; i < net->numberOfTransitions(); i++)
            //     {
            //         if (stubborn[i])
            //         {
            //             potency[i] = 10;
            //         }
            //         else
            //         {
            //             potency[i] = 1;
            //         }
            //     }
            }
            // ALL, 
            if(!SMCit || stubborn[tindex] || !current_depth)
            {
                if(PQL::evaluate(query.get(), context) == PQL::Condition::RTRUE)
                {
                    return true;
                }
                // update Am(phi)
                if(SMCit == 1 && !current_depth)
                {
                    stubset->prepare(&write);
                }
            }
            current_depth++;
        }
        while(current_depth < max_depth && sgen.next(write, tindex, potency));
        
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

        auto potency = net->transitionPotency();
        
        if(SMCit){
            // Am(phi)
            if(SMCit == 1){
                stubset->SMC::SMCStubbornSet::setInterestingVisitor<PetriEngine::InterestingTransitionVisitor>();
            }
            // A(phi)
            else{
                stubset->SMC::SMCStubbornSet::setInterestingSMCVisitor<PetriEngine::InterestingSMCTransitionVisitor>();
            }
            stubset->prepare(&initialwrite);
        }

        auto initpotency = potency;

        for (int i = 0; i < options.smcruns; i++)
        {
            if (SMCRun(sgen, net, query, options.smcdepth, SMCit, stubset, stubborn, potency))
            {
                successful_runs++;
            }
            total_runs++;

            // reset Am(phi) to initial
            if(SMCit == 1){
                stubset->prepare(&initialwrite);
                potency = initpotency;
            }
        }
        return (((double)successful_runs)/((double)total_runs))*100.;
    }
}

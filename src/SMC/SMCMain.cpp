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
                bool aphi)
    {
        int current_depth = 0;
        uint32_t tindex = 0;
        
        Structures::State write(net->makeInitialMarking());
        sgen.prepare(&write);
        sgen.reset();
        
        PQL::EvaluationContext context(write.marking(), net);
        
        // This if-statement is new :)
        if(PQL::evaluate(query.get(), context) == PQL::Condition::RTRUE)
        {
            return true;
        }

        auto stubset = std::make_shared<SMCStubbornSet>(*net, query);
        if(aphi){
            stubset->SMC::SMCStubbornSet::setInterestingVisitor<PetriEngine::InterestingTransitionVisitor>();
        }
        else{
            stubset->SMC::SMCStubbornSet::setInterestingSMCVisitor<PetriEngine::InterestingSMCTransitionVisitor>();
        }
        auto stubborn = stubset->stubborn();
        stubset->prepare(&write);

        while(current_depth < max_depth && sgen.next(write, tindex))
        {
            context.setMarking(write.marking());

            if(stubborn[tindex])
            {
                if(PQL::evaluate(query.get(), context) == PQL::Condition::RTRUE)
                {
                    return true;
                }
            }

            if(aphi){
                stubset->prepare(&write);
            }
            current_depth++;
        }
        return false;
    }

    double SMCMain(const PetriNet *net,
                         options_t &options,
                         const PQL::Condition_ptr &query)
    {
        int total_runs = 0;
        int successful_runs = 0;

        SMCSuccessorGenerator sgen(*net);
        
        for (int i = 0; i < options.smcruns; i++)
        {
            if (SMCRun(sgen, net, query, options.smcdepth, options.useAphi))
            {
                successful_runs++;
            }
            total_runs++;
        }
        return (((double)successful_runs)/((double)total_runs))*100.;
    }
}

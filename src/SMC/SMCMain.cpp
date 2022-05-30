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
                std::vector<int> &potency,
                std::vector<bool> heuristics,
                int64_t &time)
    {
        int current_depth = 0;
        uint32_t tindex = 0;
        std::vector<uint32_t> fired;
        
        Structures::State write(net->makeInitialMarking());
        sgen.prepare(&write);
        sgen.reset();

        PQL::EvaluationContext context(write.marking(), net);
        SMC::SMCDistanceHeuristic heuristic(net, query);

        uint32_t last_heuristic = heuristic.eval(write, tindex);

        auto begin = std::chrono::high_resolution_clock::now();

        // Evaluate
        // Prepare

        // This if-statement is new :)
        if(PQL::evaluate(query.get(), context) == PQL::Condition::RTRUE )
        {
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            if(query->isInvariant())
            {
                //false
                return true;
            }
            return true;
        }

        uint32_t h;
        std::pair<uint32_t,uint32_t> bestdist;
        bestdist.first = last_heuristic;
        bestdist.second = -1;

        while(current_depth < max_depth && sgen.next(write, tindex, potency))
        {            
            context.setMarking(write.marking());
            fired.push_back(tindex);

            if(heuristics[1] || heuristics[2])
            {
                h = heuristic.eval(write, tindex);
            }

            // Heuristic
            if(heuristics[1])
            {
                int v = 1;
            
                if(h < last_heuristic)
                {
                    potency[tindex] += v;
                }
                else if(h > last_heuristic)
                {
                    if(potency[tindex] > v)
                    {
                        potency[tindex] -= v;
                    }
                    else
                    {
                        potency[tindex] = 1;
                    }
                }

                last_heuristic = h;
            }

            if(heuristics[2] && h < bestdist.first)
            {
                bestdist.first = h;
                bestdist.second = current_depth;
            }

            // ALL, 
            if(!SMCit || stubborn[tindex])
            {
                if(PQL::evaluate(query.get(), context) == PQL::Condition::RTRUE)
                {
                    auto end = std::chrono::high_resolution_clock::now();
                    time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
                    if(query->isInvariant())
                    {
                        //false
                        return true;
                    }
                    return true;
                }
                // update Am(phi)
                if(SMCit == 1)
                {
                    // gem array med trans der er blevet fjernet fra stubset :D
                    stubset->prepare(&write);
                }
            }
            current_depth++;
        }
        while(current_depth < max_depth && sgen.next(write, tindex, potency));

        if(heuristics[2])
        {
            int v = 1;

            if(bestdist.first < last_heuristic)
            {
                for(uint32_t t = 0; t < bestdist.second; t++)
                {
                    potency[fired[t]] += v;
                }
                if(potency[fired[bestdist.second+1]] > v)
                {
                    potency[fired[bestdist.second+1]] -= v;
                }
                else
                {
                    potency[fired[bestdist.second+1]] = 1;
                }
            }
            else
            {
                for(uint32_t t : fired)
                {
                    if(potency[t] > v)
                    {
                        potency[t] -= v;
                    }
                    else
                    {
                        potency[t] = 1;
                    }
                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
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
        auto potency = net->transitionPotency();
        std::vector<bool> heuristics (3, false);

        for(int h : options.heuristicnumbers)
        {
            heuristics[h] = true;
        }
        
        if(options.smcit){
            // Am(phi)
            if(options.smcit == 1){
                stubset->SMC::SMCStubbornSet::setInterestingVisitor<PetriEngine::InterestingTransitionVisitor>();
            }
            // A(phi)
            else{
                stubset->SMC::SMCStubbornSet::setInterestingSMCVisitor<PetriEngine::InterestingSMCTransitionVisitor>();
            }
            stubset->prepare(&initialwrite);
            if(heuristics[0])
                for(uint32_t i = 0; i < net->numberOfTransitions(); i++)
                {
                    if (stubborn[i])
                    {
                        potency[i] = 100;
                    }
                }
        }

        auto initpotency = potency;

        int64_t time = 0;

        for (int i = 0; i < options.smcruns; i++)
        {
            if (SMCRun(sgen, net, query, options.smcdepth, options.smcit, stubset, stubborn, potency, heuristics, time))
            {
                return (double)i;
                successful_runs++;
            }
            total_runs++;

            // reset Am(phi) to initial
            if(options.smcit == 1){
                stubset->prepare(&initialwrite);
            }
        }
        return (((double)successful_runs)/((double)total_runs))*100.;
    }
}

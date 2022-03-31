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

//#include <stdlib.h>
#include <memory>
#include "SMC/SMCMain.h"

#include "SMC/SuccessorGeneration/SMCSuccessorGenerator.h"
#include "PetriEngine/PQL/PQL.h"
#include "PetriEngine/PQL/Contexts.h"
#include "PetriEngine/PQL/Evaluation.h"

#include "PetriEngine/PQL/PredicateCheckers.h"

using namespace PetriEngine;

namespace SMC {
    bool SMCRun(SMCSuccessorGenerator& sgen, const PetriNet *net, std::vector<std::shared_ptr<PQL::Condition>> &queries, int max_depth){

        int current_depth = 0;
        uint32_t max = std::numeric_limits<uint32_t>::max();
        uint32_t tindex = std::numeric_limits<uint32_t>::max();


        Structures::State write(net->makeInitialMarking());
        sgen.prepare(&write);
        sgen.reset();
        
        int n_size = net->numberOfPlaces();
        std::cout << "size: " << n_size << std::endl;
        std::cout << "initial marking: " << *write.marking() << std::endl;

        std::cout << "query: ";
        queries[0].get()->toString(std::cout);
        std::cout << "\nisReach?: " << (isReachability(queries[0]) ? "True" : "False") << std::endl;
        std::cout << std::endl;

        while(sgen.next(write, tindex) && current_depth <= max_depth){
            // simply check whether query is true for each place visited?
            PQL::EvaluationContext ec(write.marking(), net);
            if(PQL::evaluate(queries[0].get(), ec) == PQL::Condition::RTRUE)
            {
                std::cout << "test eval" << std::endl;
                return true;
            }
            current_depth++;
        }
        std::cout << "final marking: " << *write.marking() << ", tindex: " << tindex << std::endl;
        if (tindex == max){
            return false;
        }
        return true;
        
    }

    double SMCMain(const PetriNet *net,
                         options_t &options,
                         std::vector<std::shared_ptr<PQL::Condition>> &queries) {

        int total_runs = 0;
        int successful_runs = 0;
        SMCSuccessorGenerator sgen(*net);

        for (int i = 0; i < options.smcruns; i++) {
            if (SMCRun(sgen, net, queries, options.smcdepth)) {
                successful_runs++;
                std::cout << "run " << total_runs+1 << "/" << options.smcruns << " finished successfully.\n" << std::endl;
            }
            else {
                std::cout << "run " << total_runs+1 << "/" << options.smcruns << " finished.\n" << std::endl;
            }
            total_runs++;
        }
        return (((double)successful_runs)/((double)total_runs))*100;
    }
}

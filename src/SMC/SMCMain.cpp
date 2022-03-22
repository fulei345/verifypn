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

#include <stdlib.h>
#include "SMC/SMCMain.h"

#include "SMC/SuccessorGeneration/SMCSuccessorGenerator.h"

using namespace PetriEngine;

namespace SMC {
    bool SMCRun(SMCSuccessorGenerator& sgen, const PetriNet *net, int max_depth){

        int current_depth = 0;
        uint32_t max = std::numeric_limits<uint32_t>::max();
        uint32_t tcurrent = 0;
        uint32_t tindex = 0;


        Structures::State write(net->makeInitialMarking());
        sgen.prepare(&write);
        sgen.reset();
        
        int n_size = net->numberOfPlaces();
        std::cout << "size: " << n_size << std::endl;
        std::cout << "initial marking: " << *write.marking() << "\n" << std::endl;

        // Fire transitions until property is satisfied, deadlock, or max depth is reached
        while (current_depth < max_depth) { 
            std::cout << "\nc_depth: " << current_depth << std::endl;           
            // Check if state satisfies property here
            if (false) {
                return true;
            }

            int n = 1;
            if (tindex != max) {
               std::cout << "tindex:" << tindex << ", checkpreset: " << sgen.checkPreset(tindex) << ", write: " << *write.marking() << std::endl;
            }
            else {
               std::cout << "tindex:" << tindex << ", write: " << *write.marking() << std::endl;
            }

            // Choosing transition to fire with uniform probabilities
            while (sgen.next(write, tindex)) {
                double randomNum = (double)rand()/RAND_MAX;
                if (randomNum <= 1./((double)n)) {
                    std::cout << "current t: " << tcurrent << ", next t: " << tindex << std::endl;
                    tcurrent = tindex;
                }
                n++;
            }
            // Check if there is a transition to fire
            if (tcurrent != max) {
                std::cout << "tcurrent != max, tcurrent: " << tcurrent << std::endl;
                sgen.fire(write, tcurrent);
                return true;
            }

            current_depth++;
            sgen.reset();
        }
        return false;
    }

    double SMCMain(const PetriNet *net,
                         options_t &options) {

        int total_runs = 0;
        int successful_runs = 0;
        SMCSuccessorGenerator sgen(*net);

        for (int i = 0; i < options.smcruns; i++) {
            if (SMCRun(sgen, net, options.smcdepth)) {
                successful_runs++;
            }
            std::cout << "run " << total_runs << " finished.\n" << std::endl;
            total_runs++;
        }
        return ((double)successful_runs)/((double)total_runs);
    }
}

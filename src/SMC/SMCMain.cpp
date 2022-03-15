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
    bool SMCRun(const PetriNet *net, int depth){
        
        SMC::SMCSuccessorGenerator SMCSG(*net);
        Structures::State write(net->makeInitialMarking());

        int current_depth = 0;
        uint32_t max = std::numeric_limits<uint32_t>::max();
        // Fire transitions untill property is satisfied, deadlock, or max depth is reached
        while (current_depth <= depth) {            
            // Check if state satisfies property here
            if (false) {
                return true;
            }

            int n = 1;
            uint32_t tcurrent;
            uint32_t tindex = std::numeric_limits<uint32_t>::min();

            // Choosing transition to fire with uniform probabilities
            while (SMCSG.next(write, tindex)) {
                float randomNum = rand()/RAND_MAX;
                if (randomNum <= 1./((float)n)) {
                    tcurrent = tindex;
                }
                n++;
            }

            // Check if there is a transition to fire
            if (tcurrent != max) {
                SMCSG.fire(write, tcurrent);
            } else {
                return false;
            }

            current_depth++;
        }
        return false;
    }

    double SMCMain(const PetriNet *net,
                         options_t &options) {

        int total_runs = 0;
        int successful_runs = 0;

        for (int i = 0; i < options.smcruns; i++) {
            if (SMCRun(net, options.smcdepth)) {
                successful_runs++;
            }
            total_runs++;
        }
        return ((double)successful_runs)/((double)total_runs);
    }
}

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
        uint32_t max, tcurrent = std::numeric_limits<uint32_t>::max();
        uint32_t tindex = 0;


        Structures::State write(net->makeInitialMarking());
        sgen.prepare(&write);
        sgen.reset();
        
        int n_size = net->numberOfPlaces();
        std::cout << "size: " << n_size << std::endl;
        std::cout << "initial marking: " << *write.marking() << "\n" << std::endl;
        while(sgen.next(write, tindex)){}
        std::cout << "final marking: " << *write.marking() << ", tindex: " << tindex << std::endl;
        return true;
    }

    double SMCMain(const PetriNet *net,
                         options_t &options) {

        int total_runs = 0;
        int successful_runs = 0;
        SMCSuccessorGenerator sgen(*net);

        for (int i = 0; i < options.smcruns; i++) {
            if (SMCRun(sgen, net, options.smcdepth)) {
                successful_runs++;
                std::cout << "run " << total_runs+1 << "/" << options.smcruns << " finished successfully.\n" << std::endl;
            }
            else {
                std::cout << "run " << total_runs+1 << "/" << options.smcruns << " finished.\n" << std::endl;
            }
            total_runs++;
        }
        return ((double)successful_runs)/((double)total_runs)*100;
    }
}

/* Copyright (C) 2020  Emil F. L. Aagreen <emil.fulei@gmail.com>,
 *                     Frederik A. Jensen <frederik@arnfeldt-jensen.com>,
 *                     Mikkel T. Jensen <mikjen97@gmail.com>
 *                     Rasmus-Emil N. Herum <>
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

#include "SMC/SMCMain.h"

#include "SMC/SuccessorGeneration/SMCSuccessorGenerator.h"

using namespace PetriEngine;

namespace SMC {
    double SMCMain(const PetriNet *net,
                         options_t &options){
                    std::unique_ptr<MarkVal[]> qm0(qnet->makeInitialMarking());
            
            for(size_t i = 0; i < qnet->numberOfPlaces(); ++i) {
                initial_size += qm0[i];
            }
            // dummy "write" state and tindex pointers?
            Structures::State state;

            // double free SIGABRT malloc auto delete when pointer deref or .get()???
            // because unique_ptr -> default_delete -> int_free -> malloc???
            //state.setMarking(qm0.get());

            uint32_t tindex = std::numeric_limits<uint32_t>::min();

            // our first succ gen now with public next?
            SMC::SMCSuccessorGenerator sgs(*qnet);
            
            // cant use next when state is empty
            //bool test = sgs.next(state, tindex);

            // pseudo alg for smc simulator
            // bool SMCSimulation(Structures::State& write, uint32_t &tindex) {
            //     int m, n, current_depth = 0;
            //     while (SG.next(write, tindex) && current_depth < options.smcdepth){
            //         //m = *tindex.Domination();
            //         m = 1;
            //         n = n + m;
            //         if (m/n =< random(1)){
            //             SG.fire(tindex);
            //             if (property == yes){
            //                 return true;
            //             }
            //         }
            //     }
            //     return false;
            // }

            if(options.trace != TraceLevel::None) {
                std::cout << "\nSMC Simulation:" << std::endl;
                std::cout << "Simulate " << options.smcruns << " runs, with " << options.smcdepth << " max depth." << std::endl;
                std::cout << "Trace:" << std::endl;
            }
            else {
                // for (int i = 0; i < options.smcruns; i++) {
                //     int m, n, current_depth, count = 0;
                //     uint32_t tindex = std::numeric_limits<uint32_t>::min();
                //     //SG.next(state, tindex)
                //     while (SG.next(state, tindex) && current_depth <= options.smcdepth) {
                //         //m = qm0[0].domination();
                //         m = 1;
                //         n = n + m;
                //         current_depth++;
                //         if (m/n <= rand()){
                //             SG.fire(*tindex);
                //             if (property == true){
                //                 count++;
                //             }
                //         }
                //     }
                // }
                std::cout << "\nSMC Simulation:" << std::endl;
                std::cout << "Simulate " << options.smcruns << " runs, with " << options.smcdepth << " max depth." << std::endl;
            }
    }
}

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

#include "SMC/SMCMain.h"

#include "SMC/SuccessorGeneration/SMCSuccessorGenerator.h"

using namespace PetriEngine;

namespace SMC {
    bool Simulator(PetriNet net, MarkVal write){
        SMC::SMCSuccessorGenerator sgs(net);

        uint32_t tindex = std::numeric_limits<uint32_t>::min();

        MarkVal test = *write.get();
        Structures::State state(&test);

        bool test = sgs.next(state, tindex);
        return test;

        // pseudo alg for smc simulator
        // int m, n, current_depth = 0;
        // while (SG.next(write, tindex) && current_depth < options.smcdepth){
        //     //m = *tindex.Domination();
        //     m = 1;
        //     n = n + m;
        //     if (m/n =< random(1)){
        //         SG.fire(tindex);
        //         if (property == yes){
        //             return true;
        //         }
        //     }
        // }
        // return false;
    }
    double SMCMain(const PetriNet *net,
                         options_t &options){
            
        auto qm0 = std::unique_ptr<MarkVal[]> (net->makeInitialMarking());


        for (size_t i = 0; i < options.smcruns; i++)
        {
            //if( Simulator(*net,write,tindex)) {
            //  succ_count++;
            //}
        }
        // return succ_count/options.smcruns
        return 0.0;
    }
}

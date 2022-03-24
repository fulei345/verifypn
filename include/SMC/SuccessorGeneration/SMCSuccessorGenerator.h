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

#include "PetriEngine/SuccessorGenerator.h"
#include "PetriEngine/Structures/State.h"

namespace SMC{
    using namespace PetriEngine;
    class SMCSuccessorGenerator : public SuccessorGenerator{
        public:
        int n = 0;
        SMCSuccessorGenerator(const PetriNet &net);
    
        // bool next(Structures::State &write, uint32_t &tindex) {
        //     bool has_suc = next(write, tindex);
        //     return has_suc;
        // }

        bool next(Structures::State &write, uint32_t &tindex);

        void fire(Structures::State &write, uint32_t tindex){
            _fire(write, tindex);
        }
    };
}
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

#include "PetriEngine/PetriNet.h"
#include "PetriEngine/Structures/State.h"


#include <iostream>

namespace SMC {
    class SMCHeuristic{
    public:
        virtual void prepare(const Structures::State &state) {}

        virtual uint32_t eval(const Structures::State &state, uint32_t tid) = 0;

        /**
         * Does the heuristic provide a prioritisation from this state.
         * @return True if a heuristic can be calculated from this state.
         */
        virtual bool has_heuristic(const Structures::State &)
        {
            return true;
        }

        virtual void push(uint32_t tid) {};

        virtual void pop(uint32_t tid) {};

        virtual ~SMCHeuristic() = default;

        virtual std::ostream &output(std::ostream &os) = 0;
    };
}


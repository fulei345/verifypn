/* Copyright (C) 2021  Nikolaj J. Ulrik <nikolaj@njulrik.dk>,
 *                     Simon M. Virenfeldt <simon@simwir.dk>
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

#ifndef VERIFYPN_ALGORITHMTYPES_H
#define VERIFYPN_ALGORITHMTYPES_H

#include "utils/errors.h"

namespace LTL {
    enum class Algorithm {
        NDFS, Tarjan, None=-1
    };

    enum class BuchiOutType {
        Dot,
        HOA,
        Spin
    };

    inline auto to_string(Algorithm alg) {
        switch (alg) {
            case Algorithm::NDFS:
                return "NDFS";
            case Algorithm::Tarjan:
                return "TARJAN";
            case Algorithm::None:
            default:
                throw base_error("to_string: Invalid LTL Algorithm ", static_cast<int>(alg));
        }
    }
}

#endif //VERIFYPN_ALGORITHMTYPES_H

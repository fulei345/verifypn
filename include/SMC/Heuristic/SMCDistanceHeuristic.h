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

#include "SMC/Heuristic/SMCHeuristic.h"

#include <utility>
#include "PetriEngine/PQL/Contexts.h"

namespace SMC {
    class SMCDistanceHeuristic : public SMCHeuristic {
    public:
        SMCDistanceHeuristic(const PetriEngine::PetriNet *net, PetriEngine::PQL::Condition_ptr cond) : _net(net), _cond(std::move(cond)) {}

        uint32_t eval(const Structures::State &state, uint32_t tid) override
        {
            PetriEngine::PQL::DistanceContext context{_net, state.marking()};
            return _cond->distance(context);
        }

        std::ostream &output(std::ostream &os) {
            return os << "DIST_HEUR";
        }
    private:
        const PetriEngine::PetriNet *_net;
        const PetriEngine::PQL::Condition_ptr _cond;
    };
}

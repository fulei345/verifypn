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

#include "SMC/Stubborn/SMCStubbornSet.h"
#include "PetriEngine/PQL/Contexts.h"
#include "PetriEngine/PQL/Evaluation.h"

namespace SMC {
    using namespace PetriEngine;
    bool SMCStubbornSet::prepare(const Structures::State *state) {
        reset();
        auto _parent = state;

        assert(!_queries.empty());
        for (auto &q : _queries) {
            PetriEngine::PQL::evaluateAndSet(q, PQL::EvaluationContext((*_parent).marking(), &_net));

            assert(_interesting->get_negated() == false);
            PQL::Visitor::visit(_interesting, q);
        }

        return true;
    }
}

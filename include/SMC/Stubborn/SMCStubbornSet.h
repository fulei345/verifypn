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

#include "SMC/Stubborn/InterestingSMCTransitionVisitor.h"

namespace SMC {
    using namespace PetriEngine;
    class SMCStubbornSet : public StubbornSet {
    public:
        SMCStubbornSet(const PetriNet &net, const PQL::Condition_ptr &query, bool closure = false)
                : StubbornSet(net, query), _closure(closure) {
            setInterestingVisitor<PetriEngine::InterestingSMCTransitionVisitor>();
        }

        bool prepare(const Structures::State *state) override;

        template <typename TVisitor>
        void setInterestingVisitor()
        {
            _interesting = std::make_unique<TVisitor>(*this, _closure);
        }

    private:
        std::unique_ptr<PetriEngine::InterestingSMCTransitionVisitor> _interesting;

        bool _closure;
    };
}


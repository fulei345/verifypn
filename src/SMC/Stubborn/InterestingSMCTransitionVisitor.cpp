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

namespace PetriEngine {
    void InterestingSMCTransitionVisitor::_accept(const PQL::DeadlockCondition *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::_accept(const PQL::LessThanCondition *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::_accept(const PQL::LessThanOrEqualCondition *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::_accept(const PQL::EqualCondition *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::_accept(const PQL::NotEqualCondition *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::_accept(const PQL::AndCondition *element)
    {
        // TODO implement this
    }

        void InterestingSMCTransitionVisitor::_accept(const PQL::OrCondition *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::_accept(const PQL::CompareConjunction *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::IncrVisitor::_accept(const PQL::PlusExpr *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::DecrVisitor::_accept(const PQL::PlusExpr *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::IncrVisitor::_accept(const PQL::SubtractExpr *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::DecrVisitor::_accept(const PQL::SubtractExpr *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::IncrVisitor::_accept(const PQL::MultiplyExpr *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::DecrVisitor::_accept(const PQL::MultiplyExpr *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::IncrVisitor::_accept(const PQL::MinusExpr *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::DecrVisitor::_accept(const PQL::MinusExpr *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::IncrVisitor::_accept(const PQL::LiteralExpr *element)
    {
        // Add nothing
    }

    void InterestingSMCTransitionVisitor::DecrVisitor::_accept(const PQL::LiteralExpr *element)
    {
        // Add nothing
    }

    void InterestingSMCTransitionVisitor::IncrVisitor::_accept(const PQL::UnfoldedIdentifierExpr *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::DecrVisitor::_accept(const PQL::UnfoldedIdentifierExpr *element)
    {
        // TODO implement this
    }
}
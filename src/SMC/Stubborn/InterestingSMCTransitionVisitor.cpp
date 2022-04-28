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
        // If the property is a deadlock property we add all transitions
        // TODO make sure that "_net" is not a reduced net?
        for (uint32_t t = 0; t < _stubborn._net.numberOfTransitions(); t++)
        {
            _stubborn.addToStub(t);
        } 
    }

    void InterestingSMCTransitionVisitor::_accept(const PQL::EqualCondition *element)
    {
        // We are more pessimistic so we add more transitions when the property is an equality property
        if (!negated) {               // equal
            if (element->getExpr1()->getEval() == element->getExpr2()->getEval()) { return; }
            Visitor::visit(incr, element->getExpr1());
            Visitor::visit(decr, element->getExpr1());
            Visitor::visit(incr, element->getExpr2());
            Visitor::visit(decr, element->getExpr2());
        } else {                    // not equal
            if (element->getExpr1()->getEval() != element->getExpr2()->getEval()) { return; }
            Visitor::visit(incr, element->getExpr1());
            Visitor::visit(decr, element->getExpr1());
            Visitor::visit(incr, element->getExpr2());
            Visitor::visit(decr, element->getExpr2());
        }
    }

    void InterestingSMCTransitionVisitor::_accept(const PQL::NotEqualCondition *element)
    {
        if (!negated) {               // not equal
            if (element->getExpr1()->getEval() != element->getExpr2()->getEval()) { return; }
            Visitor::visit(incr, element->getExpr1());
            Visitor::visit(decr, element->getExpr1());
            Visitor::visit(incr, element->getExpr2());
            Visitor::visit(decr, element->getExpr2());
        } else {                    // equal
            if (element->getExpr1()->getEval() == element->getExpr2()->getEval()) { return; }
            Visitor::visit(incr, element->getExpr1());
            Visitor::visit(decr, element->getExpr1());
            Visitor::visit(incr, element->getExpr2());
            Visitor::visit(decr, element->getExpr2());
        }
    }

    void InterestingSMCTransitionVisitor::_accept(const PQL::CompareConjunction *element)
    {
        // TODO implement this
    }

    void InterestingSMCTransitionVisitor::_accept(const PQL::AndCondition *element)
    {
        for (auto &c : *element) Visitor::visit(this, c);
    }
}
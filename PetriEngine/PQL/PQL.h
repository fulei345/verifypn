/* PeTe - Petri Engine exTremE
 * Copyright (C) 2011  Jonas Finnemann Jensen <jopsen@gmail.com>,
 *                     Thomas Søndersø Nielsen <primogens@gmail.com>,
 *                     Lars Kærlund Østergaard <larsko@gmail.com>,
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
#ifndef PQL_H
#define PQL_H
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <map>
#include <memory>

#include "../PetriNet.h"
#include "../Structures/State.h"

namespace llvm {
    class Value;
    class BasicBlock;
    class LLVMContext;
}

namespace PetriEngine {
    namespace PQL {

        class AnalysisContext;
        class EvaluationContext;
        class DistanceContext;
        class ConstraintAnalysisContext;
        class TAPAALConditionExportContext;

        /** Representation of a PQL error */
        class ExprError {
            std::string _text;
            int _length;
        public:

            ExprError(std::string text = "", int length = 0) {
                _text = text;
                _length = length;
            }

            /** Human readable explaination of the error */
            const std::string& text() const {
                return _text;
            }

            /** length in the source, 0 if not applicable */
            int length() const {
                return _length;
            }

            /** Convert error to string */
            std::string toString() const {
                return "Parsing error \"" + text() + "\"";
            }

            /** True, if this is a default created ExprError without any information */
            bool isEmpty() const {
                return _text.empty() && _length == 0;
            }
        };

        /** Representation of an expression */
        class Expr {
        public:

            /** Types of expressions */
            enum Types {
                /** Binary addition expression */
                PlusExpr,
                /** Binary subtraction expression */
                SubtractExpr,
                /** Binary multiplication expression */
                MultiplyExpr,
                /** Unary minus expression */
                MinusExpr,
                /** Literal integer expression */
                LiteralExpr,
                /** Identifier expression */
                IdentifierExpr
            };
        public:
            /** Virtual destructor, an expression should know it subexpressions */
            virtual ~Expr();
            /** Perform context analysis */
            virtual void analyze(AnalysisContext& context) = 0;
            /** True, if the expression is p-free */
            virtual bool pfree() const = 0;
            /** Evaluate the expression given marking and assignment */
            virtual int evaluate(const EvaluationContext& context) const = 0;
            /** Generate LLVM intermediate code for this expr  */
            //virtual llvm::Value* codegen(CodeGenerationContext& context) const = 0;
            /** Convert expression to string */
            virtual std::string toString() const = 0;
            /** Expression type */
            virtual Types type() const = 0;
        };

        /** Base condition */
        class Condition {
            bool _inv = false;
            std::vector<std::string> _placenameforbound;
            std::vector<size_t> _placeids;
            size_t _bound = 0;
        public:
            /** Virtual destructor */
            virtual ~Condition();
            /** Evaluate condition */
            bool evaluate(Structures::State& state, const PetriNet* net);
            /** Perform context analysis  */
            virtual void analyze(AnalysisContext& context) = 0;
            /** Evaluate condition */
            virtual bool evaluate(const EvaluationContext& context) const = 0;
            /** Analyze constraints for over-approximation */
            virtual void findConstraints(ConstraintAnalysisContext& context) const = 0;
            /** Generate LLVM intermediate code for this condition  */
            //virtual llvm::Value* codegen(CodeGenerationContext& context) const = 0;
            /** Convert condition to string */
            virtual std::string toString() const = 0;
            /** Export condition to TAPAAL query (add EF manually!) */
            virtual std::string toTAPAALQuery(TAPAALConditionExportContext& context) const = 0;
            /** Get distance to query */
            virtual uint32_t distance(DistanceContext& context) const = 0;
            
            void setInvariant(bool isInvariant)
            {
                _inv = isInvariant;
            }
           
            size_t getBound()
            {
                return _bound;
            }
            
            bool isInvariant()
            {
                return _inv;
            }
            
            void setPlaceNameForBounds(std::vector<std::string>& b)
            {
                _placenameforbound  = b;
            }
            
            void indexPlaces(const std::map<std::string, uint32_t>& map)
            {
                _placeids.clear();
                for(auto& i : _placenameforbound)
                {
                    _placeids.push_back(map.at(i));
                }
                std::sort(_placeids.begin(), _placeids.end());
            }
            
            std::vector<std::string>& placeNameForBound(){
                return _placenameforbound;
            }
        };
        typedef std::shared_ptr<Condition> Condition_ptr;
        typedef std::shared_ptr<Expr> Expr_ptr;
    } // PQL
} // PetriEngine

#endif // PQL_H

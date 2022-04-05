/* Copyright (C) 2020  Alexander Bilgram <alexander@bilgram.dk>,
 *                     Peter Haar Taankvist <ptaankvist@gmail.com>,
 *                     Thomas Pedersen <thomas.pedersen@stofanet.dk>
 *                     Andreas H. Klostergaard
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

#include "SMS/SMCPetriNetBuilder.h"
#include "utils/errors.h"

#include <tuple>
using std::get;
namespace PetriEngine {
    SMCPetriNetBuilder::SMCPetriNetBuilder(){
    }

    SMCPetriNetBuilder::SMCPetriNetBuilder(const SMCPetriNetBuilder& orig)
    : _placenames(orig._placenames), _transitionnames(orig._transitionnames),
       _places(orig._places), _transitions(orig._transitions)
    {
    }

    void SMCPetriNetBuilder::addPlace(const std::string& name, uint32_t tokens, double x, double y) {
        _ptBuilder.addPlace(name, tokens, x, y);
    }

    void ColoredPetriNetBuilder::addPlace(const std::string& name, const Colored::ColorType* type, Colored::Multiset&& tokens, double x, double y) {
        // LOL
    }

    void SMCPetriNetBuilder::addTransition(const std::string& name, int32_t player, double x, double y) {
        _ptBuilder.addTransition(name, player, x, y);
    }

    void SMCPetriNetBuilder::addTransition(const std::string& name, const Colored::GuardExpression_ptr& guard, int32_t player, double x, double y) {
        // LOL
    }

    void SMCPetriNetBuilder::addInputArc(const std::string& place, const std::string& transition, bool inhibitor, int weight) {
        _ptBuilder.addInputArc(place, transition, inhibitor, weight);
    }

    void SMCPetriNetBuilder::addInputArc(const std::string& place, const std::string& transition, const Colored::ArcExpression_ptr& expr, bool inhibitor, int weight) {
        addArc(place, transition, expr, true, inhibitor, weight);
    }

    void SMCPetriNetBuilder::addOutputArc(const std::string& transition, const std::string& place, int weight) {
        _ptBuilder.addOutputArc(transition, place, weight);
    }

    void SMCPetriNetBuilder::addOutputArc(const std::string& transition, const std::string& place, const Colored::ArcExpression_ptr& expr) {
        addArc(place, transition, expr, false, false, 1);
    }

    void SMCPetriNetBuilder::addArc(const std::string& place, const std::string& transition, const Colored::ArcExpression_ptr& expr, bool input, bool inhibitor, int weight) {
        if(_transitionnames.count(transition) == 0)
            throw base_error("Transition '", transition, "' not found. ");
        if(_placenames.count(place) == 0)
            throw base_error("Place '", place, "' not found. ");
        uint32_t p = _placenames[place];
        uint32_t t = _transitionnames[transition];

        assert(t < _transitions.size());
        assert(p < _places.size());

        if(input) _places[p]._post.emplace_back(t);
        else      _places[p]._pre.emplace_back(t);

        Colored::Arc arc;
        arc.place = p;
        arc.transition = t;
        _places[p].inhibitor |= inhibitor;
        if(!inhibitor)
            assert(expr != nullptr);
        arc.expr = std::move(expr);
        arc.input = input;
        arc.weight = weight;
        if(inhibitor){
            _inhibitorArcs.push_back(std::move(arc));
        } else {
            input? _transitions[t].input_arcs.push_back(std::move(arc)): _transitions[t].output_arcs.push_back(std::move(arc));
        }

    // }

    // void ColoredPetriNetBuilder::addColorType(const std::string& id, const Colored::ColorType* type) {
    //     _colors[id] = type;
    // }

    void SMCPetriNetBuilder::sort() {
    }
}

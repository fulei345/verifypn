/*
 * File:   StubbornSet.h
 * Author: Nikolaj J. Ulrik <nikolaj@njulrik.dk>
 *
 * Created on 03/02/2021
 */

#ifndef VERIFYPN_STUBBORNSET_H
#define VERIFYPN_STUBBORNSET_H

#include "PetriEngine/PetriNet.h"
#include "PetriEngine/Structures/State.h"
#include "PetriEngine/Structures/light_deque.h"
#include "PetriEngine/PQL/PQL.h"

#include <memory>
#include <vector>

namespace PetriEngine {
    class StubbornSet {
    public:
        StubbornSet(const PetriEngine::PetriNet &net)
                : _net(net), _inhibpost(net._nplaces) {
            _current = 0;
            _enabled = std::make_unique<bool[]>(net._ntransitions);
            _stubborn = std::make_unique<bool[]>(net._ntransitions);
            _dependency = std::make_unique<uint32_t[]>(net._ntransitions);
            _places_seen = std::make_unique<uint8_t[]>(_net.numberOfPlaces());
            StubbornSet::reset();
            constructPrePost();
            constructDependency();
            checkForInhibitor();

        }

        StubbornSet(const PetriEngine::PetriNet &net, const std::vector<PQL::Condition_ptr> &queries)
                : StubbornSet(net) {
            for (auto &q: queries) {
                _queries.push_back(q.get());
            }
        }

        virtual void prepare(const Structures::State *marking) = 0;

        size_t next();

        virtual ~StubbornSet() = default;

        virtual void reset();

        [[nodiscard]] const MarkVal *getParent() const {
            return _parent->marking();
        }

        uint32_t _current;

        void presetOf(uint32_t place, bool make_closure = false);

        void postsetOf(uint32_t place, bool make_closure = false);

        void postPresetOf(uint32_t t, bool make_closure = false);

        void inhibitorPostsetOf(uint32_t place);

        bool seenPre(uint32_t place) const;

        bool seenPost(uint32_t place) const;

        uint32_t leastDependentEnabled();

        uint32_t fired() {
            return _current;
        }

        void setQuery(PQL::Condition *ptr) {
            _queries.clear();
            _queries = {ptr};
        }

    protected:
        const PetriEngine::PetriNet &_net;
        const Structures::State *_parent;

        struct place_t {
            uint32_t pre, post;
        };

        struct trans_t {
            uint32_t index;
            int8_t direction;

            trans_t() = default;

            trans_t(uint32_t id, int8_t dir) : index(id), direction(dir) {};

            bool operator<(const trans_t &t) const {
                return index < t.index;
            }
        };

        size_t _tid = 0;

        const std::vector<TransPtr> &transitions() { return _net._transitions; }

        const std::vector<Invariant> &invariants() { return _net._invariants; }

        const std::vector<uint32_t> &placeToPtrs() { return _net._placeToPtrs; }

        bool checkPreset(uint32_t t);

        inline void addToStub(uint32_t t);

        void closure();

        std::unique_ptr<bool[]> _enabled, _stubborn;
        std::unique_ptr<uint8_t[]> _places_seen;
        std::unique_ptr<place_t[]> _places;
        std::unique_ptr<trans_t[]> _transitions;
        light_deque<uint32_t> _unprocessed, _ordering;
        std::unique_ptr<uint32_t[]> _dependency;
        bool _netContainsInhibitorArcs;
        std::vector<std::vector<uint32_t>> _inhibpost;

        std::vector<PQL::Condition *> _queries;

        void constructEnabled();

        void constructPrePost();

        void constructDependency();

        void checkForInhibitor();
    };
}

#endif //VERIFYPN_STUBBORNSET_H

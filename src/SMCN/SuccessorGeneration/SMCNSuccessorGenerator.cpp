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

#include <random>

#include "SMCN/SuccessorGeneration/SMCNSuccessorGenerator.h"
#include "PetriEngine/Structures/State.h"

namespace SMCN
{
    using namespace PetriEngine;

    SMCNSuccessorGenerator::SMCNSuccessorGenerator(const PetriNet &net)
    : SuccessorGenerator(net){}

    bool SMCNSuccessorGenerator::next(Structures::State& write, uint32_t &tindex)
    {
        _parent = &write;
        std::vector<u_int32_t> enabled;
        std::vector<u_int32_t> enabledPotencies;

        std::random_device rd;
        std::mt19937 gen(rd());
        
        for (_suc_pcounter = 0; _suc_pcounter < _net.numberOfPlaces(); ++_suc_pcounter)
        {
            // orphans are currently under "place 0" as a special case
            if (_suc_pcounter == 0 || (*_parent).marking()[_suc_pcounter] > 0)
            {
                tindex = _net.placeToPtrs()[_suc_pcounter];
                uint32_t last = _net.placeToPtrs()[_suc_pcounter + 1];
                for (; tindex < last; ++tindex)
                {
                    if (!checkPreset(tindex))
                    {
                        continue;
                    }
                    else
                    {
                        enabled.push_back(tindex);
                        enabledPotencies.push_back(_net.transitionPotency()[tindex]);
                    }
                }
            }
        }
        // Set tindex to chosen transition so we can read it in main
        if(!enabled.empty())
        {
            //uniform
            std::uniform_int_distribution<> distr(0, enabled.size());
            int select_tindex = distr(gen);
            tindex = enabled[select_tindex];

            //non-uniform (also uniform)
            // std::discrete_distribution<> distr(enabledPotencies.begin(), enabledPotencies.end());
            // int select_tindex = distr(gen);
            // tindex = enabled[select_tindex];
            
            _fire(write, tindex);
            return true;
        }
        tindex = std::numeric_limits<uint32_t>::max();;
        return false;
    }
}

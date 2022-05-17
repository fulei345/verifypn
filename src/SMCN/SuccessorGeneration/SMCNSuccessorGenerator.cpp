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
#include <chrono>
#include "SMCN/SuccessorGeneration/SMCNSuccessorGenerator.h"
#include "PetriEngine/Structures/State.h"

namespace SMCN
{
    using namespace PetriEngine;

    SMCNSuccessorGenerator::SMCNSuccessorGenerator(const PetriNet &net)
    : SuccessorGenerator(net){}

    bool SMCNSuccessorGenerator::next(Structures::State& write, uint32_t &tindex, int64_t &timer)
    {
        _parent = &write;
        std::vector<u_int32_t> enabled;
        std::vector<u_int32_t> enabledPotencies;

        std::random_device rd;
        std::mt19937 gen(rd());
        int sum = 0;
        // begin timer
        auto begin = std::chrono::high_resolution_clock::now();
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
                        int potency = _net.transitionPotency()[tindex];
                        sum += potency;
                        enabled.push_back(tindex);
                        enabledPotencies.push_back(potency);
                    }
                }
            }
        }
        // Set tindex to chosen transition so we can read it in main
        if(!enabled.empty())
        {
            //uniform
            // int current_sum = 0;
            // std::uniform_int_distribution<> distr(0, enabled.size());
            // int r = distr(gen);
            // for (size_t i = 0; i < enabled.size(); i++)
            // {
            //     ++current_sum;
            //     if (current_sum >= r)
            //     {
            //         tindex = enabled[i];
            //         break;
            //     }
            // }

            //non-uniform (also uniform)
            int current_sum = 0;
            std::uniform_int_distribution<> distr(0, sum);
            int r = distr(gen);
            for (size_t i = 0; i < enabled.size(); i++)
            {
                current_sum += enabledPotencies[i];
                if (current_sum >= r)
                {
                    tindex = enabled[i];
                    break;
                }
            }

            _fire(write, tindex);
            // end timer
            auto end = std::chrono::high_resolution_clock::now();
            timer += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            return true;
        }
        tindex = std::numeric_limits<uint32_t>::max();
        // end timer
        auto end = std::chrono::high_resolution_clock::now();
        timer += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        return false;
    }
}

#ifndef MAKE_NEIGHBORHOOD_H
#define MAKE_NEIGHBORHOOD_H

// STL
#include <vector>

// Paradiseo
#include <eoPop.h>
#include <utils/eoRNG.h>

// CODEA
#include "core/neighborhood.h"
#include "neighborhoods/staticNeighborhood.h"



// Star topology - All agents will broadcast messages to all
template <class EOT>
void do_make_star_topology(eoPop<EOT>& _population)
{
    for (size_t i = 0; i < _population.size(); i++)
        _population[i].setNeighborhood(new staticNeighborhood<EOT>(_population));
}

// Ring topology - Each agent a_i sends a message to the next a_{i+1} and the previous agent a_{i-1}
template <class EOT>
void do_make_ring_topology(eoPop<EOT>& _population)
{
    for (size_t i = 0; i < _population.size(); i++)
    {
        unsigned id1 = (int(i - 1) < 0)? _population.size() - 1 : (i - 1);
        unsigned id2 = (i + 1) > (_population.size() - 1)? 0 : (i + 1);

        _population[i].setNeighborhood(new staticNeighborhood<EOT>);

        agent<EOT>* _agent = static_cast<agent<EOT>* >(&_population[id1]);
        _population[i].getNeighborhood()->addRecipient(_agent);

        _agent = static_cast<agent<EOT>* >(&_population[id2]);
        _population[i].getNeighborhood()->addRecipient(_agent);
    }
}

// K-random topology (Each agent a_i, sends its information to k random agents)
template <class EOT>
void do_make_krandom_topology(eoPop<EOT>& _population)
{
    std::vector<unsigned> ids;
    unsigned candidate = 0, k = 0;
    for (size_t j = 0; j < _population.size(); j++)
    {
        // At least one agent to share
        k = 1 + rng.uniform(_population.size() - 2);
        ids.clear();
        while (ids.size() < k)
        {
            candidate = rng.uniform(_population.size());
            if (find(ids.begin(), ids.end(), candidate) == ids.end())
                ids.push_back(candidate);
        }

        _population[j].setNeighborhood(new staticNeighborhood<EOT>);
        for (size_t i = 0; i < ids.size(); i++)
            _population[j].getNeighborhood()->addRecipient(static_cast<agent<EOT>* >(&_population[ids[i]]));
    }
}


#endif

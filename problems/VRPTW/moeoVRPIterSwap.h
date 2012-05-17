#ifndef MOEOVRPITERSWAP_H
#define MOEOVRPITERSWAP_H

// Paradiseo
#include <mo>

// CODEA
#include "moNeighborhoodExplorer.h"
#include "moeoVRP.h"

template <class EOT>
class moeoVRPIterSwap : public moNeighborhoodExplorer <EOT>
{


public:

    moeoVRPIterSwap() : i(-1), k(1), state(true)
    { }
    
    moeoVRPIterSwap(unsigned _i, unsigned _k) : i(_i), k(_k), state(true)
    {
        assert(_k > 0);
    }

    // These are the bounds for both indices
    // 0 <= i < length - k - 1
    // 1 <= j < length - 1
    void updateParam(EOT& _routePlan)
    {
        unsigned length = _routePlan.size();

        //std::cout << std::endl;
        //std::cout << " Before: " << i << " " << i + k << " l:" << length << std::endl;
        if (i + k >= length - 1)
        {
            if (k >= length - 1)
            {
                state = false;
                return;
            }
            i = 0;
            k++;      
            //std::cout << " After: " << i << " " << i + k << " l:" << length << std::endl;
            return;
        }
        i++;

        //std::cout << " After: " << i << " " << i + k << " l:" << length << std::endl;
        //std::cout << std::endl;
        
    }

    bool reset(EOT &_solution)
    {
        return true;
    }
    
    void initParam(EOT& _routePlan)
    {
        state = true;
        i = -1;
        k = 1;
    }

    void undo(EOT& _routePlan)
    {
        std::swap(_routePlan[i], _routePlan[i + k]);
    }

    bool move(EOT& _routePlan)
    {
        // No more neighbors
        if (!state)
            return false;

        updateParam(_routePlan);
        std::swap(_routePlan[i], _routePlan[i + k]);
        _routePlan.invalidate();
        return true;
    }

    bool accept(EOT &_solution)
    {
        return true;
    }

    void terminate(EOT &_solution)
    { }

private:

    int i;
    unsigned k;

    bool state;
};

#endif // MOEOVRPLOCALSEARCH_H

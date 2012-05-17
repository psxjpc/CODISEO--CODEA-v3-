#ifndef EOSINGLEOP_H
#define EOSINGLEOP_H

// Paradiseo
#include <eoOpContainer.h>

// CODEA
#include "core/eoQuadSingleGenOp.h"
#include "core/eoMonSingleGenOp.h"

/** Single Operation:
 *     Only one operator is randomly applied. The operator
 *     is selected using the given rates.
 */
template <class EOT>
class eoSingleOp : public eoOpContainer<EOT>
{
    
private:
    
    int crossOverIndex;
    int mutationIndex;
    
protected:
    
    void findIndices()
    {
        if (crossOverIndex == -1 || mutationIndex == -1)
            for (size_t i = 0; i < ops.size(); i++)
            {
                if (ops[i]->className() == "eoPropCombinedQuadOp")
                    crossOverIndex = i;
                else if (ops[i]->className() == "eoPropCombinedMonOp")        
                    mutationIndex = i;
            }
    }
    
    
public:
    
    
    using eoOpContainer<EOT>::ops;
    using eoOpContainer<EOT>::rates;
    
    typedef unsigned position_type;
    
    eoSingleOp()
    {
        crossOverIndex = -1;
        mutationIndex  = -1;
    }
    
    void apply(eoPopulator<EOT>& _pop)   
    { }
    
    // Crossover
    bool apply(EOT& _individual1, EOT& _individual2)
    {
        findIndices();
        eoQuadSingleGenOp<EOT>* op = static_cast<eoQuadSingleGenOp<EOT>*> (ops[crossOverIndex]);
        return op->apply(_individual1, _individual2);
    }
    
    // Mutation
    bool apply(EOT& _individual)
    {
        findIndices();        
        eoMonSingleGenOp<EOT>* op = static_cast<eoMonSingleGenOp<EOT>*> (ops[mutationIndex]);
        
        _individual.invalidate();
        return op->apply(_individual);
        
    }
    
    
    virtual std::string className() const {return "SingleOp";}
    
};


#endif // EOSINGLEOP_H

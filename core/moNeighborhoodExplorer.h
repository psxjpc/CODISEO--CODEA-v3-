#ifndef MONEIGHBORHOODEXPLORER_H
#define MONEIGHBORHOODEXPLORER_H

#include <eoFunctor.h>

template< class EOT >
class moNeighborhoodExplorer
{

public:

    virtual void initParam (EOT& _solution) = 0 ;

    virtual void updateParam (EOT& _solution) = 0 ;

    virtual bool reset(EOT& _solution) = 0 ;

    virtual bool move(EOT& _solution) = 0 ;

    virtual bool accept(EOT& _solution) = 0 ;

    virtual void terminate(EOT& _solution) = 0 ;
};

#endif // MONEIGHBORHOODEXPLORER_H

#ifndef EOMONSINGLEGENOP_H
#define EOMONSINGLEGENOP_H

#include <eoOp.h>
#include <eoGenOp.h>
#include <eoProportionalCombinedOp.h>

/** CODISEO Wrapper for MonOp
 */
template <class EOT>
class eoMonSingleGenOp : public eoGenOp<EOT>
{

public:

    eoMonSingleGenOp(eoMonOp<EOT>& _op) : op(_op) {}

    unsigned max_production(void) { return 1; }

    eoMonOp<EOT>& getOperators()
    {
        return op;
    }

    void apply(eoPopulator<EOT>& _it)
    {
        if (op(*_it))
            (*_it).invalidate();
    }

    bool apply(EOT& _indi)
    {
        _indi.invalidate();
        eoPropCombinedMonOp<EOT>* operators = static_cast<eoPropCombinedMonOp<EOT>*>(&op);
        return operators->operator()(_indi);
    }

    virtual std::string className() const {return op.className();}

private :
    eoMonOp<EOT>& op;

};

#endif // EOMONSINGLEGENOP_H

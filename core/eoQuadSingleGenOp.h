#ifndef EOQUADSINGLEGENOP_H
#define EOQUADSINGLEGENOP_H

#include <eoOp.h>
#include <eoGenOp.h>
#include <eoProportionalCombinedOp.h>

/** CODISEO Wrapper for quadop
 */
template <class EOT>
class eoQuadSingleGenOp : public eoGenOp<EOT>
{

public:
    eoQuadSingleGenOp(eoQuadOp<EOT>& _op) : op(_op) {}

    unsigned max_production(void) { return 2; }

    eoQuadOp<EOT>& getOperators()
    {
        return op;
    }

    void apply(eoPopulator<EOT>& _pop)
    {
        EOT& a = *_pop;
        EOT& b = *++_pop;


        if(op(a, b))
        {
            a.invalidate();
            b.invalidate();
        }

    }

    bool apply(EOT& _indi1, EOT& _indi2)
    {
        eoPropCombinedQuadOp<EOT>* operators = static_cast<eoPropCombinedQuadOp<EOT>*>(&op);
        return operators->operator()(_indi1, _indi2);
    }

    virtual std::string className() const {return op.className();}

private :
    eoQuadOp<EOT>& op;

};


#endif // EOQUADSINGLEGENOP_H

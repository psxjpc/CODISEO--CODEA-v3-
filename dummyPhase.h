#ifndef DUMMYPHASE_H
#define DUMMYPHASE_H

#include <vector>

#include "core/agent.h"
#include "core/phase.h"

#include "fitness/moeoDominanceDepthFitnessAssignment.h"

template <class EOT>
class dummyPhase : public phase<EOT>
{

private:

protected:

public:
    void prePhase(EOT* _agent, eoPop<EOT>* _pop)
    {

    }

    void corePhase(EOT* _agent, eoPop<EOT>* _pop)
    {
        // We will try to access some data and operations from here
        std::cout << "I am Agent [" << _agent->id() << "]" << std::endl;
        std::cout << *_agent << std::endl;

    }

    void postPhase(EOT* _agent, eoPop<EOT>* _pop)
    {
       // We will send some info to other agents from here
       // EOT _individual = (*population)[0];
       // agent<EOT>* _agent = static_cast<agent<EOT>* >(&_individual);

    }

    void operator () (eoPop<EOT>& _pop)
    { }

    void operator () (EOT* agent, eoPop<EOT>& pop)
    { }

};

#endif // DUMMYPHASE_H

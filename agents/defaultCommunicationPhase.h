#ifndef DEFAULTCOMMUNICATIONPHASE_H
#define DEFAULTCOMMUNICATIONPHASE_H

/**
   @class defaultCommunicationPhase

   @brief Carries out a by-default Communication Phase.

   This class is designed to implement the steps to share information
   with other agents. To do this, it starts retrieving the list of
   recipients depending on the neighborhood of this agent. Next, it
   packs the message to be sent. And finally, it puts the message
   on neighbor's mailboxes.

   @author Group of Intelligent Computing - Universidad de La Laguna - 2008
   @author ASAP Group                     - Nottingham University    - 2011
*/

#include <vector>

#include <boost/shared_ptr.hpp>

#include <CODEATypes.h>
#include <phase.h>

template <class EOT>
class defaultCommunicationPhase : public phase<EOT>
{
   private:
    tMessage parcel;

   protected:

   public:
      /**
         Default constructor. It does nothing.
      */
      defaultCommunicationPhase()
      { };

      /**
         Default destructor. It does nothing.
      */
      ~defaultCommunicationPhase()
      { };

      defaultCommunicationPhase<EOT>* clone() const
      { return new defaultCommunicationPhase(*this); }


      void createMessage(EOT* _agent, eoPop<EOT>& _pop)
      {
          // Remove any previous content
          parcel.clear();
          
          // Sender
          container data("Sender", _agent);
          parcel.push_back(data);

          // Let's start the tests by sending the best solution found by the agent.
          //    We will see if what happen when we send other solutions.

          // A copy of the agent will be sent
          data.set("Solution", boost::shared_ptr<EOT>(new EOT(*_agent)));
          parcel.push_back(data);
      }

      // Operator controlling just one individual
      void operator() (EOT* _agent, eoPop<EOT>& _pop)
      {
          createMessage(_agent, _pop);
          std::vector<agent<EOT>*>* group = _agent->getNeighborhood()->list();
          for (size_t i = 0; i < group->size(); i++)
              // Sanity check to avoid sending to oneself
              if (_agent->getId() != (*group)[i]->getId())
                 (*group)[i]->queueMessage(parcel);
      }

      // Operator controlling not one but the whole population
      void operator() (eoPop<EOT>& _pop)
      { }


};

#endif // DEFAULTCOMMUNICATIONPHASE_H

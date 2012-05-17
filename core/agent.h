/** 
   @class agent

   @brief Contains propperties and methods agents work with at a high
   level. 

   This class doesn't have directions on how an agent behaves,
   but a basic scheme of all the components it uses to carry out
   its operations. These components are the attributes (explained
   below) that it needs to cooperate, perform solving operations,
   save temporary information, etcetera.

   @author Group of Intelligent Computing - Universidad de La Laguna - 2008
*/


#ifndef AGENT_H
#define AGENT_H

#include <deque>
#include <vector>

#include "container.h"
#include "CODEATypes.h"
#include "mailBox.h"
#include "neighborhood.h"
#include "phase.h"

#include "../libs/concurrentQueue.h"

typedef std::vector<container> tMessage;

using std::vector;
using std::deque;

template <class EOT>
class neighborhood;

template <class EOT>
class agent
{

private:

    /**
         Agent's id. This is just a way to identify each agent.         
      */
    unsigned id;

    /**
         Agent's state. It helps to disable an agent and its operations.         
      */
    bool active;

    /**
         Agent's neighborhood. Current list of neighboors the agent will communication with.
      */
    neighborhood<EOT>* recipients;

    /**
         Agent's phases. List of states the agent will go through repeteadly. This is the 
            way an agent has to perform its operations at all levels.
      */
    std::vector<phase<EOT>*> phases;

    /**
         Agent's mailbox. Place the agent has to storage the incoming mesages.
      */
    mailBox inbox;


protected:


public:

    /**
         Default constructor. It does nothing.
      */
    agent()
    {
        id = 0;
        active = true;
        recipients = NULL;

        concurrentQueue<tMessage> _inbox;
        inbox.setInbox(_inbox);

    }

    /**
         Standard constructor.
         @param idAgentType is the identificator for the agent.
         @param core* is a pointer to the core of the agent.
         @param deque<message> is the data structure the agent is going to use to store the incoming messages.
         @param neighborhood* is the list of agents it is going to communicate with.
         @param vector<phase*> is the list of phases it is going to go through.
         @return an agent with the given parameters.
      */
    agent(const unsigned _id,
          const concurrentQueue<tMessage> _inbox, neighborhood<EOT>* _recipients,
          const vector<phase<EOT>*> _phases)
    {
        // By default, the agent is active
        active = true;

        id = _id;
        inbox.setInbox(_inbox);
        recipients = _recipients;
        phases = _phases;

    }

    agent(const EOT& _agent)
    {
        // Copy contructor will NOT copy the mailbox.
        id = _agent.id;
        active = _agent.active;
        recipients = _agent.recipients;

        // Copying phases
        phases.resize(_agent.phases.size());
        for (size_t i = 0; i < phases.size(); i++)
           if (_agent.phases[i] != NULL)
               phases[i] = (*_agent.phases[i]).clone();
           else
               phases[i] = NULL;




    }

    agent& operator= (const agent& _agent)
    {
       if (this != &_agent)
       {
          // Copy contructor will NOT copy the mailbox.
          id = _agent.id;
          active = _agent.active;
          recipients = _agent.recipients;

          // Copying phases
          phases.resize(_agent.phases.size());
          for (size_t i = 0; i < phases.size(); i++)
             if (_agent.phases[i] != NULL)
                 phases[i] = (*_agent.phases[i]).clone();
             else
                 phases[i] = NULL;
       }
       return *this;
    }


    /**
         Default destructor. It frees all the memory used by this class.
      */
    ~agent()
    {

       // Freing phases
       for (size_t i = 0; i < phases.size(); i++)
           delete(phases[i]);
    }

    /**
         Method that returns the agent's id.
         @return agent's id.
      */
    unsigned getId() const
    {
        return id;
    }

    /**
         Method that returns whether the agent is active or not.
         @return agent's state.
      */
    bool isActive() const
    {
        return active;
    }

    /**
         Method that returns the agent's neighborhood.
         @return a pointer to agent's neighborhood.
      */
    neighborhood<EOT>* getNeighborhood() const
    {
        return recipients;
    }

    /**
         Method that returns the agent's phases.
         @return agent's phases.
      */
    vector<phase<EOT>*> getPhases() const
    {
        return phases;
    }


    phase<EOT>* getPhase(unsigned _phaseIndex)
    {
        return phases[_phaseIndex];
    }

    /**
         Method that returns the agent's mailbox.
         @return a pointer to agent's mailbox.
      */
    concurrentQueue<tMessage>* getPointerToInbox()
    {
        return inbox.getPointerToInbox();
    }

    /**
         Method to queue a new message.
      */
    void queueMessage(const tMessage& _message)
    {
        inbox.push_back(_message);
    }

    /**
         Method that returns the agent's mailbox.
         @return agent's mailbox.
      */
    const concurrentQueue<tMessage>& getInbox() const
    {
        return inbox.getInbox();
    }
    
    unsigned numberOfPhases() const
    {
        return phases.size();
    }

    /**
         Method that sets the agent's id.
         @param agent's id.
      */
    void setId(const unsigned _id)
    {
        id = _id;
    }

    /**
         Method that sets the agent's state.
         @param agent's state.
      */
    void setActive(const bool _active)
    {
        active = _active;
    }

    /**
         Method that sets the agent's neighborhood.
         @param a pointer to agent's neighborhood.
      */
    void setNeighborhood(neighborhood<EOT>* _recipients)
    {
        recipients = _recipients;
    }

    /**
         Method that sets the agent's mailbox.
         @param agent's mailbox.
      */
    void setInBox(const concurrentQueue<tMessage>& _inbox)
    {
        inbox.setInbox(_inbox);
    }

    /**
         Method that sets the agent's phases.
         @param agent's phases.
      */
    void setPhases(const vector<phase<EOT>*> _phases)
    {
        phases = _phases;
    }

    /**
         Method that adds a phase the agent's phases vector.
         @param agent's phase.
      */
    void addPhase(phase<EOT>* _phase)
    {
        phases.push_back(_phase);
    }

    /**

      */
    void clearMailBox()
    {
        inbox.clear();
    }

    /**
         Method that invokes an agent's phase
         @param agent's phase index
      */
    void startPhase(const unsigned _phaseIndex, eoPop<EOT>& _population)
    {
        phases[_phaseIndex]->operator()(static_cast<EOT*>(this), _population);
    }
};




#endif

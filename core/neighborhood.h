/**
   @class neighborhood
  
   @brief Interface for creating neighborhoods.

   This class acts as an interface between the agent class and user neighborhoods classes. 
   This implements the attributes and methods needed to manage a wide variety of neighboring
   topologies.

   From a 'state' point of view, we can discern two types of communication topologies:
   - static.
   - dynamic.

   In order to optimize both topologies, this class has been conceived to 
   store a pointer to the agents vector for dynamic topologies and a pointer
   to a vector with the agents for the current topology in case of static
   topologies.

   This is due to with static topologies it is not necessary to change the 
   neighbors list in real time. It is just needed to initialize a 
   static list of neighbors for each agent. For dynamic neighborhoods,
   the pointer to the vector of agents, must be a pointer to the same vector
   ssystem counts with. This way, any type of dynamic neighborhood inherited 
   from this class are not to change this vector, instead it will has its
   own data-structure. For example, suppose we are going to work with a 
   'scoreBasedNeighborhood' (SBN). This type of neighborhood will have access 
   to the vector of agents whithin the system. For creating the SBN, it 
   shouldn't try to modify this vector but, for example, it should store 
   a vector of pairs <agentdIds, scores>. Using the phases, the agents
   would be able to update this vector of scores. At the appropiate time, 
   when the list of neighbors are needed in a communication phase, using
   this data-structure the scoreBasedNeighborhoood has and the pointer to 
   the vector of agents of ssystem, the new neighbors list would be computed.
   
   This methodology allows not to have any computation for static neigborhoods
   and flexibility for dynamic ones in cases of creation or deletation of
   agents in real time.

   @author Group of Intelligent Computing - Universidad de La Laguna - 2008
*/

#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include <vector>

#include "agent.h"
#include "CODEATypes.h"

template <class EOT>
class agent;

template <class EOT>
class neighborhood
{
   private:
      /**
         This vector or group of agents will be the recipients of the message
         to be deliver in a communication phase. It supposed to be set or 
         updated by inherited classes depending the type of neighborhood:
         - static: a vector is created and assigned.
         - dynamic: the vector that represents the group of agents is assigned.
      */
      std::vector<agent<EOT>*>* recipients;

   protected:
      /** 
         Method that returns a pointer to the list of agents
         in this class. This method is protected because it has
         been designed to be accessible only for inherited classes.
         The method that is inteded to return the neighboring list is
         a virtual method getNeighborhood (see more details bellow). 
         @return a pointer to the group of agents in this class.
      */
      std::vector<agent<EOT>*>* const getRecipients() const
      { return recipients; }



   public:

      /**
         Default constructor. It initilizes the agent's pointer to NULL.
      */
      neighborhood()
      { recipients = NULL; }
      /**
         Default constructor. It does nothing. This method is NOT
         supposed to destroy the memory in which the agents reside,
         but is ssystem that should perform this operation.
      */
      ~neighborhood() 
      {
          for (size_t i = 0; i < recipients->size(); i++)
              delete((*recipients)[i]);
          delete(recipients);
      }


      /** 
         Method that sets the neighboring list to send the message.
         @param a pointer to the group of agents who will receive the message
         in case of static neighborhoods or to work with for
         dynamic neighborhoods
      */
      void setRecipients(eoPop<EOT*>* _recipients)
      {
         if (recipients == NULL)
             recipients = new std::vector<agent<EOT>*>;

         for (size_t i = 0; i < _recipients->size(); i++)
             recipients->push_back((*_recipients)[i]);
      }

      void setRecipients(eoPop<EOT>& _recipients)
      {
          if (recipients == NULL)
            recipients = new std::vector<agent<EOT>*>;

          for (size_t i = 0; i < _recipients.size(); i++)
              recipients->push_back(&(_recipients[i]));
      }

      /** 
         Method that adds an agent to the neighboring list. 
         @param a pointer to a new recipient.
         @warning This mehod is design for static neihgboring lists only.
      */
      void addRecipient(agent<EOT>* recipient)
      {
          recipients->push_back(recipient);
      }

      /** 
         Virtual method that returns the neighboring list to send the message.
         @return a pointer to the group of agent who will receive the message.
      */
      virtual std::vector<agent<EOT>*>* const list() const = 0;

};


#endif

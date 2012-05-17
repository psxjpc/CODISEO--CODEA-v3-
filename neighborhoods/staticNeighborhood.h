/**
   @class staticNeighborhood

   @brief To work with static neighborhoods.

   This class inherits from neighborhood to deal with static communication
   topologies. The idea is to create for each agent a vector of neihgbors.
   This vector is to be assigned to the vector in the neihgborhood class using
   the method setAgents contained in the same class. Thus, this clas will
   implement the virtual method getNeighborhood that will simply return
   the pointer to the vector contained in neihgborhood class.

   @author Group of Intelligent Computing - Universidad de La Laguna - 2008
*/

#ifndef STATICNEIGHBORHOOD_H
#define STATICNEIGHBORHOOD_H

#include <vector>

#include <eoPop.h>

#include "core/neighborhood.h"

template <class EOT>
class staticNeighborhood : public neighborhood<EOT>
{
   private:

   protected:

   public:

      /**
         Default constructor. It does nothing.
      */
      staticNeighborhood() { };

      /**
         Standard constructor.
         @param vector<agent*>* const is the vector that contains the agents
         this one is related to.
         @return a static topology with the given data.
      */
      staticNeighborhood(eoPop<EOT>& listOfRecipients)
      {
         setRecipients(listOfRecipients);
      }

      /**
         Default destructor. It does nothing.
      */
      ~staticNeighborhood()
      { }

      /**
         Method that returns the list of recipients the agent is going to send its message
         @return a pointer to the vector of recipients.
      */
      std::vector<agent<EOT>*>* const list() const
      {
           return this->getRecipients();
      }


};

#endif

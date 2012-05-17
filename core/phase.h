/** 
   @class phase
  
   @brief Interface for the creation of phases.

   This abstract class acts as an interface between agents and the
   operations they carry out. The agents are designed to run a 
   number of phases repeteadly whithin the ssystem main-loop. 
   The idea is that the main-loop calls iteratively a phase
   after phase without knowing which operation an agent is about
   to perform. In other words, this is just a way to provide
   a system in which each agent has a turn (phase) to do
   whatever operation he has to do.

   This class is deliberately simple to enhace the creation of 
   phases of all types as complex as the user wants.

   This interface allows each agent to have a vector of different
   phases by using polymorphism. 

   Inherited classes must implement three classes. The idea behind
   this is to separate the code in three simple steps.
   - prePhase: Operations to be performed before main operations (initialization)
   - core: Operation this phase was designed for
   - postPhase: Operations to be performad after main operations (update)

   @author Group of Intelligent Computing - Universidad de La Laguna - 2008
*/

#ifndef PHASE_H
#define PHASE_H


#include <vector>

// Paradiseo
#include <eoAlgo.h>

#include <agent.h>

template <class EOT>
class agent;

template <class EOT>
class phase : public eoAlgo<EOT>
{
   private:

   protected:

   public:

      /**
         Default constructor. It does nothing.
      */
      phase()  { };

      /**
         Default destructor. It does nothing.
      */
      virtual ~phase() { };

      /**

        */

      /**
         Method that is invoked from the ssystem's main-loop to start the
         phase. This simply calls the inherited methods explained above.
      */
      virtual void operator () (EOT*, eoPop<EOT>&) = 0;

      virtual void operator () (eoPop<EOT>&) = 0;

      virtual phase<EOT>* clone() const = 0;


};


#endif

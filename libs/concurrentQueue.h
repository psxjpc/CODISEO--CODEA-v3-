/** 
   @class VRPSolution
  
   @brief Wapper for std::queue that enables concurrent operatrions.

   This class embends a std::queue object providing a thread-safe queue.
   To this aim, some classes borrowed from the boost library are used to
   control de access to the queue.
   
   @author Group of Intelligent Computing - Universidad de La Laguna - 2008

   Class adapted from http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
*/

#ifndef CONCURRENT_QUEUE
#define CONCURRENT_QUEUE

#include <queue>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

template<typename Data>
class concurrentQueue
{
   private:
      /**
         Standard (STL) queue that holds the information
      */
      std::queue<Data> the_queue;

      /**
         Boost variable intended to protect the queue 
      */
      mutable boost::mutex the_mutex;

      /**
         Boost variable to avoid pulling the state of the queue over and over again.
      */
      boost::condition_variable the_condition_variable;

  public:

      /**
         Default constructor. It does nothing.
      */
      concurrentQueue();
      /**
         Destructor. It frees the_queue.
      */      
      ~concurrentQueue();

      /**
         Method that returns the mutex.
         @return a boost::mutex containving the mutex
      */
      boost::mutex getMutex() const;
      /**
         Method that returns the condition variable.
         @return a boost::condition_variable containing the condition variable
      */
      boost::condition_variable getConditionVariable() const;
      /**
         Thread-safe method to push data into the queue
         @param Data const& is a const reference to the data to be pushed
      */
      void push(Data const& data);

 
      /**
         Method that checks whether the queue is empty
         @return bool checks the state
      */     
      bool empty() const;
 
      /**
         Method that returns the size of the queue
         @return size_t is the number of elements the queue holds
      */
      size_t size() const;


      /**
         Method to pop data out of the queue
         @param Data& is the reference to the data to be popped out
         @return true or false if the operation was correctly done or not
      */
      bool try_pop(Data& popped_value);

      /**
         Thread-safe method to pop data out of the queue
         @param Data& is the reference to the data to be popped out
      */ 
      void wait_and_pop(Data& popped_value);
      /**
         Method that clear the queue up
      */
      void clear();

      /**
         Method that returns the internal object the queue.
         @return std::queue is the object itself
      */
      std::queue<Data> getQueue() const;


      /**
         Method to establish the queue.
         @param const std::queue<>& is a constant reference to the queue to be set up.
      */
      void setQueue(const std::queue<Data>& newQueue);

};


template<typename Data>
concurrentQueue<Data>::concurrentQueue() 
{ }

template<typename Data>  
concurrentQueue<Data>::~concurrentQueue() 
{ clear(); }

template<typename Data>
boost::mutex concurrentQueue<Data>::getMutex() const
{
   return this->the_mutex;
}

template<typename Data>
boost::condition_variable concurrentQueue<Data>::getConditionVariable() const
{
   return this->the_condition_variable;
}

template<typename Data>
void concurrentQueue<Data>::push(Data const& data)
{
    //boost::mutex::scoped_lock lock(the_mutex);
    the_queue.push(data);
    //lock.unlock();
    //the_condition_variable.notify_one();
}


template<typename Data>
bool concurrentQueue<Data>::empty() const
{
    boost::mutex::scoped_lock lock(the_mutex);
    return the_queue.empty();
}
 
template<typename Data>
size_t concurrentQueue<Data>::size() const
{
   return this->the_queue.size();
}

template<typename Data>
bool concurrentQueue<Data>::try_pop(Data& popped_value)
{
    boost::mutex::scoped_lock lock(the_mutex);
    if(the_queue.empty())
        return false;
  
    popped_value = the_queue.front();
    the_queue.pop();
    return true;
}

template<typename Data>
void concurrentQueue<Data>::wait_and_pop(Data& popped_value)
{
    boost::mutex::scoped_lock lock(the_mutex);
    while(the_queue.empty())
        the_condition_variable.wait(lock);

    popped_value = the_queue.front();
    the_queue.pop();
}

template<typename Data> 
void concurrentQueue<Data>::clear()
{
   Data dummy;
   while (!empty())
      try_pop(dummy);
}

template<typename Data>
std::queue<Data> concurrentQueue<Data>::getQueue() const
{
   return this->the_queue;
}

template<typename Data>
void concurrentQueue<Data>::setQueue(const std::queue<Data>& newQueue) 
{
   this->the_queue = newQueue;
}


#endif


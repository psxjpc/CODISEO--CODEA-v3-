/** 
   @class mailTypeBox

   @brief Stores the messages sent by other agents.

   This class handles basic methods to manage a virtual mailTypebox of message
   sent by other agents. 

   @author Group of Intelligent Computing - Universidad de La Laguna - 2008
*/
#ifndef MAILBOX_H
#define MAILBOX_H

#include "../libs/concurrentQueue.h"
#include "CODEATypes.h"



class mailBox
{

    typedef std::vector<container> tMessage;


private:
    /**
         mailTypebox of messages. Store the messages an agent receives.
         A special type of object is used to enable thread-safe message
         passing.
      */
    concurrentQueue<tMessage> inbox;

protected:

public:
    /**
         Default constructor. It does nothing.
      */
    mailBox() { };

    /**
         Default destructor. It does nothing.
      */
    ~mailBox() { };

    /**
         Method that returns the mailTypebox.
         @return the data structure that holds the messages.
      */
    const concurrentQueue<tMessage>& getInbox() const
    {
        return inbox;
    }

    /**
         Method that returns the mailTypebox.
         @return a pointer to the data structure that holds the messages.
      */
    concurrentQueue<tMessage>* getPointerToInbox()
    {
        return &inbox;
    }

    /**
         Method that sets the mailTypebox.
         @param const mailType is the mailTypebox.
      */
    void setInbox(const concurrentQueue<tMessage>& _inbox)
    {
        inbox.setQueue(_inbox.getQueue());
    }

    /**
         Method that clears up the mailTypebox.
      */
    void clear()
    {
        inbox.clear();
    }

    /**
         Method that inserts a new message in the mailTypebox.
         @param const message is the new message to be inserted in the mailTypebox.
      */
    void insert(const tMessage _message)
    {
        inbox.push(_message);
    }

    /**
         Method that inserts a new message in the mailTypebox.
         @param const message is the new message to be inserted in the mailTypebox.
         @note This method was implemented to maintain compatibility with the STL.
      */
    void push_back(const tMessage& _message)
    {
        inbox.push(_message);
    }

    /**
         Method that returns true or false depending on whether the mailTypebox is empty or not.
         @param bool is true if the mailTypebox is empty, false otherwise.
      */
    bool empty() const
    {
        return inbox.empty();
    }

};



#endif

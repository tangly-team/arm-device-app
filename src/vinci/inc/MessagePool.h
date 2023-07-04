#ifndef VINCI_MESSAGE_POOL_H
#define VINCI_MESSAGE_POOL_H

#include "Message.h"

namespace vinci {
    /**
     * Define the abstraction of a pool of messages for the application. Concrete implementation implements various allocation mechanisms.
     * One major goal is to provide the possibility to define static sized pools for embedded applications and avoid using dynmaic memory allocation.
     * @tparam T type of the payload of the message to pool
     */
    template<typename T>
    class MessagePool {
    public:
        static const int MSG_POOL_SIZE = 10;

        /**
         * Acquire a message from the message pool.
         * @return the newly acquired message
         */
        virtual Message<T>* acquire() = 0;

        /**
         * Release the message to the message pool.
         * @param msg message to be returned to the pool
         */
        virtual void release(Message<T>* msg) = 0;

        /**
         * Return the number of available free messages in the pool.
         * @return number of available messages
         */
        virtual int nrOfFreeMsgs() = 0;
    };
}
#endif

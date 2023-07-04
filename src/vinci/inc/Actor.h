#ifndef VINCI_ACTOR_H
#define VINCI_ACTOR_H

#include <cstring>
#include "Message.h"
#include "MessagePool.h"
#include "ConcurrentQueue.h"

namespace vinci {

    /**
     * Define the abstraction of an actor. An actor is an active object with a private mailbox containing a queue of messages.
     * @tparam T type of the message payload
     */
    template<typename T>
    class Actor {
    public:
        /**
         * Maximum length of an actor name.
         */
        static const int ACTOR_NAME_SIZE = 32;

        virtual ~Actor();

        /**
         * Return the human-readable name of the actor.
         * @return name of the actor
         */
        const char* name() const noexcept;

        /**
         * Join with the actor thread.
         */
        virtual void join() = 0;

        /**
         * Send a message to the given actor.
         * @param actor actor receiving the message
         * @param msg message to be sent
         */
        static void send(Actor<T>& actor, Message<T>* msg);

    protected:
        /**
         * Constructor of the class.
         * @param name name of the actor. The name should be human-readable and a unique identifier of the active object.
         */
        Actor(const char* name, MessagePool<T>& pool) : _pool{pool} {
            strncpy(_name, name, ACTOR_NAME_SIZE);
        }

        /**
         * Process the next received message. Overwrite the method to implement the business logic of your actor.
         * The returned message can be nullptr if the waitDelay is set to a value bigger than 0.
         * @param  msg message to process
         * @return flag indicating if the processing should continue
         */
        virtual bool processMsg(Message<T>* msg) = 0;

        /**
         * Receive a message.
         * @param msg received message.
        ` */
        virtual void receive(Message<T>* msg) = 0;

        /**
         * Get the next message of the message queue. The call is blocks until message is received
         * @return the next message
         */
        virtual Message<T>* message() = 0;

        /**
         * Get the next message of the message queue. The call is blocks until message is received or timeout occurs.
         * @param ticks timeout in ticks
         * @return the next message or nullptr if timeout occurred
         */
        virtual Message<T>* message(const uint32_t ticks) = 0;

        /**
         * Suspend the actor for the specified time.
         * @param ticks delay in milliseconds
         */
        virtual void delay(const uint32_t ticks) = 0;

        /**
         * Run the body of the actor. Overwrite this method only for special needs no covered by the regular actor semantic.
         * The default behavior is to wait for the next message and process it with the @ref processMsg(const Message<T>&).
         */
        virtual void runBody();

        inline MessagePool<T>& pool() { return _pool; };

        inline uint32_t waitDelay() { return _waitDelay; };

        inline void waitDelay(uint32_t waitDelay) {
            _waitDelay = waitDelay;
        };
    private:
        char _name[ACTOR_NAME_SIZE];
        MessagePool<T>& _pool;
        uint32_t _waitDelay = 0;
    };

    template<typename T>
    const char* Actor<T>::name() const noexcept {
        return _name;
    }

    template<typename T>
    Actor<T>::~Actor() {}

    template<typename T>
    void Actor<T>::send(Actor<T>& actor, Message<T>* msg) {
        actor.receive(msg);
    }

    template<typename T>
    void Actor<T>::runBody() {
        bool continues = true;
        while (continues) {
            Message<T>* msg = (waitDelay() > 0) ? message(waitDelay()) : message();
            continues = processMsg(msg);
        }
    }
}
#endif

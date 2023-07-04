
#ifndef VINCI_ACTOR_CMSIS_H
#define VINCI_ACTOR_CMSIS_H

#include <cstring>
#include "../../vinci/inc/Actor.h"
#include "MsgPoolCmsis.h"
#include "ConcurrentQueueCmsis.h"
#include "cmsis_os2.h"

namespace vinci {

    template<typename T>
    class ActorCmsis : public Actor<T> {
    public:
        /**
         * Constructor of the class.
         * @param name human-readable name of the actor
         * @param queueSize maximum size of the message queue
         * @param attributes attributes for the OS task creation
         */
        explicit ActorCmsis(const char* name, int queueSize, osThreadAttr_t& attributes, MsgPoolCmsis<T>& pool);

        virtual ~ActorCmsis();

        virtual void join();

    protected:

        virtual void receive(Message<T>* msg);

        virtual Message<T>* message();

        virtual Message<T>* message(const uint32_t waitForMilliSeconds);

        virtual void delay(const uint32_t milliseconds);

    private:
        /**
         * Synthetic method to connect the C++ class with the CMSIS OS thread.
         * @param self pointer to the class instance
         */
        static void run(void* self);

        ConcurrentQueueCmsis<Message<T>*> _queue;
        osThreadId_t _task;
    };

    template<typename T>
    ActorCmsis<T>::ActorCmsis(const char* name, int queueSize, osThreadAttr_t& attributes, MsgPoolCmsis<T>& pool) : Actor<T>(name, pool), _queue{queueSize} {
        strncpy(attributes.name, name, Actor<T>::ACTOR_NAME_SIZE);
        _task = osThreadNew(&ActorCmsis<T>::run, this, &attributes);
    }

    template<typename T>
    ActorCmsis<T>::~ActorCmsis() {}

    template<typename T>
    void ActorCmsis<T>::receive(Message<T>* msg) {
        _queue.push(msg);
    }

    template<typename T>
    Message<T>* ActorCmsis<T>::message() {
        return _queue.pop();
    }

    template<typename T>
    Message<T>* ActorCmsis<T>::message(const uint32_t ticks) {
        std::optional<Message<T>*> msg = _queue.pop(ticks);
        return msg.has_value() ? msg.value() : nullptr;
    }

    template<typename T>
    inline void ActorCmsis<T>::join() {
        osThreadJoin(_task);
    }

    template<typename T>
    void ActorCmsis<T>::delay(const uint32_t ticks) {
        osDelay(ticks);
    };


    template<typename T>
    void ActorCmsis<T>::run(void* self) {
        ActorCmsis* instance = reinterpret_cast<ActorCmsis*>(self);
        instance->runBody();
    }
}
#endif

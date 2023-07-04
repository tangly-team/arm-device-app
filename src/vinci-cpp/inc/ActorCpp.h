
#ifndef VINCI_ACTOR_CPP_H
#define VINCI_ACTOR_CPP_H

#include <thread>
#include <string>
#include "../../vinci/inc/Actor.h"
#include "MsgPoolCpp.h"
#include "ConcurrentQueueCpp.h"

namespace vinci {

    template<typename T>
    class ActorCpp : public Actor<T> {
    public:
        /**
         * Constructor of the class.
         * @param name human-readable name of the actor
         */
        explicit ActorCpp(const char* name, MsgPoolCpp<T>& pool);

        virtual ~ActorCpp();

        virtual void join();

    protected:

        virtual void receive(Message<T>* msg);

        virtual Message<T>* message();

        virtual Message<T>* message(const uint32_t ticks);

        virtual void delay(const uint32_t ticks);

    private:
        void run();

        ConcurrentQueueCpp<Message<T>*> _queue;
        std::thread _task;
    };

    template<typename T>
    ActorCpp<T>::ActorCpp(const char* name, MsgPoolCpp<T>& pool) : Actor<T>(name, pool), _task{std::thread(&ActorCpp<T>::run, this)} {}

    template<typename T>
    ActorCpp<T>::~ActorCpp() {}

    template<typename T>
    inline void ActorCpp<T>::join() {
        _task.join();
    }

    template<typename T>
    void ActorCpp<T>::receive(Message<T>* msg) {
        _queue.push(msg);
    }

    template<typename T>
    Message<T>* ActorCpp<T>::message() {
        return _queue.pop();
    }

    template<typename T>
    Message<T>* ActorCpp<T>::message(const uint32_t ticks) {
        std::optional<Message<T>*> msg = _queue.pop(ticks);
        return msg.has_value() ? msg.value() : nullptr;
    }

    template<typename T>
    void ActorCpp<T>::delay(const uint32_t ticks) {
        std::this_thread::sleep_for(std::chrono::microseconds(ticks));
    };

    template<typename T>
    void ActorCpp<T>::run() {
        this->runBody();
    }
}
#endif

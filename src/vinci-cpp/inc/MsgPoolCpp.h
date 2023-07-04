#ifndef VINCI_MSG_POOL_H
#define VINCI_MSG_POOL_H

#include "../../vinci/inc/MessagePool.h"
#include "ConcurrentQueueCpp.h"

namespace vinci {
    using namespace std;

    template<typename T>
    class MsgPoolCpp : public MessagePool<T> {
    public:

        MsgPoolCpp();

        MsgPoolCpp(const MsgPoolCpp<T>&) = default;

        MsgPoolCpp(const MsgPoolCpp<T>&&) = default;

        virtual Message<T>* acquire();

        virtual void release(Message<T>* msg);

        virtual int nrOfFreeMsgs();

    private:
        Message<T> _pool[MessagePool<T>::MSG_POOL_SIZE];
        ConcurrentQueueCpp<Message<T>*> _messages;
    };

    template<typename T>
    MsgPoolCpp<T>::MsgPoolCpp() {
        for (int i = 0; i < MessagePool<T>::MSG_POOL_SIZE; i++) {
            Message<T>* ptr = &_pool[i];
            _messages.push(ptr);
        }
    }

    template<typename T>
    Message<T>* MsgPoolCpp<T>::acquire() {
        optional<Message<T>*> msg = _messages.pop(0);
        return msg.has_value() ? msg.value() : nullptr;
    }

    template<typename T>
    void MsgPoolCpp<T>::release(Message<T>* msg) {
        _messages.push(msg);
    }

    template<typename T>
    int MsgPoolCpp<T>::nrOfFreeMsgs() {
        return _messages.count();
    }
}


#endif

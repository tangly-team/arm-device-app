/*
 * Copyright 2023-2023 Marcel Baumann
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
 * OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
 *
 */

#ifndef VINCI_MSG_POOL_CMSIS_H
#define VINCI_MSG_POOL_CMSIS_H

#include "../../vinci/inc/MessagePool.h"
#include "ConcurrentQueueCmsis.h"

namespace vinci {
    using namespace std;

    template<typename T>
    class MsgPoolCmsis : public MessagePool<T> {
    public:
        MsgPoolCmsis();

        virtual Message<T>* acquire();

        virtual void release(Message<T>* msg);

        virtual int nrOfFreeMsgs();

    private:
        ConcurrentQueueCmsis<Message<T>*> _messages;
        Message<T> _pool[MessagePool<T>::MSG_POOL_SIZE];
    };

    template<typename T>
    MsgPoolCmsis<T>::MsgPoolCmsis() : _messages(MessagePool<T>::MSG_POOL_SIZE) {
        for (int i = 0; i < MessagePool<T>::MSG_POOL_SIZE; i++) {
            Message<T>* ptr = &_pool[i];
            _messages.push(ptr);
        }
    }

    template<typename T>
    Message<T>* MsgPoolCmsis<T>::acquire() {
        optional<Message<T>*> msg = _messages.pop(0);
        return msg.has_value() ? msg.value() : nullptr;
    }

    template<typename T>
    void MsgPoolCmsis<T>::release(Message<T>* msg) {
        _messages.push(msg);
    }

    template<typename T>
    int MsgPoolCmsis<T>::nrOfFreeMsgs() {
        return _messages.count();
    }
} // namespace vinci

#endif

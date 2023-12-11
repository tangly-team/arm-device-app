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

#ifndef SAL_PRESSSURE_SENSOR_H
#define SAL_PRESSSURE_SENSOR_H

#include "../../vinci/inc/MessagePool.h"
#include "../../vinci-cmsis/inc/ActorCmsis.h"
#include "../../vinci-cmsis/inc/MsgPoolCmsis.h"
#include "Messages.h"

namespace sal {
    template<typename T>
    class PressureSensor : public ActorCmsis<T> {
    public:
        PressureSensor(const char* name, int queueSize, osThreadAttr_t& attributes, MsgPoolCmsis<T>& pool);

        void registerListener(Actor<T>* listener);

        virtual bool processMsg(Message<T>* msg);

    private:
        uint32_t _pressure = 0;
        Actor<T>* _listener = nullptr;
    };

    template<typename T>
    PressureSensor<T>::PressureSensor(const char* name, int queueSize, osThreadAttr_t& attributes, MsgPoolCmsis<T>& pool)
            : ActorCmsis<T>(name, queueSize, attributes, pool) {}

    template<typename T>
    void PressureSensor<T>::registerListener(Actor<T>* listener) {
        PressureSensorCmd cmd{REGISTER, listener};
        Messages* msg = MessageBuilder::build(cmd, this->pool());
        this->receive(msg);
    }

    template<typename T>
    bool PressureSensor<T>::processMsg(Message<T>* msg) {
        PressureSensorCmd* cmd = msg->data().pressureSensorCmd();
        if (cmd != nullptr) {
            _listener = cmd->listener;
            this->pool().release(msg);
        } else if (_listener != nullptr) {
            Actor<T>::send(*_listener, msg);
        }
        return true;
    }
}
#endif

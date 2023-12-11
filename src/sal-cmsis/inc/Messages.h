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

#ifndef SAL_MESSAGES_H
#define SAL_MESSAGES_H

#include <variant>
#include "../../vinci/inc/Actor.h"
#include "../../vinci//inc/MessagePool.h"

using namespace vinci;

namespace sal {
    struct Data;

    enum LedCommands {
        START, STOP
    };

    struct LedCmd {
        LedCommands cmd;
        uint32_t rate;
    };

    enum PressureSensorCommands {
        REGISTER
    };

    struct PressureSensorCmd {
        PressureSensorCommands cmd;
        Actor<Data>* listener;
    };

    struct PressureSensorData {
        uint32_t pressure;
    };

    typedef std::variant<LedCmd, PressureSensorCmd, PressureSensorData> Variants;

    struct Data {
        Variants variants;

        LedCmd* ledCmd() {
            return std::get_if<LedCmd>(&variants);
        }

        PressureSensorCmd* pressureSensorCmd() {
            return std::get_if<PressureSensorCmd>(&variants);
        }

        PressureSensorData* pressureSensorData() {
            return std::get_if<PressureSensorData>(&variants);
        }
    };

    class MessageBuilder {
    public:
        static Message<Data>* build(LedCmd& cmd, MessagePool<Data>& pool) {
            Data data = {cmd};
            return build(data, pool);
        }

        static Message<Data>* build(PressureSensorCmd& cmd, MessagePool<Data>& pool) {
            Data data = {cmd};
            return build(data, pool);
        }

        static Message<Data>* build(PressureSensorData& sensorData, MessagePool<Data>& pool) {
            Data data = {sensorData};
            return build(data, pool);
        }

        static Message<Data>* build(Data& data, MessagePool<Data>& pool) {
            Message<Data>* msg = pool.acquire();
            msg->data(data);
            return msg;
        }
    };

    typedef Message<Data> Messages;

}
#endif

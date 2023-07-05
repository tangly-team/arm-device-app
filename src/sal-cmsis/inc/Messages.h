#ifndef SAL_MESSAGES_H
#define SAL_MESSAGES_H

#include <variant>
#include "../../vinci//inc/MessagePool.h"
#include "../../vinci/inc/TimerMgr.h"

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

    typedef std::variant<LedCmd,
            PressureSensorCmd, PressureSensorData,
            TimerCmd<Data>, Timer<Data>> Variants;

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

        TimerCmd<Data>* timerCmd() {
            return std::get_if<TimerCmd<Data>>(&variants);
        }

        Timer<Data>* timer() {
            return std::get_if<Timer<Data>>(&variants);
        };
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

        static Message<Data>* build(TimerCmd<Data>& cmd, MessagePool<Data>& pool) {
            Data data = {cmd};
            return build(data, pool);
        }

        static Message<Data>* build(Timer<Data>& timer, MessagePool<Data>& pool) {
            Data data = {timer};
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

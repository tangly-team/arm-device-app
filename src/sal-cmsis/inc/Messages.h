#ifndef SAL_MESSAGES_H
#define SAL_MESSAGES_H

#include <variant>
#include "../../vinci//inc/MessagePool.h"
#include "../../vinci/inc/TimerMgr.h"

using namespace vinci;

namespace sal {
    enum LedCommands {
        START, STOP
    };

    struct LedCmd {
        LedCommands cmd;
        uint32_t rate;
    };

    struct Data;

    typedef std::variant<LedCmd, TimerCmd<Data>, Timer<Data>> Variants;

    struct Data {
        Variants variants;

        LedCmd* ledCmd() {
            return std::get_if<LedCmd>(&variants);
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
            Message<Data>* msg = pool.acquire();
            msg->data(data);
            return msg;
        }

        static Message<Data>* build(TimerCmd<Data>& cmd, MessagePool<Data>& pool) {
            Data data = {cmd};
            Message<Data>* msg = pool.acquire();
            msg->data(data);
            return msg;
        }

        static Message<Data>* build(Timer<Data>& timer, MessagePool<Data>& pool) {
            Data data = {timer};
            Message<Data>* msg = pool.acquire();
            msg->data(data);
            return msg;
        }
    };

    typedef Message<Data> Messages;
}
#endif

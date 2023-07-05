
#ifndef SAL_ACTOR_TIMER_MGR_CMSIS_H
#define SAL_ACTOR_TIMER_MGR_CMSIS_H

#include <vector>
#include "../../vinci/inc/TimerMgr.h"
#include "../../vinci-cmsis/inc/ActorCmsis.h"
#include "Messages.h"
#include "cmsis_os2.h"

namespace sal {
    class TimerMgrSal : public TimerMgr<Data>, ActorCmsis<Data> {
    public:
        static const int ACTOR_TIMER_MGR_MSG_TYPE = 1;

        TimerMgrSal(const char* name, int queueSize, osThreadAttr_t& attributes, MsgPoolCmsis<Data>& pool);

        virtual void createTimer(Timer<Data>& timer);

        virtual void cancelTimer(Actor<Data>& client, uint32_t timerId);

        virtual uint32_t currentTime();

        virtual Message<Data>* create(Timer<Data>& timer);

    protected:
        virtual bool processMsg(Message<Data>* msg);
    };
    TimerMgrSal::TimerMgrSal(const char* name, int queueSize, osThreadAttr_t& attributes, MsgPoolCmsis<Data>& pool) : ActorCmsis<Data>(name, queueSize, attributes, pool) {}

    void TimerMgrSal::createTimer(Timer<Data>& timer) {
        TimerCmd<Data> cmd{CREATE, timer};
        Messages* msg = MessageBuilder::build(cmd, this->pool());
        this->receive(msg);
    }

    void TimerMgrSal::cancelTimer(Actor<Data>& client, uint32_t timerId) {
        TimerCmd<Data> cmd{CANCEL, Timer<Data>::ofOnce(&client, timerId, 0)};
        Messages* msg = MessageBuilder::build(cmd, this->pool());
        this->receive(msg);
    }

    uint32_t TimerMgrSal::currentTime() {
        return osKernelGetTickCount();
    }

    Message<Data>* TimerMgrSal::create(Timer<Data>& timer) {
        return MessageBuilder::build(timer, pool());
    }

    bool TimerMgrSal::processMsg(Message<Data>* msg) {
        bool continues = true;
        if (msg == nullptr) {
            this->processTimeouts(currentTime());
            waitDelay(waitFor(currentTime()));
        } else {
            continues = this->processCmd(*msg->data().timerCmd(), currentTime());
            this->pool().release(msg);
        }
        return continues;
    }
}

#endif

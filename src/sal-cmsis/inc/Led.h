#ifndef SAL_LED_H
#define SAL_LED_H

#include <cstdint>
#include "../../vinci/inc/MessagePool.h"
#include "../../vinci-cmsis/inc/ActorCmsis.h"
#include "../../vinci-cmsis/inc/MsgPoolCmsis.h"
#include "Messages.h"

using namespace vinci;

namespace sal {
    template<typename T>
    class Led : public ActorCmsis<T> {
    public:
        Led(char* name, int queueSize, osThreadAttr_t& attributes, MsgPoolCmsis <T>& pool);

        void start(uint32_t rateInTicks);

        void stop();

        virtual bool processMsg(Message <T>* msg);

    private:
        void toggle();

        bool _on = false;
    };

    template<typename T>
    Led<T>::Led(char* name, int queueSize, osThreadAttr_t& attributes, MsgPoolCmsis <T>& pool) : ActorCmsis<T>(name, queueSize, attributes) {}

    template<typename T>
    void Led<T>::start(uint32_t rateInTicks) {
        LedCmd cmd{START, rateInTicks};
        Messages* msg = build(cmd, this->pool());
        this->receive(msg);
    }

    template<typename T>
    void Led<T>::stop() {
        LedCmd cmd{STOP, 0};
        Messages* msg = build(cmd, this->pool());
        this->receive(msg);
    }

    template<typename T>
    bool Led<T>::processMsg(Message <T>* msg) {
        if (msg == nullptr) {
            toggle();
        } else {
            LedCmd* cmd = msg->data().ledCmd();
            if (cmd != nullptr) {
                switch (cmd->cmd) {
                    case START:
                        this->waitDelay(cmd->rate);
                        break;
                    case STOP:
                        this->waitDelay(0);
                        break;
                }
            }
        }
    }

    template<typename T>
    void Led<T>::toggle() {
        _on = !_on;
    }
}
#endif

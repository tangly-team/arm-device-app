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

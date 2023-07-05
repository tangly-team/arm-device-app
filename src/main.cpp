#include <iostream>
#include <chrono>
#include <thread>

#include "vinci-cmsis/inc/ActorCmsis.h"
#include "vinci-cmsis/inc/MsgPoolCmsis.h"
#include "vinci/inc/TimerMgr.h"
#include "sal-cmsis/inc/Messages.h"
#include "sal-cmsis/inc/Led.h"
#include "sal-cmsis/inc/PressureSensor.h"
#include "sal-cmsis/inc/TimerMgrSal.h"
#include "vinci-cmsis-simulation/inc/Simulator.h"


using namespace vinci;
using namespace sal;

MsgPoolCmsis<Data> pool;

/**
 * The processor one regulates two leds based on the input of a pressure sensor
 * - If the pressure sensor is between 0 and  50, Led Green is stopped and Led Red blinks every 1000 ticks
 * - If the pressure sensor is between 50 and 100, Led Green blinks every 1000 ticks and Red Led is stopped
 * - If the pressure sensor is above 100, Led Green blinks every 4000 ticks and Read Lead blinks every 2000 ticks
 */
class ProcessorOne : public ActorCmsis<Data> {
public:
    ProcessorOne(const char* name, int queueSize, osThreadAttr_t& threadAttributes, MsgPoolCmsis<Data>& pool, Led<Data>& ledGreen, Led<Data>& ledRed,
                 PressureSensor<Data>& pressureSensorOne);

protected:
    bool processMsg(Message<Data>* msg);

private:
    Led<Data>& _ledGreen;
    Led<Data>& _ledRed;
    PressureSensor<Data>& _pressureSensorOne;
};

ProcessorOne::ProcessorOne(const char* name, int queueSize, osThreadAttr_t& threadAttributes, MsgPoolCmsis<Data>& pool, Led<Data>& ledGreen, Led<Data>& ledRed,
                           PressureSensor<Data>& pressureSensorOne)
        : ActorCmsis<Data>(name, queueSize, threadAttributes, pool), _ledGreen{ledGreen}, _ledRed{ledRed}, _pressureSensorOne{pressureSensorOne} {
    pressureSensorOne.registerListener(this);
}


bool ProcessorOne::processMsg(Message<Data>* msg) {
    bool continues = true;
    Variants variants = msg->data().variants;
    PressureSensorData* data = msg->data().pressureSensorData();
    if (data != nullptr) {
        if ((data->pressure >= 0) && data->pressure < 50) {
            _ledGreen.stop();
            _ledRed.start(1000);
        } else if ((data->pressure >= 50) && data->pressure < 100) {
            _ledGreen.start(1000);
            _ledRed.stop();
        } else {
            _ledGreen.start(4000);
            _ledRed.start(2000);
        }
    }
    pool().release(msg);
    return continues;
};

/**
 * The configuration is two LED (read, green), a pressure sensor, and a processor with a simple algorithm.
 * The state of the hardware is displayed and user input is awaited.
 */
int main(int argc, char** argv) {
    osKernelInitialize();
    osKernelStart();
    osThreadAttr_t attributes;

    PressureSensor<Data> pressureSensorOne("pressureOne", 10, attributes, pool);

    //This is equivalent to configure an ISR routine to send message to an actor if an interrupt is received.
    Simulator<Data> simulator{pool, pressureSensorOne};

    // Declares output components no requiring ISR routines
    Led<Data> ledGreen(LED_GREEN, 10, attributes, pool);
    Led<Data> ledRed(LED_RED, 10, attributes, pool);

    // Connects higher order processes to inputs and outputs
    ProcessorOne processorOne("processorOne", 10, attributes, pool, ledGreen, ledRed, pressureSensorOne);

    // Timer manager for the application
    TimerMgrSal timerMgr("timerMgr", 10, attributes, pool);

    // Initial setup of the components
    ledGreen.start(100);
    ledRed.start(400);

    // Run the system with a primitive hardware simulator for two LEDs and one pressure sensor
    for (;;) {
        simulator.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

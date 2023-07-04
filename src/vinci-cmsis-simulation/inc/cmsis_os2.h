#ifndef CMSIS_OS2_H
#define CMSIS_OS2_H

#include <thread>
#include <mutex>
#include <sstream>
#include <iostream>
#include <chrono>
#include "../../vinci-cpp/inc/ConcurrentQueueCpp.h"

// must be pointers because NULL can be returned

typedef std::shared_ptr<vinci::ConcurrentQueueCpp<uint8_t *>> osMessageQueueId_t;
typedef std::shared_ptr<std::thread> osThreadId_t;
typedef std::shared_ptr<std::mutex> osMutexId_t;
typedef void(*osThreadFunc_t)(void *argument);

#define osWaitForever   0xFFFFFFFFU

struct osMessageQueueAttr_t {
    const char *name;
    uint32_t attr_bits;
    void *cb_mem;
    uint32_t cb_size;
};

enum osThreadState_t {
    osThreadInactive = 0,
    osThreadReady = 1,
    osThreadRunning = 2,
    osThreadBlocked = 3,
    osThreadTerminated = 4,
    osThreadError = -1,
    osThreadReserved = 0x7FFFFFFF
};

enum osPriority_t {
    osPriorityNone = 0,
    osPriorityIdle = 1,
    osPriorityLow = 8,
    osPriorityLow1 = 8 + 1,
    osPriorityLow2 = 8 + 2,
    osPriorityLow3 = 8 + 3,
    osPriorityLow4 = 8 + 4,
    osPriorityLow5 = 8 + 5,
    osPriorityLow6 = 8 + 6,
    osPriorityLow7 = 8 + 7,
    osPriorityBelowNormal = 16,
    osPriorityBelowNormal1 = 16 + 1,
    osPriorityBelowNormal2 = 16 + 2,
    osPriorityBelowNormal3 = 16 + 3,
    osPriorityBelowNormal4 = 16 + 4,
    osPriorityBelowNormal5 = 16 + 5,
    osPriorityBelowNormal6 = 16 + 6,
    osPriorityBelowNormal7 = 16 + 7,
    osPriorityNormal = 24,
    osPriorityNormal1 = 24 + 1,
    osPriorityNormal2 = 24 + 2,
    osPriorityNormal3 = 24 + 3,
    osPriorityNormal4 = 24 + 4,
    osPriorityNormal5 = 24 + 5,
    osPriorityNormal6 = 24 + 6,
    osPriorityNormal7 = 24 + 7,
    osPriorityAboveNormal = 32,
    osPriorityAboveNormal1 = 32 + 1,
    osPriorityAboveNormal2 = 32 + 2,
    osPriorityAboveNormal3 = 32 + 3,
    osPriorityAboveNormal4 = 32 + 4,
    osPriorityAboveNormal5 = 32 + 5,
    osPriorityAboveNormal6 = 32 + 6,
    osPriorityAboveNormal7 = 32 + 7,
    osPriorityHigh = 40,
    osPriorityHigh1 = 40 + 1,
    osPriorityHigh2 = 40 + 2,
    osPriorityHigh3 = 40 + 3,
    osPriorityHigh4 = 40 + 4,
    osPriorityHigh5 = 40 + 5,
    osPriorityHigh6 = 40 + 6,
    osPriorityHigh7 = 40 + 7,
    osPriorityRealtime = 48,
    osPriorityRealtime1 = 48 + 1,
    osPriorityRealtime2 = 48 + 2,
    osPriorityRealtime3 = 48 + 3,
    osPriorityRealtime4 = 48 + 4,
    osPriorityRealtime5 = 48 + 5,
    osPriorityRealtime6 = 48 + 6,
    osPriorityRealtime7 = 48 + 7,
    osPriorityISR = 56,
    osPriorityError = -1,
    osPriorityReserved = 0x7FFFFFFF
};

enum osStatus_t {
    osOK,
    osError,
    osErrorTimeout,
    osErrorResource,
    osErrorParameter
};

typedef uint32_t TZ_ModuleId_t;

static const int THREAD_NAME_SIZE = 32;


struct osThreadAttr_t {
    char name[THREAD_NAME_SIZE];
    uint32_t attr_bits;
    void *cb_mem;
    uint32_t cb_size;
    void *stack_mem;
    uint32_t stack_size;
    osPriority_t priority;
    TZ_ModuleId_t tz_module;
    uint32_t reserved;
};

struct osMutexAttr_t {
    const char *name;
    uint32_t attr_bits;
    void *cb_mem;
    uint32_t cb_size;
    void *stack_mem;
};

struct Kernel {
    uint64_t timeFromStart;
    uint32_t tickFrequency;
};

static Kernel kernel;

/**
 * Hardcoded with tick duration 1 ms, tick frequency 1000 Hz, start of counting set with kernel initialize.
 * @return
 */
osStatus_t osKernelInitialize(void) {
    kernel.timeFromStart = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    kernel.tickFrequency = 1000;
    return osOK;
}

osStatus_t osKernelStart(void) {
    return osOK;
}

uint32_t osKernelGetTickCount(void) {
    uint64_t timeNow = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return (timeNow - kernel.timeFromStart);
}

uint32_t osKernelGetTickFreq(void) {
    return kernel.tickFrequency;
}

osThreadId_t osThreadNew(osThreadFunc_t func,
                         void *argument,
                         const osThreadAttr_t *attr) {
    return std::shared_ptr<std::thread>(new std::thread(func, argument));
}

const char *osThreadGetName(osThreadId_t thread_id) {
    std::stringstream buffer;
    buffer << thread_id->get_id();
    return buffer.str().c_str();
}

osStatus_t osThreadJoin(osThreadId_t thread_id) {
    thread_id->join();
    return osOK;
}

osStatus_t osDelay(uint32_t milliseconds) {
    std::this_thread::sleep_for(std::chrono::microseconds(milliseconds));
    return osOK;
}

static uint32_t message_size = 0;

osMessageQueueId_t osMessageQueueNew(uint32_t msg_count,
                                     uint32_t msg_size,
                                     const osMessageQueueAttr_t *attr) {
    message_size = msg_size;
    return osMessageQueueId_t(new vinci::ConcurrentQueueCpp<uint8_t *>());
}

uint32_t osMessageQueueGetMsgSize(osMessageQueueId_t mq_id) {
    return mq_id.get()->msgSize();
}

osStatus_t osMessageQueuePut(osMessageQueueId_t mq_id,
                             const void *msg_ptr,
                             uint8_t msg_prio,
                             uint32_t timeout) {
    uint8_t *data = new uint8_t[message_size];
    memcpy(data, msg_ptr, message_size);
    mq_id.get()->push(data);
    return osOK;
}

osStatus_t osMessageQueueGet(osMessageQueueId_t mq_id,
                             void *msg_ptr,
                             uint8_t *msg_prio,
                             uint32_t timeout) {
    uint8_t *data = (uint8_t *) mq_id.get()->pop();
    memcpy(msg_ptr, data, message_size);
    delete[] data;
    msg_prio = nullptr;
    return osOK;
}

osStatus_t osMessageQueueReset(osMessageQueueId_t mq_id) {
    mq_id.get()->reset();
    return osOK;
}

uint32_t osMessageQueueGetCount(osMessageQueueId_t mq_id) {
    return mq_id.get()->count();
}

osMutexId_t osMutexNew(const osMutexAttr_t *attr) {
    return std::shared_ptr<std::mutex>();
}

osStatus_t osMutexAcquire(osMutexId_t mutex_id, uint32_t timeout) {
    mutex_id->lock();
    return osOK;
}

osStatus_t osMutexRelease(osMutexId_t mutex_id) {
    mutex_id->unlock();
    return osOK;
}

#endif

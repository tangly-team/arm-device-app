#ifndef VINCI_CMSIS_CONCURRENT_QUEUE_H
#define VINCI_CMSIS_CONCURRENT_QUEUE_H

#include "../../vinci/inc/ConcurrentQueue.h"
#include "cmsis_os2.h"

namespace vinci {

    template<typename T>
    class ConcurrentQueueCmsis : public ConcurrentQueue<T> {
    public:
        ConcurrentQueueCmsis(int queueSize);

        ConcurrentQueueCmsis(ConcurrentQueueCmsis<T>&&) = default;

        virtual ~ConcurrentQueueCmsis() {}

        virtual void push(T& item);

        virtual T pop();

        virtual std::optional<T> pop(const uint32_t ticks);

        virtual void reset();

        virtual int count() const;

        int msgSize() const;

    private:
        osMessageQueueId_t _cmsisQueue;
    };

    template<typename T>
    ConcurrentQueueCmsis<T>::ConcurrentQueueCmsis(int queueSize) {
        _cmsisQueue = osMessageQueueNew(queueSize, sizeof(T), nullptr);
    }

    template<typename T>
    void ConcurrentQueueCmsis<T>::push(T& item) {
        osMessageQueuePut(_cmsisQueue, &item, 0, osWaitForever);
    }

    template<typename T>
    T ConcurrentQueueCmsis<T>::pop() {
        char data[sizeof(T)];
        uint8_t priority;
        osMessageQueueGet(_cmsisQueue, &data, &priority, osWaitForever);
        return *(reinterpret_cast<T*>(data));
    }

    template<typename T>
    std::optional<T> ConcurrentQueueCmsis<T>::pop(const uint32_t ticks) {
        char data[sizeof(T)];
        uint8_t priority;
        osStatus_t status = osMessageQueueGet(_cmsisQueue, &data, &priority, ticks);
        return (status == osErrorTimeout) ? std::nullopt : std::optional<T>{*(reinterpret_cast<T*>(data))};
    }

    template<typename T>
    void ConcurrentQueueCmsis<T>::reset() {
        osMessageQueueReset(_cmsisQueue);
    }

    template<typename T>
    int ConcurrentQueueCmsis<T>::count() const {
        return osMessageQueueGetCount(_cmsisQueue);
    }

    template<typename T>
    int ConcurrentQueueCmsis<T>::msgSize() const {
        return osMessageQueueGetMsgSize(_cmsisQueue);
    }
}
#endif

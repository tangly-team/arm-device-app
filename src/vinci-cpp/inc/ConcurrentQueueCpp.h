#ifndef VINCI_CONCURRENT_QUEUE_CPP_H
#define VINCI_CONCURRENT_QUEUE_CPP_H

#include <queue>
#include <mutex>
#include "../../vinci/inc/ConcurrentQueue.h"

namespace vinci {

    template<typename T>
    class ConcurrentQueueCpp : public ConcurrentQueue<T> {
    public:
        ConcurrentQueueCpp() = default;

        ConcurrentQueueCpp(ConcurrentQueueCpp<T>&&) = default;

        virtual ~ConcurrentQueueCpp() {};

        virtual void push(T& item);

        virtual T pop();

        virtual std::optional<T> pop(const uint32_t ticks);

        virtual void reset();

        virtual int count() const;

        virtual int msgSize() const;

    private:
        std::queue<T> _queue;
        std::mutex _mutex;
        std::condition_variable _condition;
    };

    template<typename T>
    void ConcurrentQueueCpp<T>::push(T& item) {
        std::unique_lock<std::mutex> lock(_mutex);
        _queue.push(item);
        _condition.notify_one();
    }

    template<typename T>
    T ConcurrentQueueCpp<T>::pop() {
        std::unique_lock<std::mutex> lock(_mutex);
        _condition.wait(lock, [this]() { return !_queue.empty(); });
        T item = _queue.front();
        _queue.pop();
        return item;
    }

    template<typename T>
    std::optional<T> ConcurrentQueueCpp<T>::pop(const uint32_t ticks) {
        std::unique_lock<std::mutex> lock(_mutex);
        _condition.wait_for(lock, std::chrono::milliseconds(ticks), [this]() { return !_queue.empty(); });
        if (_queue.empty()) {
            return std::nullopt;
        } else {
            T item = _queue.front();
            _queue.pop();
            return std::optional<T>{item};
        }
    }

    template<typename T>
    void ConcurrentQueueCpp<T>::reset() {
        std::unique_lock<std::mutex> lock(_mutex);
        std::queue<T>().swap(_queue);
    }

    template<typename T>
    int ConcurrentQueueCpp<T>::count() const {
        return _queue.size();
    }

    template<typename T>
    int ConcurrentQueueCpp<T>::msgSize() const {
        return sizeof(T);
    }
}
#endif

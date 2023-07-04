
#ifndef VINCI_CONCURRENT_QUEUE_H
#define VINCI_CONCURRENT_QUEUE_H

#include <optional>

namespace vinci {

    /**
     * Define the abstraction of a concurrent queue. A concurrent queue can be accessed by multiple threads.
     * @tparam T type of the items stored in the queue
     */
    template<typename T>
    class ConcurrentQueue {
    public:
        /**
         * Push an item into the concurrent _queue.
         * @param item item to push
         */
        virtual void push(T& item) = 0;

        /**
         * Pop an item from the concurrent _queue.
         * @return the item removed from the _queue
         */
        virtual T pop() = 0;

        /**
         * Pop an item from the concurrent _queue.
         * @param ticks wait delay before returning
         * @return the item removed from the _queue
         */
        virtual std::optional<T> pop(const uint32_t ticks) = 0;

        /**
         * Reset the queue and remove all items.
         */
        virtual void reset() = 0;

        /**
         * Return the size of a message stored in the queue.
         * @return size of the message
         */
        virtual int msgSize() const = 0;

        /**
         * Return the number of messages in the queue.
         * @return number of message in the queue
         */
        virtual int count() const = 0;
    };
}
#endif

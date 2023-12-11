/*
 * Copyright 2023-2023 Marcel Baumann
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
 * OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
 *
 */

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
} // namespace vinci
#endif

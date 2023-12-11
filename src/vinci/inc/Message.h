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

#ifndef VINCI_MESSAGE_H
#define VINCI_MESSAGE_H

namespace vinci {
    /**
     * Define the abstraction of a message sent between actors.
     * @tparam T type of the payload of the message
     */
    template<typename T>
    class Message {
    public:
        Message() = default;

        Message(const Message<T>&) = default;

        Message(const Message<T>&&) = default;

        ~Message() = default;

        Message<T>& operator=(const Message<T>&) = default;

        Message<T>& operator=(const Message<T>&&) = default;

        Message(int kind, T data) : _kind{kind}, _data(data) {}

        inline void kind(int kind) { _kind = kind; }

        inline int kind() const noexcept { return _kind; };

        inline T data() const noexcept { return _data; }

        inline void data(T data) { _data = data; }
    private:
        int _kind;
        T _data;
    };
} // namespace vinci
#endif

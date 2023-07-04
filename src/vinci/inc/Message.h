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
}
#endif

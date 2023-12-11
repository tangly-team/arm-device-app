#ifndef VINCI_VALVE_H
#define VINCI_VALVE_H

class Valve {
public:
    void open();

    void close();

    bool isOpen() const;

private:
    Valve(const Valve&) = delete;

    const Valve& operator=(const Valve&) = delete;

    bool _opened = false;
};

inline void Valve::open() {
    _opened = true;
}

inline void Valve::close() {
    _opened = false;
}

inline bool Valve::isOpen() const {
    return _opened;
}

#endif

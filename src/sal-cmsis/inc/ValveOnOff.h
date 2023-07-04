
#ifndef VINCI_SENSOR_H
#define VINCI_SENSOR_H

class ValveOnOff {
public:
    void open();

    void close();

    bool isOpen() const;

private:
    bool _opened = false;
};

void ValveOnOff::open() {}

void ValveOnOff::close() {}

bool ValveOnOff::isOpen() const {
    return _opened;
}

#endif

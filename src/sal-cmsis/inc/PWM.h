#ifndef VINCI_PWM_H
#define VINCI_PWM_H

#include <cstdint>

using namespace std;

class PWM {
public:
    void dutyCycle(uint32_t);

    uint32_t dutyCycle() const;

private:
    PWM(const PWM&) = delete;

    const PWM& operator=(const PWM&) = delete;

    uint32_t _dutyCycle = 0;
};

inline uint32_t PWM::dutyCycle() const {
    return _dutyCycle;
}

inline void PWM::dutyCycle(uint32_t dutyCycle) {
    _dutyCycle = dutyCycle;
}

#endif

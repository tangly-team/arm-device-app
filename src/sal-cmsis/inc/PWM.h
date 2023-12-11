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

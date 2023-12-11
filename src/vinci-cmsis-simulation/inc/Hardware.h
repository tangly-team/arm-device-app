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

#ifndef VINCI_HARDWARE_H
#define VINCI_HARDWARE_H

char LED_GREEN[] = "green";
char LED_RED[] = "red";

struct Hardware {
    bool _ledGreen;
    bool _ledRed;
    uint32_t _pressureSensorOne;

    void setLed(const char* name, bool on);
};

void Hardware::setLed(const char* name, bool on) {
    if (strcmp(LED_GREEN, name) == 0) {
        _ledGreen = on;
    } else if (strcmp(LED_RED, name) == 0) {
        _ledRed = on;
    }
}

Hardware hardware;

#endif

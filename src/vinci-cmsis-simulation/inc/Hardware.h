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

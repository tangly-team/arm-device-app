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

#ifndef VINCI_SIMULATOR_H
#define VINCI_SIMULATOR_H

#include <future>

#include "../../vinci/inc/MessagePool.h"
#include "../../sal-cmsis/inc/Messages.h"
#include "../../sal-cmsis/inc/PressureSensor.h"

#include "Hardware.h"

using namespace sal;

char PRESSURE_SENSOR_ONE[] = "PressureOne";

template<typename T>
class Simulator {
 public:
  Simulator(MessagePool<T> &pool, PressureSensor<T> &pressureSensorOne);

  void setPressureSensor(uint32_t pressure);

  void display();

 private:
  MessagePool<T> &_pool;
  PressureSensor<T> &_pressureSensorOne;
};

template<typename T>
Simulator<T>::Simulator(MessagePool<T> &pool, PressureSensor<T> &pressureSensorOne) : _pool{pool}, _pressureSensorOne{pressureSensorOne} {}

template<typename T>
void Simulator<T>::display() {
  // printf("\33[2K\r");
  cout << "LED-Green[" << hardware._ledGreen << "] "
       << "LED-Red[" << hardware._ledRed << "] "
       << "PR[" << hardware._pressureSensorOne << "] "
       << "-> new value?"
       << flush;
  cin >> hardware._pressureSensorOne;
  if (hardware._pressureSensorOne > 0) {
    setPressureSensor(hardware._pressureSensorOne);
  }
}

template<typename T>
void Simulator<T>::setPressureSensor(uint32_t pressure) {
  PressureSensorData sensorData{pressure};
  Message<Data> *msg = MessageBuilder::build(sensorData, _pool);
  Actor<Data>::send(_pressureSensorOne, msg);
}

#endif

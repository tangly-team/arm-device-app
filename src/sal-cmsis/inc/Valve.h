
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

#ifndef VINCI_VALVE_H
#define VINCI_VALVE_H

class Valve {
 public:
  void open();

  void close();

  bool isOpen() const;

 private:
  Valve(const Valve &) = delete;

  const Valve &operator=(const Valve &) = delete;

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

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

#include "../vinci/inc/Message.h"
#include "../vinci-cpp/inc/MsgPoolCpp.h"
#include "../vinci-cmsis/inc/MsgPoolCmsis.h"
#include "../sal-cmsis/inc/Messages.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../doctest/doctest.h"

using sal::Data;
using sal::LedCmd;
using sal::Messages;
using sal::START;
using sal::STOP;

namespace vinci::tests {
const int MSG_POOL_SIZE = MessagePool<Data>::MSG_POOL_SIZE;

TEST_SUITE("Message Pool") {
TEST_CASE("Message Pool c++") {
  MsgPoolCpp<Data> pool;
  CHECK_EQ(pool.nrOfFreeMsgs(), MSG_POOL_SIZE);
  Messages *msg = pool.acquire();
  CHECK_EQ(pool.nrOfFreeMsgs(), MSG_POOL_SIZE - 1);
  pool.release(msg);
  CHECK_EQ(pool.nrOfFreeMsgs(), MSG_POOL_SIZE);
}

TEST_CASE("Message Pool cmsis") {
  MsgPoolCmsis<Data> pool;
  CHECK(pool.nrOfFreeMsgs() == MSG_POOL_SIZE);
  Messages *msg = pool.acquire();
  CHECK_EQ(pool.nrOfFreeMsgs(), MSG_POOL_SIZE - 1);
  pool.release(msg);
  CHECK_EQ(pool.nrOfFreeMsgs(), MSG_POOL_SIZE);
}
}

TEST_SUITE("Concurrent Queue") {
TEST_CASE("Concurrent Queue CPP") {
  ConcurrentQueueCpp<Data> queue;
  LedCmd cmd = {START, 42};
  Data data{cmd};
  CHECK_EQ(queue.count(), 0);
  queue.push(data);
  CHECK_EQ(queue.count(), 1);
  data = queue.pop();
  CHECK_EQ(data.ledCmd()->rate, 42);
  CHECK_EQ(queue.count(), 0);
  queue.push(data);
  queue.reset();
  CHECK_EQ(queue.count(), 0);
}

TEST_CASE("Concurrent Queue cmsis") {
  ConcurrentQueueCmsis<Data> queue(10);
  LedCmd cmd = {START, 42};
  Data data{cmd};
  CHECK_EQ(queue.count(), 0);
  queue.push(data);
  CHECK_EQ(queue.count(), 1);
  data = queue.pop();
  CHECK_EQ(data.ledCmd()->rate, 42);
  CHECK_EQ(queue.count(), 0);
  queue.push(data);
  queue.reset();
  CHECK_EQ(queue.count(), 0);
}
}
}  // namespace vinci::tests

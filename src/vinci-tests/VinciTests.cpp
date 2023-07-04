#include "../vinci/inc/Message.h"
#include "../vinci/inc/TimerMgr.h"
#include "../vinci-cpp/inc/MsgPoolCpp.h"
#include "../vinci-cmsis/inc/MsgPoolCmsis.h"
#include "../sal-cmsis/inc/Messages.h"
#include "../sal-cmsis/inc/TimerMgrSal.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../doctest/doctest.h"

using namespace sal;

namespace vinci::tests {
    const int MSG_POOL_SIZE = MessagePool<Data>::MSG_POOL_SIZE;

    TEST_SUITE("Message Pool") {
        TEST_CASE("Message Pool c++") {
            MsgPoolCpp<Data> testMsgPool;
            CHECK(testMsgPool.nrOfFreeMsgs() == MSG_POOL_SIZE);
            Messages* msg = testMsgPool.acquire();
            CHECK(testMsgPool.nrOfFreeMsgs() == MSG_POOL_SIZE - 1);
            testMsgPool.release(msg);
            CHECK(testMsgPool.nrOfFreeMsgs() == MSG_POOL_SIZE);
        }

        TEST_CASE("Message Pool cmsis") {
            MsgPoolCmsis<Data> testMsgPool;
            CHECK(testMsgPool.nrOfFreeMsgs() == MSG_POOL_SIZE);
            Messages* msg = testMsgPool.acquire();
            CHECK(testMsgPool.nrOfFreeMsgs() == MSG_POOL_SIZE - 1);
            testMsgPool.release(msg);
            CHECK(testMsgPool.nrOfFreeMsgs() == MSG_POOL_SIZE);
        }
    }

    TEST_SUITE("Concurrent Queue") {
        TEST_CASE("Concurrent Queue CPP") {
            ConcurrentQueueCpp<Data> testQueue;
            LedCmd cmd = {START, 42};
            Data data{cmd};
            CHECK(testQueue.count() == 0);
            testQueue.push(data);
            CHECK(testQueue.count() == 1);
            data = testQueue.pop();
            CHECK(data.ledCmd()->rate == 42);
            CHECK(testQueue.count() == 0);
            testQueue.push(data);
            testQueue.reset();
            CHECK(testQueue.count() == 0);
        }

        TEST_CASE("Concurrent Queue cmsis") {
            ConcurrentQueueCmsis<Data> testQueue(10);
            LedCmd cmd = {START, 42};
            Data data{cmd};
            CHECK(testQueue.count() == 0);
            testQueue.push(data);
            CHECK(testQueue.count() == 1);
            data = testQueue.pop();
            CHECK(data.ledCmd()->rate == 42);
            CHECK(testQueue.count() == 0);
            testQueue.push(data);
            testQueue.reset();
            CHECK(testQueue.count() == 0);
        }
    }

    TEST_CASE("Timer Manager sal") {
        osThreadAttr_t attributes;
        MsgPoolCmsis<Data> msgPool;
        char name[] = "TestTimerManagerCmsis";
        sal::TimerMgrSal timerMgr{name, 10, attributes, msgPool};
    }
}

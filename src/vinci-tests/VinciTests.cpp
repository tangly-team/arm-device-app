#include "../vinci/inc/Message.h"
#include "../vinci-cpp/inc/MsgPoolCpp.h"
#include "../vinci-cmsis/inc/MsgPoolCmsis.h"
#include "../sal-cmsis/inc/Messages.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../doctest/doctest.h"

using namespace sal;

namespace vinci::tests {
    const int MSG_POOL_SIZE = MessagePool<Data>::MSG_POOL_SIZE;

    TEST_SUITE("Message Pool") {
        TEST_CASE("Message Pool c++") {
            MsgPoolCpp<Data> pool;
            CHECK(pool.nrOfFreeMsgs() == MSG_POOL_SIZE);
            Messages* msg = pool.acquire();
            CHECK(pool.nrOfFreeMsgs() == MSG_POOL_SIZE - 1);
            pool.release(msg);
            CHECK(pool.nrOfFreeMsgs() == MSG_POOL_SIZE);
        }

        TEST_CASE("Message Pool cmsis") {
            MsgPoolCmsis<Data> pool;
            CHECK(pool.nrOfFreeMsgs() == MSG_POOL_SIZE);
            Messages* msg = pool.acquire();
            CHECK(pool.nrOfFreeMsgs() == MSG_POOL_SIZE - 1);
            pool.release(msg);
            CHECK(pool.nrOfFreeMsgs() == MSG_POOL_SIZE);
        }
    }

    TEST_SUITE("Concurrent Queue") {
        TEST_CASE("Concurrent Queue CPP") {
            ConcurrentQueueCpp<Data> queue;
            LedCmd cmd = {START, 42};
            Data data{cmd};
            CHECK(queue.count() == 0);
            queue.push(data);
            CHECK(queue.count() == 1);
            data = queue.pop();
            CHECK(data.ledCmd()->rate == 42);
            CHECK(queue.count() == 0);
            queue.push(data);
            queue.reset();
            CHECK(queue.count() == 0);
        }

        TEST_CASE("Concurrent Queue cmsis") {
            ConcurrentQueueCmsis<Data> queue(10);
            LedCmd cmd = {START, 42};
            Data data{cmd};
            CHECK(queue.count() == 0);
            queue.push(data);
            CHECK(queue.count() == 1);
            data = queue.pop();
            CHECK(data.ledCmd()->rate == 42);
            CHECK(queue.count() == 0);
            queue.push(data);
            queue.reset();
            CHECK(queue.count() == 0);
        }
    }

    // TODO: Find out how to run threads in doctest

//    TEST_SUITE("Timer Manager") {
//        TEST_CASE("Timer Manager sal") {
//            osThreadAttr_t attributes;
//            MsgPoolCmsis<Data> pool;
//            char name[] = "TestTimerManagerSal";
//            sal::TimerMgrSal timerMgr{name, 10, attributes, pool};
//            CHECK(timerMgr.currentTime() != 0);
//        }
//    }
}

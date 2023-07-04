#ifndef VINCI_TIMER_MGR_H
#define VINCI_TIMER_MGR_H

#include <vector>
#include <algorithm>
#include "Actor.h"
#include "Message.h"
#include "MessagePool.h"

namespace vinci {

    using namespace std;

    enum TimerCommands {
        CREATE, CANCEL, ABORT
    };

    template<typename T>
    struct Timer {
    public:
        static Timer<T> ofOnceAbsolute(Actor<T>* client, uint32_t timerId, uint32_t alarmTime) {
            return Timer<T>{client, timerId, alarmTime, false, 0};
        }

        static Timer<T> ofOnce(Actor<T>* client, uint32_t timerId, uint32_t duration) {
            return Timer<T>{client, timerId, 0, false, duration};
        }

        static Timer<T> ofRecurringAbsolute(Actor<T>* client, uint32_t timerId, uint32_t absolute, uint32_t duration) {
            return Timer<T>{client, timerId, absolute, true, duration};
        }

        static Timer<T> ofRecurring(Actor<T>* client, uint32_t timerId, uint32_t duration) {
            return Timer<T>{client, timerId, 0, true, duration};
        }

        static bool compareInterval(const Timer& t1, const Timer& t2) {
            return (t1.alarmTimeInTicks() < t2.alarmTimeInTicks());
        }

        inline Actor<T>* client() const { return _client; }

        inline uint32_t id() const { return _id; }

        inline uint32_t alarmTimeInTicks() const { return _alarmTimeInTicks; }

        inline bool recurring() const { return _recurring; }

        inline uint32_t delayInTicks() const { return _delayInTicks; }

        Actor<T>* _client;
        uint32_t _id;
        uint32_t _alarmTimeInTicks;
        bool _recurring;
        uint32_t _delayInTicks;
    };

    template<typename T>
    struct TimerCmd {
    public:
        TimerCommands cmd;
        Timer<T> timer;
    };

    template<typename T>
    class TimerMgr {
    public:
        virtual void createTimer(Timer<T>& timer) = 0;

        virtual void cancelTimer(Actor<T>& client, uint32_t timerId) = 0;

        virtual uint32_t currentTime() = 0;

        virtual Message<T>* create(Timer<T>& timer) = 0;

    protected:
        bool processCmd(TimerCmd<T>& cmd, uint32_t currentTimeInTicks);

        void processTimeouts(const uint32_t currentTimeInTicks);

        uint32_t waitFor(const uint32_t currentTimeInTicks) const;

    private:
        void scheduleNextOccurrence(Timer<T>& timer, const uint32_t currentTimeInTicks);

        void removeTimer(Timer<T>& timer);

        void addTimer(Timer<T>& timer);

        // TODO replace with std::array to avoid dynamic memory allocations
        vector<Timer<T>> _timers;
    };

    template<typename T>
    bool TimerMgr<T>::processCmd(TimerCmd<T>& cmd, uint32_t currentTimeInTicks) {
        bool continues = true;
        switch (cmd.cmd) {
            case CREATE:
                scheduleNextOccurrence(cmd.timer, currentTimeInTicks);
                break;
            case CANCEL:
                removeTimer(cmd.timer);
                break;
            case ABORT:
                continues = false;
                break;
        }
        return continues;
    }

    template<typename T>
    void TimerMgr<T>::processTimeouts(const uint32_t currentTimeInTicks) {
        while (!_timers.empty() && (_timers[0].alarmTimeInTicks() <= currentTimeInTicks)) {
            Timer<T>& timer = _timers[0];
            Actor<T>::send(*timer.client(), create(timer));
            removeTimer(timer);
            if (timer.recurring()) {
                scheduleNextOccurrence(timer, currentTimeInTicks);
            }
        }
    }

    template<typename T>
    uint32_t TimerMgr<T>::waitFor(uint32_t currentTimeInTicks) const {
        return _timers.empty() ? 0 : std::max(_timers[0].alarmTimeInTicks() - currentTimeInTicks, (uint32_t) 1);
    }

    template<typename T>
    void TimerMgr<T>::scheduleNextOccurrence(Timer<T>& timer, uint32_t currentTimeInTicks) {
        timer._alarmTimeInTicks = (timer.alarmTimeInTicks() == 0) ? currentTimeInTicks + timer.delayInTicks() : timer.alarmTimeInTicks();
        addTimer(timer);
    }

    template<typename T>
    void TimerMgr<T>::removeTimer(Timer<T>& timer) {
        std::remove_if(_timers.begin(), _timers.end(), [&timer](const Timer<T>& o) { return (timer.client() == o.client()) && (timer.id() == o.id()); });
    }

    template<typename T>
    void TimerMgr<T>::addTimer(Timer<T>& timer) {
        _timers.push_back(timer);
        std::sort(_timers.begin(), _timers.end(),Timer<T>::compareInterval);
    }
}
#endif

#pragma once

#include "Service.h"

#include <server/asio/timer.h>

namespace CSharpServer {

    ref class Timer;

    class TimerEx : public CppServer::Asio::Timer
    {
    public:
        using CppServer::Asio::Timer::Timer;

        gcroot<CSharpServer::Timer^> root;

        void onTimer(bool canceled) override;
        void onError(int error, const std::string& category, const std::string& message) override;
    };

    //! Timer
    /*!
        Timer is used to plan and perform delayed operation.
    */
    public ref class Timer
    {
    public:
        //! Initialize timer with a given service
        /*!
            \param service - Service
        */
        Timer(Service^ service);
        //! Initialize timer with a given service and absolute expiry time
        /*!
            \param service - Service
            \param time - Absolute time
        */
        Timer(Service^ service, DateTime time);
        //! Initialize timer with a given service and expiry time relative to now
        /*!
            \param service - Service
            \param timespan - Relative timespan
        */
        Timer(Service^ service, TimeSpan timespan);
        ~Timer() { this->!Timer(); }

        //! Get the service
        property Service^ Service { CSharpServer::Service^ get() { return _service; } }

        //! Get the timer's expiry time as an absolute time
        property DateTime ExpireTime { DateTime get() { return DateTime(_timer->get()->expire_time().utcstamp().nanoseconds() / 100, DateTimeKind::Utc); } }
        //! Get the timer's expiry time relative to now
        property TimeSpan ExpireTimeSpan { TimeSpan get() { return TimeSpan(_timer->get()->expire_timespan().nanoseconds() / 100); } }

        //! Setup the timer with absolute expiry time
        /*!
            \param time - Absolute time
            \return 'true' if the timer was successfully setup, 'false' if the timer failed to setup
        */
        bool Setup(DateTime time) { return _timer->get()->Setup(CppCommon::UtcTime(time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second, time.Millisecond)); }
        //! Setup the timer with expiry time relative to now
        /*!
            \param timespan - Relative timespan
            \return 'true' if the timer was successfully setup, 'false' if the timer failed to setup
        */
        bool Setup(TimeSpan timespan) { return _timer->get()->Setup(CppCommon::Timespan(timespan.Ticks * 100)); }

        //! Wait for the timer (asynchronous)
        /*!
            \return 'true' if the timer was successfully expired, 'false' if any error occurred
        */
        bool WaitAsync() { return _timer->get()->WaitAsync(); }
        //! Wait for the timer (synchronous)
        /*!
            \return 'true' if the timer was successfully expired, 'false' if any error occurred
        */
        bool WaitSync() { return _timer->get()->WaitSync(); }

        //! Cancel any wait operation on the timer
        /*!
            \return 'true' if the timer was successfully canceled, 'false' if any error occurred
        */
        bool Cancel() { return _timer->get()->Cancel(); }

    protected:
        //! Handle timer notification
        virtual void OnTimer(bool canceled) {}

        //! Handle error notification
        /*!
            \param error - Error code
            \param category - Error category
            \param message - Error message
        */
        virtual void OnError(int error, String^ category, String^ message) {}

    internal:
        void InternalOnTimer(bool canceled) { OnTimer(canceled); }
        void InternalOnError(int error, String^ category, String^ message) { OnError(error, category, message); }

    protected:
        !Timer() { _timer.Release(); }

    private:
        CSharpServer::Service^ _service;
        Embedded<std::shared_ptr<TimerEx>> _timer;
    };

}

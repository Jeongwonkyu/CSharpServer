#include "stdafx.h"

#include "Service.h"

namespace CSharpServer {

    void ServiceEx::onThreadInitialize()
    {
        root->InternalOnThreadInitialize();
    }

    void ServiceEx::onThreadCleanup()
    {
        root->InternalOnThreadCleanup();
    }

    void ServiceEx::onStarted()
    {
        root->InternalOnStarted();
    }

    void ServiceEx::onStopped()
    {
        root->InternalOnStopped();
    }

    void ServiceEx::onIdle()
    {
        root->InternalOnIdle();
    }

    void ServiceEx::onError(int error, const std::string& category, const std::string& message)
    {
        String^ cat = marshal_as<String^>(category);
        String^ msg = marshal_as<String^>(message);
        root->InternalOnError(errno, cat, msg);
    }

    Service::Service(int threads, bool pool) :
        _service(new std::shared_ptr<ServiceEx>(std::make_shared<ServiceEx>(threads, pool)))
    {
        _service->get()->root = this;
    }

    String^ Service::GenerateDataSize(double b)
    {
        std::ostringstream stream;

		int64_t bytes = (int64_t)b;
        int64_t abs_bytes = std::abs(bytes);

        if (abs_bytes >= (1024ll * 1024ll * 1024ll * 1024ll))
        {
            int64_t tb = bytes / (1024ll * 1024ll * 1024ll * 1024ll);
            int64_t gb = (bytes % (1024ll * 1024ll * 1024ll * 1024ll)) / (1024 * 1024 * 1024);
            stream << tb << '.' << ((gb < 100) ? "0" : "") << ((gb < 10) ? "0" : "") << gb << " TiB";
        }
        else if (abs_bytes >= (1024 * 1024 * 1024))
        {
            int64_t gb = bytes / (1024 * 1024 * 1024);
            int64_t mb = (bytes % (1024 * 1024 * 1024)) / (1024 * 1024);
            stream << gb << '.' << ((mb < 100) ? "0" : "") << ((mb < 10) ? "0" : "") << mb << " GiB";
        }
        else if (abs_bytes >= (1024 * 1024))
        {
            int64_t mb = bytes / (1024 * 1024);
            int64_t kb = (bytes % (1024 * 1024)) / 1024;
            stream << mb << '.' << ((kb < 100) ? "0" : "") << ((kb < 10) ? "0" : "") << kb << " MiB";
        }
        else if (abs_bytes >= 1024)
        {
            int64_t kb = bytes / 1024;
            bytes = bytes % 1024;
            stream << kb << '.' << ((bytes < 100) ? "0" : "") << ((bytes < 10) ? "0" : "") << bytes << " KiB";
        }
        else
            stream << bytes << " bytes";

        return marshal_as<String^>(stream.str());
    }

    String^ Service::GenerateTimePeriod(double milliseconds)
    {
        std::ostringstream stream;

        int64_t nanoseconds = (int64_t)(milliseconds * 1000.0 * 1000.0);
        int64_t abs_nanoseconds = Math::Abs(nanoseconds);

        if (abs_nanoseconds >= (60 * 60 * 1000000000ll))
        {
            int64_t hours = nanoseconds / (60 * 60 * 1000000000ll);
            int64_t minutes = ((nanoseconds % (60 * 60 * 1000000000ll)) / 1000000000) / 60;
            int64_t seconds = ((nanoseconds % (60 * 60 * 1000000000ll)) / 1000000000) % 60;
            int64_t milliseconds = ((nanoseconds % (60 * 60 * 1000000000ll)) % 1000000000) / 1000000;
            stream << hours << ':' << ((minutes < 10) ? "0" : "") << minutes << ':' << ((seconds < 10) ? "0" : "") << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds << " h";
        }
        else if (abs_nanoseconds >= (60 * 1000000000ll))
        {
            int64_t minutes = nanoseconds / (60 * 1000000000ll);
            int64_t seconds = (nanoseconds % (60 * 1000000000ll)) / 1000000000;
            int64_t milliseconds = ((nanoseconds % (60 * 1000000000ll)) % 1000000000) / 1000000;
            stream << minutes << ':' << ((seconds < 10) ? "0" : "") << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds << " m";
        }
        else if (abs_nanoseconds >= 1000000000)
        {
            int64_t seconds = nanoseconds / 1000000000;
            int64_t milliseconds = (nanoseconds % 1000000000) / 1000000;
            stream << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds << " s";
        }
        else if (abs_nanoseconds >= 1000000)
        {
            int64_t milliseconds = nanoseconds / 1000000;
            int64_t microseconds = (nanoseconds % 1000000) / 1000;
            stream << milliseconds << '.' << ((microseconds < 100) ? "0" : "") << ((microseconds < 10) ? "0" : "") << microseconds << " ms";
        }
        else if (abs_nanoseconds >= 1000)
        {
            int64_t microseconds = nanoseconds / 1000;
            nanoseconds = nanoseconds % 1000;
            stream << microseconds << '.' << ((nanoseconds < 100) ? "0" : "") << ((nanoseconds < 10) ? "0" : "") << nanoseconds << " mcs";
        }
        else
            stream << nanoseconds << " ns";

        return marshal_as<String^>(stream.str());
    }
}

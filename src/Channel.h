// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include <mutex>
#include <optional>
#include <condition_variable>


// TODO: Create a BufferedChannel when needed.

template<typename T>
class UnbufferedChannel
{
public:
    ~UnbufferedChannel()
    {
        close();
    }

    void send(T data_) // BLOCKING CALL TODO: Should this be Universal Reference T&&. Then non-trivial data types won't work.
    {
        std::unique_lock<std::mutex> dataGuard{dataMutex};
        data = std::forward<T>(data_);
        isDataSent = true;
        dataCondition.notify_all(); // Notify all the threads that are waiting on this condition. Aka all the receivers.
    }

    std::optional<T> receive() // BLOCKING CALL
    {
        std::unique_lock<std::mutex> dataGuard{dataMutex};
        dataCondition.wait(dataGuard, [this]()
        {
            return isDataSent || shouldClose;
        });

        isDataSent = false;

        if (shouldClose)
        {
            return std::nullopt;
        }

        return std::make_optional<T>(data);
    }

    void close()
    {
        if (shouldClose)
        {
            return;
        }

        shouldClose = true;

        std::unique_lock<std::mutex> dataGuard{dataMutex};
        isDataSent = true;
        dataCondition.notify_all(); // Notify all the threads that are waiting on this condition. Aka all the receivers.
    }

    [[nodiscard]] bool isClosed() const
    {
        return shouldClose;
    }

private:
    T data;
    std::mutex dataMutex;
    std::condition_variable dataCondition;
    bool isDataSent = false;
    std::atomic_bool shouldClose = false;
};

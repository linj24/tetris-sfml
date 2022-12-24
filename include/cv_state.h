#ifndef CV_STATE_H
#define CV_STATE_H

#include <condition_variable>
#include <shared_mutex>

struct CVState
{
    bool flag{};
    std::shared_mutex m;
    std::condition_variable_any cv;

    void wake()
    {
        {
            std::lock_guard lck(m);
            flag = true;
        }
        cv.notify_all();
    }
};

#endif
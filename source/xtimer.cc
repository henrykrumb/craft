#include <chrono>

#include "xtimer.h"

#include "unistd.h"
#include "signal.h"

/*----------------------------------------------------------------------*/
/* CLASS xtimer (funktions)                                              */
/*----------------------------------------------------------------------*/

long x_sys_time()
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();
    long duration = value.count();
    return duration;
}

void xtimer::start()
{
    stamp = x_sys_time();
    dt = 0;
}

void xtimer::stop()
{
    dt += (double)(x_sys_time() - stamp);
    stamp = x_sys_time();
}

void xtimer::cont()
{
    stamp = x_sys_time();
}

double xtimer::exec()
{
    return (double)(x_sys_time() - stamp) + dt;
}

double xtimer::read()
{
    return dt;
}

void sig(int signo)
{
    signal(SIGALRM, sig);
}

void delay(int dt)
{
    itimerval tick;

    tick.it_value.tv_sec = dt / 10000;
    tick.it_interval.tv_sec = 0;
    tick.it_value.tv_usec = dt * 10;
    tick.it_interval.tv_usec = 600000;

    signal(SIGALRM, sig);
    if (setitimer(ITIMER_REAL, &tick, 0) == 0)
    {
        pause();
    }
}

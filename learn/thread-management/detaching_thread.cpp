#include <iostream>
#include <thread>
#include <tracy/Tracy.hpp>

void do_background_work()
{
    ZoneScoped;
    // Simulate some background work
    for (int i = 0; i < 10000; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Simulate work by sleeping
        TracyPlot("Background Work Iteration", static_cast<int64_t>(i));
    }
}

int main()
{

    while (!TracyIsConnected)
    {
        // Wait for the profiler to connect
    }

    ZoneScoped;
    std::thread t(do_background_work);
    t.detach();

    std::cout << "Main thread t.joinable(): " << t.joinable() << std::endl;
    return 0;
}
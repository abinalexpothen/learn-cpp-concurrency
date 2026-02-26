// it's important to ensure that thread joins for any exit condition, whether through exception or otherwise; use RAII
// use join() in the destructor

#include <thread>
#include <iostream>
#include <vector>
#include <tracy/Tracy.hpp>

void do_something(int i)
{
    ZoneScoped;
    // std::cout << "I am doing something with " << i << std::endl; // <-- i want to flush buffer immediately, so endl
}

void do_something_with_std_exception()
{
    ZoneScoped;
    std::vector<int> v;
    v.at(100); // exception because vector is empty
}

class thread_guard
{
    std::thread &t;

public:
    explicit thread_guard(std::thread &t_) : t(t_) {}

    ~thread_guard()
    {
        ZoneScoped;
        if (t.joinable())
        {
            ZoneScopedN("Waiting for Join"); // Visualizes the blocking time
            t.join();
        }
        std::cout << "Called destructor of thread_guard" << std::endl;
    }

    // copying or assigning is are deleted to prevent the object from
    // outliving the scope of the thread it was joining
    thread_guard(thread_guard const &) = delete;
    thread_guard &operator=(thread_guard const &) = delete;
};

struct f
{
    int &i;
    f(int &i_) : i(i_) {}

    void operator()()
    {
        ZoneScoped;
        for (unsigned j = 0; j < 100000; j++)
        {
            TracyPlot("Loop Iteration", static_cast<int64_t>(j));
            do_something(i);
        }
    }
};

void problem_function()
{
    ZoneScoped;
    // even through the motivation to RAII in this context was to not use the try/catch,
    // it looks like the std::exception causes std::terminate to be called when there's no
    // try-catch for it.
    try
    {
        int some_local_state = 0;
        f my_function(some_local_state);
        std::thread t(my_function);
        thread_guard g(t);
        // note: local objects are destroyed in reverse order of construction,
        // so the destructor of thread_guard will be hit first; it has a valid t to join()
        do_something_with_std_exception();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught in main: " << e.what() << std::endl;
    }
}

int main()
{
    while (!TracyIsConnected)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ZoneScopedS(5);

    problem_function();
    return 0;
}

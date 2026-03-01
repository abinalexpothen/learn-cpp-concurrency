#include <thread>
#include <iostream>
#include <tracy/Tracy.hpp>

class protect_thread
{
    std::thread t;

public:
    // constructor that takes ownership of a thread object
    explicit protect_thread(std::thread t_) : t(std::move(t_))
    {
        ZoneScoped;
        if (!t.joinable())
        {
            throw std::logic_error("No thread to protect");
        }
    }

    ~protect_thread()
    {
        ZoneScoped;
        // join the thread in the destructor to ensure it finishes before the object is destroyed
        t.join();
    }

    // disable copy constructor
    protect_thread(const protect_thread &) = delete;
    // disable copy assignment operator
    protect_thread &operator=(const protect_thread &) = delete;
};

struct f
{
    int &i;
    f(int &i_) : i(i_) {}
    void operator()()
    {
        ZoneScoped;
        for (unsigned j = 0; j < 1000000; j++)
        {
            i++;
        }
    }
};

int main()
{
    while (!TracyIsConnected)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ZoneScoped;
    int local_var = 0;
    // create a protect_thread object that takes ownership of a thread that runs the function object f, which increments local_var
    protect_thread t{std::thread(f(local_var))};
    return 0;
}
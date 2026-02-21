#include <iostream>
#include <thread>
#include <vector>

void do_something()
{
    std::cout << "\n Do something amazing!";
}

void do_something_with(int i)
{
    std::cout << "I got " << i << std::endl; // <-- end forces immediate flush of buffer
}

class background_task
{
public:
    void
    operator()() const
    {
        do_something();
    }
};

// an example lifetime issue test
struct lifetime
{
    int &i;

    lifetime(int &i_) : i(i_) {}

    void operator()()
    {
        for (unsigned j = 0; j < 5; ++j)
        {
            do_something_with(i);
        }
    }

    void lifetimetest()
    {
        for (unsigned j = 0; j < 10; ++j)
        {
            std::cout << "\n Number is " << j;
        }
    }
};

void do_something_in_here_and_throw_excpetion()
{
    std::vector<int> v;     // empty vector
    std::cout << v.at(100); // access 100th element of empty vector, causes a standard exception
}

int main()
{
    background_task f;

    // use any callable types
    std::thread t1(do_something);
    std::thread t2(f);

    // this doesn't start a thread. It declared a t3 function that takes a single param  (of type pointer to function taking no params and returning background_task object
    // std::thread t3(background_task()); // <- compiler shows a warning

    // but, these do start a thread
    std::thread t4{background_task()};   // using uniform init syntax
    std::thread t5((background_task())); // extra paranthesis prevents impementation as function declaration

    // using lambda expression
    std::thread t6([]
                   { do_something(); });

    t1.join();
    t2.join();
    // t3.join(); -> error, because this is a function declaration
    t4.join();
    t5.join();
    t6.join();

    std::cout << "\n Demonstrate lifetime issues";
    int some_local_state = 0;
    lifetime my_lifetime(some_local_state);
    std::thread my_thread(my_lifetime);
    // my_thread.detach();
    std::cout << "\n Is thread joinable now? -> " << my_thread.joinable(); // <-this will cause interlaced cout display as both threads are printing to same resource
    my_thread.join();
    std::cout << "\n Is thread joinable now? -> " << my_thread.joinable() << "\n";

    // join threads even in the case of exceptions tp prevent accidental lifetime problems
    int another_local_var = 2;
    lifetime my_secondlifetime(another_local_var);
    std::thread my_second_thread(my_secondlifetime);
    try
    {
        do_something_in_here_and_throw_excpetion();
    }
    catch (...)
    {
        my_second_thread.join();
        throw;
    }

    my_second_thread.join();

    return 0;
}
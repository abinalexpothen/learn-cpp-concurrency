#include <iostream>
#include <thread>

void do_something()
{
    std::cout << "\n Do something amazing!";
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

int main()
{
    background_task f;

    // use any callable types
    std::thread t1(do_something);
    std::thread t2(f);

    // this doesn't start a thread. It declared a t3 function that takes a single param  (of type pointer to function taking no params and returning background_task object
    std::thread t3(background_task()); // <- compiler shows a warning

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

    return 0;
}
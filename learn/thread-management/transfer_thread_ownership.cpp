#include <thread>
#include <iostream>

void some_function()
{
    // some work
}

void some_other_function(int param)
{
    // some work with param
    std::cout << "Parameter: " << param << std::endl;
}

// demo transferring ownership outside of a function
std::thread f()
{
    return std::thread(some_function); // returns a thread object that represents the thread of execution started by some_function
}

std::thread g()
{
    std::thread t(some_other_function, 1);
    return t;
}

void h(std::thread t)
{
    t.join(); // join the thread to ensure it finishes before the program exits
}

void i()
{
    std::cout << "Demonstrating transfer of thread ownership" << std::endl;
    h(std::thread(some_function)); // creates a temporary thread object that is passed to h, which takes ownership of it and joins it
    std::cout << "Demonstrating transfer of thread ownership with parameters" << std::endl;
    std::thread t(some_other_function, 1);
    h(std::move(t)); // transfers ownership of the thread from t to h, which joins it
    // Will this cause termination? No, because h does join()
}

int main()
{

    i();

    std::thread t1(some_function);
    std::thread t2 = std::move(t1);           // transfer ownership of the thread from t1 to t2
    t1 = std::thread(some_other_function, 3); // t1 can now be assigned a new thread
    std::thread t3;                           // default constructor creates a thread object that does not represent a thread of execution
    t3 = std::move(t2);                       // transfer ownership of the thread from t2 to t3
    // t1 = std::move(t3);                       // transfer ownership of the thread from t3 to t1
    // <-- this will terminate because t1 already has a thread associated with it, and we are
    // trying to move another thread into it without joining or detaching the existing thread first

    t1.join(); // join the thread to ensure it finishes before the program exits
    t3.join();
}
#include <thread>

void some_function()
{
    // some work
}

void some_other_function()
{
    // some other work
}

// demo transferring ownership outside of a function
std::thread f()
{
    return std::thread(some_function); // returns a thread object that represents the thread of execution started by some_function
}

int main()
{
    std::thread t1(some_function);
    std::thread t2 = std::move(t1);        // transfer ownership of the thread from t1 to t2
    t1 = std::thread(some_other_function); // t1 can now be assigned a new thread
    std::thread t3;                        // default constructor creates a thread object that does not represent a thread of execution
    t3 = std::move(t2);                    // transfer ownership of the thread from t2 to t3
    t1 = std::move(t3);                    // transfer ownership of the thread from t3 to t1
    // <-- this will terminate because t1 already has a thread associated with it, and we are
    // trying to move another thread into it without joining or detaching the existing thread first

    t1.join(); // join the thread to ensure it finishes before the program exits
}
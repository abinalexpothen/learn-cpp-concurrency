#include <thread>
#include <iostream>
#include <string>

void function(int i, const std::string &s)
{
    std::cout << "Thread " << i << ": " << s << std::endl;
}

void problem_function(int param)
{
    char buffer[10];
    sprintf(buffer, "Value: %d", param);
    // buffer is a char * that the thread constructor will copy into the thread context
    // we're relying on implicit conversion of the pointer to the buffer into std::string object, this happens too late
    std::thread t(function, param, buffer);
    t.detach(); // detaching causes the buffer to go out of scope before the thread accesses it
}

void solution_function(int param)
{
    char buffer[10];
    sprintf(buffer, "Value: %d", param);
    std::thread t(function, param, std::string(buffer)); // <-- does an explicit conversion and copies the string into the thread context
    t.detach();                                          // now it's safe to detach because the thread has its own copy of the string
}

void update_data(int id, int &data)
{
    std::cout << "Updating data for thread ID: " << id << std::endl;
    data += 10;
}

void problem_update_data()
{
    int data = 0;
    // std::thread t(update_data, 1, data); <-- will cause a compilation error because it's na rvalue, not a reference
    std::thread t(update_data, 1, std::ref(data)); // <-- using std::ref to pass data by reference to the thread
    t.join();
    // some other process
}

class MyClass
{
public:
    void member_function(int param)
    {
        std::cout << "Member function called in thread with parameter: " << param << std::endl;
    }
};

// demo std::move to transfer ownership of a dynamic object into a thread
void move_demo(std::unique_ptr<int> ptr)
{
    std::cout << "Value in thread: " << *ptr << std::endl;
}

int main()
{
    MyClass obj;
    std::thread t(&MyClass::member_function, &obj, 42); // <-- passing the object pointer and parameter to the member function thread
    t.join();

    std::unique_ptr<int> p = std::make_unique<int>(100);
    std::thread t2(move_demo, std::move(p));

    // std::thread instances are movable but not copyable, like std::unique_ptr, so we can transfer ownership of a thread to another variable using std::move
    std::thread t3 = std::move(t2); // transferring ownership of the thread
    t3.join();
    return 0;
}
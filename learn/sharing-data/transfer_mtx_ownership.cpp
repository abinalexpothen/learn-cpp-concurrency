#include <mutex>
#include <thread>
#include <iostream>


class X{

    std::mutex m;

    public:

    void do_something()
    {
        // doing something
    }

    void prepare_data()
    {
        // prepare data
    }

    std::unique_lock<std::mutex> get_lock()
    {
        std::unique_lock<std::mutex> lk(m);
        prepare_data();
        return lk;
    }
};

void process_data()
{
    X x;
    std::unique_lock<std::mutex> lk(x.get_lock());
    x.do_something();
}

int main()
{
    std::thread t(process_data);
    std::cout << "Waiting to join in main";
    t.join();
    return 0;
}
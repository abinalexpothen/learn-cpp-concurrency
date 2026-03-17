#include <mutex>
#include <thread>

double get_next_data()
{
    return 0.0;
}

double process(double input)
{
    return 2.0 * input;
}

void write_result(double input, double result)
{

}

class X
{
    std::mutex m;

public:
    void get_and_process_data()
    {
        std::unique_lock<std::mutex> my_lock(m);
        double data_to_process = get_next_data();
        my_lock.unlock();
        double result = process(data_to_process);
        my_lock.lock();
        write_result(data_to_process, result);
    }
};

int main()
{
    return 0;
}
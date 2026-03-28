#include <mutex>
#include <queue>
#include <condition_variable>
#include <vector>
#include <iostream>
#include <thread>

struct Data
{
    int a;
    double b;
    char c;

    Data() : a(0), b(0.0), c(' ') {}

    // parametrized constructor
    Data(int a_, double b_, char c_) : a(a_), b(b_), c(c_) {}
};

std::vector<Data> input_data = {
    {0, 0.0, 'i'},
    {1, 1.0, 'j'},
    {2, 2.0, 'k'}
};

class ConditionVarTest
{
    std::mutex m;
    std::queue<Data> data_queue;
    std::condition_variable data_cond;

    public:

    Data prepare_data(std::vector<Data> &vec)
    {
        Data data = vec.back();
        vec.pop_back();
        return data;
    }

    void data_preparation_function()
    {
        while (!input_data.empty())
        {
            const Data data = prepare_data(input_data);
            {
                std::lock_guard<std::mutex> lk(m);
                data_queue.push(data);
            }
            data_cond.notify_one();
        }
    }

    void data_processing_function()
    {
        while(true)
        {
            std::unique_lock<std::mutex> lk(m);
            data_cond.wait(lk, [this]{ return !data_queue.empty(); });
            Data data = data_queue.front();
            data_queue.pop();
            lk.unlock();
            process(data);
            if(data_queue.empty())
            {
                break;
            }
        }
    }

    void process(Data data)
    {
        std::cout << "\n Data values are -> a: " << data.a << " b: " << data.b << " c: " << data.c;
    }
};

int main()
{
    ConditionVarTest c;

    // create two threads and in one, call the data preparation and in the other ones
    // call data processor

    std::thread preparation_thread(&ConditionVarTest::data_preparation_function, &c);

    std::thread processor_thread(&ConditionVarTest::data_processing_function, &c);

    processor_thread.join();
    preparation_thread.join();

    return 0;
}
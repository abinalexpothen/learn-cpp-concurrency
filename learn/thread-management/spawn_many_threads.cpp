#include <thread>
#include <vector>
#include <iostream>

int main()
{
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back([i]()
                             { 
                                // since std::cout is shared among threads, we will have a messy interleaved output
                                std::cout << "Hello from thread " << i << std::endl; });
    }

    for (auto &t : threads)
    {
        t.join();
    }
}
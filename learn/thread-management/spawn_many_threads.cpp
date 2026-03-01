#include <thread>
#include <vector>
#include <iostream>
#include <numeric>

template <typename Iterator, typename T>
struct accumulate_stuff
{
    void operator()(Iterator first, Iterator last, T &result)
    {
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T accumulate_parallely(Iterator first, Iterator last, T init)
{
    const unsigned long len = std::distance(first, last);
    std::cout << "len: " << len << std::endl;

    if (!len)
        return init;

    const unsigned long min_per_thread = 25;
    std::cout << "min_per_thread: " << min_per_thread << std::endl;
    const unsigned long max_threads = (len + min_per_thread - 1) / min_per_thread;
    std::cout << "max_threads: " << max_threads << std::endl;
    const unsigned long hardware_threads = std::thread::hardware_concurrency();
    std::cout << "hardware_threads: " << hardware_threads << std::endl;
    const unsigned long num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    std::cout << "num_threads: " << num_threads << std::endl;
    const unsigned long block_size = len / num_threads;
    std::cout << "block_size: " << block_size << std::endl;

    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);
    Iterator block_start = first;

    for (unsigned long i = 0; i < num_threads - 1; i++)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_stuff<Iterator, T>(), block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }

    accumulate_stuff<Iterator, T>()(block_start, last, results[num_threads - 1]);

    for (auto &entry : threads)
    {
        entry.join();
    }

    return std::accumulate(results.begin(), results.end(), init);
}

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
        std::cout << "Joining thread " << t.get_id() << std::endl;
        t.join();
    }

    std::vector<int> data(100000000);
    std::iota(data.begin(), data.end(), 1); // fill with 1, 2, ..., 1000000

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    int result = accumulate_parallely(data.begin(), data.end(), 0);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Result of parallel accumulation: " << result << std::endl; // should be 500500
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time in microseconds: " << elapsed_seconds.count() * 1000000 << " microseconds" << std::endl;

    // trasitional summation using std::accumulate for comparison
    start = std::chrono::steady_clock::now();
    int sequential_result = std::accumulate(data.begin(), data.end(), 0);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> sequential_elapsed_seconds = end - start;
    std::cout << "Result of sequential accumulation: " << sequential_result << std::endl;
    std::cout << "Elapsed time for sequential in microseconds: " << sequential_elapsed_seconds.count() * 1000000 << " microseconds" << std::endl;

    return 0;
}
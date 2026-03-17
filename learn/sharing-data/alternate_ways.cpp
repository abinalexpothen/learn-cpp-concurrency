#include <thread>
#include <memory>
#include <string>
#include <mutex>
#include <iostream>
#include <vector>

struct HeavyResource {

    HeavyResource() 
    {
        // This should be printed only once
        std::cout << "\n Heavy resource constructor";
    }
    void do_something(int thread_id) {
        std::cout << "\n Resource used by Thread id: " << thread_id;
    }
};

class some_resource
{
    std::shared_ptr<HeavyResource> resource_ptr;
    std::once_flag resource_flag;

    public:
    some_resource() : resource_ptr(nullptr) {}

    void foo(int thread_id)
    {
        // lazy initialization
        std::call_once(resource_flag, [this](){
            resource_ptr = std::make_shared<HeavyResource>();
        });

        if (resource_ptr)
        {
            resource_ptr->do_something(thread_id);
        }
    }

    // another thread-safe way to handle this
    void bar(int thread_id)
    {
        // only 1 thread does this init and no one proceeds until init complete
        static auto resource_ptr = std::make_shared<HeavyResource>();
        resource_ptr->do_something(thread_id);
    }
};

int main()
{
    some_resource shared_obj;
    std::vector<std::thread> threads;
    std::cout << "Starting threads";

    // launch 5 threads all trying to use the resource at once
    for (int i = 0; i < 5; ++i)
    {
        threads.emplace_back(&some_resource::foo, &shared_obj, i);
    }

    for (auto &t: threads)
    {
        t.join();
    }

    std::cout << "All threads finished. \n";
    return 0;
}
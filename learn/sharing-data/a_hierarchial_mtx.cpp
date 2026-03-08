#include <mutex>
#include <climits>
#include <stdexcept>

class hierarcial_mtx
{
    std::mutex mtx;
    unsigned long const level;
    unsigned long prev_level;
    static thread_local unsigned long this_thread_level;

    void check_hierarchy_violation()
    {
        if (this_thread_level <= level)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
    }

    void update_hierarchy_value()
    {
        prev_level = this_thread_level;
        this_thread_level = level;
    }

    public:
    explicit hierarcial_mtx(unsigned long val) : level(val), prev_level(0) {}

    void lock()
    {
        check_hierarchy_violation();
        mtx.lock();
        update_hierarchy_value();
    }

    void unlock()
    {
        if (this_thread_level != level)
        {
            throw std::logic_error("mutex hierarchy violated");
        }

        this_thread_level = prev_level;
        mtx.unlock();
    }

    bool try_lock()
    {
        check_hierarchy_violation();
        if (!mtx.try_lock())
        {
            return false;
        }
        update_hierarchy_value();
        return true;
    }
};

thread_local unsigned long hierarcial_mtx::this_thread_level(ULONG_MAX);

int main()
{
    // TODO something with hierarchial_mtx 
    return 0;
}
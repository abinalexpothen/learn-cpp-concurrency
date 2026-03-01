#include <thread>

class j_thread
{
    std::thread t;

public:
    j_thread() noexcept = default;

    template <typename Callable, typename... Args>
    explicit j_thread(Callable &&f, Args &&...args) : t(std::forward<Callable>(f), std::forward<Args>(args)...) {}

    ~j_thread() noexcept
    {
        if (joinable())
        {
            join();
        }
    }

    // move constructor
    j_thread(j_thread &&other) noexcept : t(std::move(other.t)) {}

    // move assignment operator
    j_thread &operator=(j_thread &&other) noexcept
    {
        if (joinable())
        {
            join();
        }
        t = std::move(other.t);
        return *this;
    }

    // note: this is not a copy assignment. note the argument.
    j_thread &operator=(std::thread other) noexcept
    {
        if (joinable())
        {
            join();
        }
        t = std::move(other);
        return *this;
    }

    void swap(j_thread &other) noexcept
    {
        t.swap(other.t);
    }

    std::thread::id get_id() const noexcept
    {
        return t.get_id();
    }

    bool joinable() const noexcept
    {
        return t.joinable();
    }

    void join()
    {
        t.join();
    }

    void detach()
    {
        t.detach();
    }

    std::thread &as_thread() noexcept
    {
        return t;
    }

    const std::thread &as_thread() const noexcept
    {
        return t;
    }
};

int main()
{
    j_thread jt;
    return 0;
}
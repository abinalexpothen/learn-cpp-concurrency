#include <exception>
#include <memory>
#include <stack>
#include <mutex>

struct empty_stack : std::exception
{
    const char *what() const noexcept override
    {
        return "empty stack";
    }
};

template <typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;

public:
    threadsafe_stack() = default;

    threadsafe_stack(const threadsafe_stack &other)
    {
        std::lock_guard<std::mutex> guard(other.m);
        data = other.data;
    }

    threadsafe_stack &operator=(const threadsafe_stack &) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> guard(m);
        data.push(std::move(new_value));
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> guard(m);
        if (data.empty())
        {
            throw empty_stack();
        }
        std::shared_ptr<T> const res(std::make_shared<T>(data.top())); // allocate return val before stack mod
        data.pop();
        return res;
    }

    void pop(T &value)
    {
        std::lock_guard<std::mutex> guard(m);
        if (data.empty())
        {
            throw empty_stack();
        }
        value = data.top();
        data.pop();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

int main()
{
    return 0;
}
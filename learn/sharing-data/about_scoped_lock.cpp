#include <mutex>

class X
{
};

class BigObject
{
    X x;
    std::mutex m;

public:
    BigObject() = default;
    BigObject(const BigObject &) = delete;
    BigObject &operator=(const BigObject &) = delete;

    void swap(BigObject &a, BigObject &b)
    {
        // swap the contents of this object with the other object
        if (&a == &b)
        {
            return; // no need to swap if both objects are the same
        }

        // style 1
        {
            std::lock(a.m, b.m); // lock both mutexes without deadlock
            std::lock_guard<std::mutex> lock_a(a.m, std::adopt_lock);
            std::lock_guard<std::mutex> lock_b(b.m, std::adopt_lock);
            std::swap(a.x, b.x); // swap the contents of the two objects
        }

        // style 2 (C++17 and later)
        {
            std::scoped_lock<std::mutex, std::mutex> lock(a.m, b.m); // lock both mutexes without deadlock
            std::swap(a.x, b.x);                                     // swap the contents of the two objects
        }

        // style 3 with template argument deduction (C++17 and later)
        {
            std::scoped_lock lock(a.m, b.m); // lock both mutexes without deadlock
            std::swap(a.x, b.x);             // swap the contents of the two objects
        }

        // another way, using std::unique_lock and std::defer_lock
        {
            std::unique_lock<std::mutex> lock_a(a.m, std::defer_lock);
            std::unique_lock<std::mutex> lock_b(b.m, std::defer_lock);
            std::lock(lock_a, lock_b); // lock both mutexes without deadlock
            std::swap(a.x, b.x);       // swap the contents of the two objects
        }
    }
};

int main()
{
    BigObject a, b;
    a.swap(a, b); // swapping an object with itself is safe
    return 0;
}

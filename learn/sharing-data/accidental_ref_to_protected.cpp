#include <string>
#include <mutex>

class X
{
    int data;
    std::string name;

public:
    void do_something()
    {
        // do something with data and name
    }
};

class X_wrapper
{
    X x;
    std::mutex m;

public:
    template <typename F>
    void process(F fn)
    {
        std::lock_guard<std::mutex> guard(m);
        fn(x); // <-- protected access to x, but fn can still modify x in the case a user-supplied function
    }
};

X *y; // global pointer to X

void malicious_fn(X &x)
{
    y = &x; // store the address of x in a global variable
}

int main()
{
    X_wrapper xwrap;
    xwrap.process(malicious_fn); // pass the malicious function to process
    y->do_something();           // call a method on the global pointer, which is now pointing to the protected X instance
    return 0;
}
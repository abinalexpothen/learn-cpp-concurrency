#include <list>
#include <mutex>
#include <algorithm>
#include <iostream>

class X
{
    std::list<int> data;
    std::mutex m;

public:
    void add_to_list(int new_value)
    {
        std::lock_guard<std::mutex> guard(m);
        data.push_back(new_value);
    }

    bool found_in_list(int value_to_find)
    {
        std::lock_guard<std::mutex> guard(m);
        return std::find(data.begin(), data.end(), value_to_find) != data.end();
    }

    void print_list()
    {
        std::lock_guard guard(m); // can also be written like this in C++17 and later (uses template argument deduction)
        for (const auto &value : data)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

int main()
{
    X x;
    x.add_to_list(1);
    x.add_to_list(2);
    x.add_to_list(3);

    if (x.found_in_list(2))
    {
        std::cout << "Found 2 in the list." << std::endl;
    }

    return 0;
}
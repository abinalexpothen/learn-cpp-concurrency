#include <iostream>
#include <thread>

void hello()
{
  std::cout << "Hello from thread!\n";
}
int main()
{
  std::thread t(hello);
  t.join();
  return 0;
}
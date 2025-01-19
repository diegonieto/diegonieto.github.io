#include <future>
#include <iostream>
#include <string>
#include <thread>

// 1000
std::string processString(int value)
{
  std::string str;
  while (value != 0)
  {
    str.insert(0,std::to_string(value%10)); // get the last digit
    value/=10;
  }
  return str;
}

int main()
{
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    std::thread thread([&promise](){
        std::cout << std::this_thread::get_id() << " thread is started " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << " setting promise " << std::endl;
        promise.set_value(5);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::cout << std::this_thread::get_id() << " doing another things " << std::endl;

    std::cout << std::this_thread::get_id() << " task result: " << future.get() << std::endl;

    thread.join();


    std::cout << " process string: " << processString(150) << std::endl;
}

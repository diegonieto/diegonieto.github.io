#include <iostream>
#include <future>
#include <thread>

int main() {

    std::future<int> future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << std::this_thread::get_id() << " doing thread things " << std::endl;
        return 16;
    });
    std::cout << " doing main thread things: " << std::this_thread::get_id << std::endl;
    std::cout << "future " << future.get() << std::endl;

    return 0;
}

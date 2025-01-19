#include <future>
#include <iostream>
#include <thread>

int main()
{
    std::packaged_task<int()> task([]{
        std::cout << " packaged task started" << std::endl;
        return 0;
    });
    std::future<int> future = task.get_future();
    std::thread thread(std::move(task));
    std::cout << " doing another things " << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));


    std::cout << " task result: " << future.get() << std::endl;

    thread.join();
}

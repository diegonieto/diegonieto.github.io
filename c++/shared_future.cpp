#include <future>
#include <iostream>
#include <thread>

int main()
{
    std::promise<void> readyPromise;
    std::shared_future<void> barrier(readyPromise.get_future());

    std::future<int> future1 = std::async(std::launch::async, [barrier](){
        std::cout << std::this_thread::get_id() << " future 1 async before wait " << std::endl;
        barrier.wait();
        std::cout << std::this_thread::get_id() << " future 1 async after wait " << std::endl;
        return 1;
    });

    std::future<int> future2 = std::async(std::launch::async, [barrier](){
        std::cout << std::this_thread::get_id() << " future 2 async before wait " << std::endl;
        barrier.wait();
        std::cout << std::this_thread::get_id() << " future 2 async after wait " << std::endl;
        return 2;
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << std::this_thread::get_id() << " main doing a lot of things " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    readyPromise.set_value();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << std::this_thread::get_id()
                << " future 1 is: " << future1.get()
                << " future 2 is: " << future2.get()
                << std::endl;


    return 0;
}

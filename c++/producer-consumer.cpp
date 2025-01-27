#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <random>

using namespace std;

int main() {
    int size = 0;
    const int maxSize = 10;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 300);


    std::condition_variable empty,full;
    std::mutex mutex;

    std::thread producer([&size,&mutex,&empty,&full,&gen,&dist](){
        std::unique_lock<std::mutex> lk(mutex);
        while (true)
        {
            {
                std::cout << std::this_thread::get_id() << " producer waiting: " << (size == maxSize) << std::endl;
                full.wait(lk, [&size]{
                    return size <= maxSize;
                });
            }
            size++;
            empty.notify_all();
            lk.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(dist(gen)));
            lk.lock();
            std::cout << " producer buffer size is: " << size << std::endl;
            std::this_thread::yield();
        }
        std::cout << "end producing" <<std::endl;
    });


    std::thread consumer([&size,&mutex,&empty,&full,&gen,&dist](){
        std::unique_lock<std::mutex> lk(mutex);
        while (true)
        {
            std::cout << std::this_thread::get_id() << " consumer waiting: " << (size == 0) << std::endl;
            empty.wait(lk, [&size]{
                return size > 0;
            });
            {
                size--;
                full.notify_all();
                lk.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(dist(gen)));
                lk.lock();
            }
            std::cout << " consumer buffer size is: " << size << std::endl;
            std::this_thread::yield();
        }
        std::cout << "end consumer" <<std::endl;
    });

    producer.join();
    consumer.join();

    return 0;
}

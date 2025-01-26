#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <vector>

class Barrier
{
public:
    Barrier(int numThreads) : _remainingThreads(numThreads),
                              _initialThreads(numThreads)
    {
    }
    void wait();

private:
    int _remainingThreads;
    int _initialThreads;
    std::mutex _mutex;
    std::condition_variable cv;
};

void Barrier::wait()
{
    std::unique_lock<std::mutex> lk(_mutex);
    _remainingThreads--;
    if (_remainingThreads > 0)
    {
        std::cout << "Before work thread " << std::this_thread::get_id() << std::endl;
        cv.wait(lk);
        _remainingThreads = _initialThreads;
    }
    else
    {
        cv.notify_all();
    }
    std::cout << "After barrier " << std::endl;
}

void heavyTask(Barrier &barrier)
{
    const int maxIterations = 1;
    for (unsigned int iter = 0; iter < maxIterations; iter++)
    {
        barrier.wait();
    }
}

int main()
{
    const int numThreads = 5;

    std::vector<std::thread> _threads;

    Barrier barrier(numThreads);

    for (unsigned int spawnThreads = 0; spawnThreads < numThreads; spawnThreads++)
    {
        _threads.push_back(std::thread([&barrier, spawnThreads]()
                                       {
                                           heavyTask(barrier);
                                       }));
    }

    for (auto &thread : _threads)
    {
        thread.join();
    }
    _threads.clear();

    return 0;
}

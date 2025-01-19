# Introduction
The aim of this file is to learn Rust by comparing its functionaly with C++ by examples.

## C++ VS Rust

### ASYNC

#### Tasks

[C++](c++/async.cpp) `std::async`.
```c++
    std::future<int> future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << std::this_thread::get_id() << " doing thread things " << std::endl;
        return 16;
    });
    std::cout << " doing main thread things: " << std::this_thread::get_id << std::endl;
    std::cout << "future " << future.get() << std::endl;
```
The above is util while we can either choose between `std::launch::async` or `std::launch::deferred`.
The sooner will start the thread inmediately and we use the future to retrieve the future when it finishes.
The latter will provide will start the task only when the result is needed.

[Rust](rust/src/bin/async.rs) by using `tokio::task`

```rust
async fn do_something() {
    println!("Starting a task...");
    tokio::time::sleep(Duration::from_secs(2)).await; // Simulate async task
    println!("Task finished!");
}

#[tokio::main]
async fn main() {
    let task1 = task::spawn(do_something());
    let task2 = task::spawn(do_something());

    let _ = tokio::try_join!(task1, task2);
}
```
In Rust however, this task is not inmediately runned, it is placed in the async runtime's queue task.

[C++](c++/packaged_task.cpp) `std::package_task`
This is another way to work with tasks and threads in C++. Unlike `std::async` here we can control when
to execute the task and in which thread explicitely.
```c++
    std::packaged_task<int()> task([]{
        std::cout << " packaged task started" << std::endl;
        return 0;
    });
    std::future<int> future = task.get_future();
    std::thread thread(std::move(task));
```

Which is equivalent to get pass the future to the thread and get the future later in [this way](c++/future.cpp)
```c++
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    std::thread thread([&promise](){
        std::cout << std::this_thread::get_id() << " thread is started " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << " setting promise " << std::endl;
        promise.set_value(5);
    });
    future.get()
```

[Rust](rust/src/bin/async2.rs) `std::thread`
Similar to what we did before with `Tokio`, we can instantiate `std::threads` with our task
```rust
fn do_something() -> i32 {
    thread::sleep(Duration::from_secs(2));
    42
}

fn main() {
    let handle = thread::spawn(|| do_something());
}
```

#### Barrier

In C++ we can implement barriers with `shared_future` or `std::barrier` in C++ 20.
[C++](c++/shared_future.cpp) `std::shared_future` work in a similar way as `std::future`, but
letting several threads to wait for the same result:
```c++
    std::promise<void> readyPromise;
    std::shared_future<void> barrier(readyPromise.get_future());

    std::future<int> future1 = std::async(std::launch::async, [barrier](){
        barrier.wait();
        return 1;
    });

    std::future<int> future2 = std::async(std::launch::async, [barrier](){
        barrier.wait();
        return 2;
    });

    // Do stuff
    readyPromise.set_value();
```

The [C++](c++/barriercpp20.cpp) `std::barrier` version is easier to implement:
```c++
    const auto workers = {"Anil", "Busara", "Carl"};

    auto on_completion = []() noexcept
    {
        static auto phase = "Cleaning up...\n";
    };

    std::barrier sync_point(std::ssize(workers), on_completion);
    auto work = [&](std::string name)
    {
        std::string product = "  " + name + " worked\n";
        std::osyncstream(std::cout) << product;  // ok, op<< call is atomic
        sync_point.arrive_and_wait();
    };

    std::vector<std::jthread> threads;
    threads.reserve(std::size(workers));
    for (auto const& worker : workers)
        threads.emplace_back(work, worker);
```

The third [C++](c++/barrier.cpp) uses `std::condition_variable` to implement that. However this complicates the code.
That version is comparable to the Rust condition variable version.

The [Rust](rust/src/bin/barrier_tokio.rs) `Tokio` is comparable to the `std::barrier` in C++:
```rust
    let barrier = Arc::new(Barrier::new(NUM_THREADS));

    let mut handles = vec![];

    for _ in 0..NUM_THREADS {
        let barrier = Arc::clone(&barrier);
        let handle = tokio::spawn(async move {
            println!("Task before barrier");
            barrier.wait().await;
            println!("Task after barrier");
        });
        handles.push(handle);
    }
```


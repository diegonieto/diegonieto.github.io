# Introduction
The aim of this file is to learn Rust by comparing its functionaly with C++ by examples.

## C++ VS Rust

### ASYNC/TASK

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

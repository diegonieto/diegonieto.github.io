use tokio::task;
use std::time::Duration;

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

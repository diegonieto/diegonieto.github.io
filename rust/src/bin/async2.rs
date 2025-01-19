use std::thread;
use std::time::Duration;

fn do_something() -> i32 {
    thread::sleep(Duration::from_secs(2));
    42
}

fn main() {
    let handle = thread::spawn(|| do_something());

    let result = handle.join().unwrap();
    println!("Result: {}", result);
}

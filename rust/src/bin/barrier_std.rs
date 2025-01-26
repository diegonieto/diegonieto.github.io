use std::sync::{Arc, Barrier};
use std::thread;

fn main() {
    let n = 5;
    let mut handles = Vec::with_capacity(n);
    let barrier = Arc::new(Barrier::new(n));
    for i in 0..n {
        let c = Arc::clone(&barrier);
        handles.push(thread::spawn(move || {
            println!("before wait thread {}", i);
            c.wait();
            println!("after wait thread {}", i);
        }));
    }
    for handle in handles {
        handle.join().unwrap();
    }
}

use std::sync::{Arc, Mutex, Condvar};
use std::thread;

fn main() {
    let num_threads = 5;

    let counter = Arc::new((Mutex::new(num_threads), Condvar::new()));

    let mut handles = vec![];

    for i in 0..num_threads {
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            let (lock, cvar) = &*counter;

            println!("Thread {} is doing work before waiting.", i);

            let mut num = lock.lock().unwrap();
            *num -= 1;
            if *num == 0 {
                println!("Last thread!");
                cvar.notify_all();
            } else {
                let _unused = cvar.wait(num).unwrap();
            }

            println!("Thread {} is proceeding after being notified.", i);
        });

        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }
}

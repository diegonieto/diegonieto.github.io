use std::sync::{mpsc, Arc, Mutex};
use std::thread;
use std::time::Duration;

fn main() {
    let (tx, rx) = mpsc::channel();

    let rx = Arc::new(Mutex::new(rx));

    let num_producers = 5;
    let mut handles = vec![];

    for i in 0..num_producers {
        let tx = tx.clone();

        let handle = thread::spawn(move || {
            for j in 0..10 {
                thread::sleep(Duration::from_millis(100));
                let msg = format!("Producer {}: Message {}", i, j);
                tx.send(msg).expect("Failed to send message");
                println!("Producer {} sent: Message {}", i, j);
            }
        });

        handles.push(handle);
    }

    let consumer_handle = thread::spawn(move || loop {
        let msg = rx.lock().unwrap().recv();
        match msg {
            Ok(message) => {
                println!("Consumer received: {}", message);
            }
            Err(_) => {
                println!("Consumer: No more messages to receive.");
                break;
            }
        }
    });

    for handle in handles {
        handle.join().unwrap();
    }

    drop(tx);
    consumer_handle.join().unwrap();
}

use tokio::sync::Barrier;
use std::sync::Arc;

#[tokio::main]
async fn main() {
    const NUM_THREADS: usize = 3;

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

    for handle in handles {
        handle.await.unwrap();
    }
}

import java.time.Duration;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Random;

public class Main {
    private static final Random generator = new Random();
    private static final Queue<String> queue = new LinkedList<>();

    public static void main(String[] args) {
        int itemCount = 50;      
        Thread producer = new Thread(() -> {
            for (int i = 0; i < itemCount; i++) {
                try {
                    Thread.sleep(Duration.ofSeconds(generator.nextInt(2)).toMillis());
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                synchronized (queue) {
                    System.out.println("Produce item no. " + i);
                    queue.add("Item no. " + i);
                    queue.notify();
                }
            }
        });

        Thread consumer = new Thread(() -> {
            int itemsLeft = itemCount;
            while (itemsLeft > 0) {
                String item;
                synchronized (queue) {
                    while (queue.isEmpty()) {
                        try {
                            queue.wait();
                        } catch (InterruptedException e) {
                            throw new RuntimeException(e);
                        }
                    }
                    item = queue.poll();
                }
                itemsLeft--;
                System.out.println("Read item: " + item);
            }
        });
        consumer.start();
        producer.start();
    }
}
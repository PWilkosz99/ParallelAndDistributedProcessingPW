import java.util.concurrent.*;

public class FindMaxJava {
    public static void main(String[] args) throws InterruptedException, ExecutionException {
            ExecutorService executor = Executors.newFixedThreadPool(16);
            executor.execute(new HelloRunnable());
            Callable<Integer> callable = new HelloCallable();
            Future<Integer> future = executor.submit(callable);
            System.out.println(future.get());
    }
}
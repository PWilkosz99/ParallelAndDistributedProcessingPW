import java.util.Random;
import java.util.concurrent.ForkJoinPool;

public class Main {

    private static final int numCount = 100;

    public static void main(String[] args) {

        int[] numArray = new int[numCount];
        Random random = new Random();

        for (int i = 0; i < numCount; i++) {
            numArray[i] = random.nextInt(1000);
            System.out.print(numArray[i] + "\t");
        }

        System.out.println("\n\n\n");

        DivideTask divideTask = new DivideTask(numArray);
        ForkJoinPool forkJoinPool = new ForkJoinPool();

        forkJoinPool.execute(divideTask);
        var sortedArray = divideTask.join();

        for (int val : sortedArray) {
            System.out.print(val + "\t");
        }
    }
}

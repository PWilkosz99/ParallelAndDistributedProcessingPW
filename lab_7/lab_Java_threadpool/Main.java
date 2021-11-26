import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class Main {

    private static final int NTHREADS = 10;

    public static void main(String[] args) {
        // seq
        Calka_callable calka = new Calka_callable(0, Math.PI, 0.01);
        var res1 = calka.compute();

        System.out.println(res1);

        // par
        double res = 0;
        double dx=0.01;


        double xp = 0;
        double xk = Math.PI;
        int div=(int) ((xk-xp)/dx);

        ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
        List<Future<Double>> list = new ArrayList<Future<Double>>();

        for (int i = 0; i < div; i++) {
            Callable<Double> callable = new Calka_callable(i * dx, (i + 1) * dx, dx);
            Future<Double> future = executor.submit(callable);
            list.add(future);
        }

        for (Future<Double> ftr : list) {
            try {
                res += ftr.get();
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
        }
        executor.shutdown();

        System.out.println(res);
    }
}

import java.util.concurrent.Callable;

public class HelloCallable implements Callable<Integer> {

    @Override
    public Integer call() throws Exception {
        System.out.print("Called");
        return 150;
    }
    
}

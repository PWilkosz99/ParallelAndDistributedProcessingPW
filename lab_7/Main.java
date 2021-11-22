public class Main {
    public static void main(String[] args) {
        //seq
        Calka_callable calka = new Calka_callable(0, 100, 0.1);
        var res1 = calka.compute();

        System.out.println(res1);
    }
}

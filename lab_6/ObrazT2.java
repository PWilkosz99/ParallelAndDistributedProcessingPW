public class ObrazT2 implements Runnable {
    private Obraz obraz;
    int a;
    int b;

    ObrazT2(Obraz o, int a, int b) {
        obraz = o;
        this.a = a;
        this.b = b;
    }

    @Override
    public void run() {
        obraz.calculate_histogram(a, b);
        obraz.print_histogram(a, b);
    }

}

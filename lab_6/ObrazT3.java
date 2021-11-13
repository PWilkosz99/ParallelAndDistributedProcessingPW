public class ObrazT3 implements Runnable {
    Obraz2 obraz;
    int row;

    ObrazT3(Obraz2 o, int r) {
        obraz = o;
        row = r;
    }

    @Override
    public void run() {
        obraz.calculate_histogram(row);
    }

}

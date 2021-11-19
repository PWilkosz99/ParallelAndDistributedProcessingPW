public class ObrazT3 implements Runnable {
    Obraz obraz;
    int row;

    ObrazT3(Obraz o, int r) {
        obraz = o;
        row = r;
    }

    @Override
    public void run() {
        obraz.calculate_histogram2(row);
        obraz.syncArrays(row);
    }
}

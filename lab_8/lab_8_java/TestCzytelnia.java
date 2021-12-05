public class TestCzytelnia {
    private final static int liczbaPisarzy = 5;
    private final static int liczbaCzytelnikow = 15;

    public static void main(String[] args) throws InterruptedException {
        Czytelnia czytelnia = new Czytelnia();
        int l_watkow = liczbaPisarzy + liczbaCzytelnikow;
        Runnable[] rPisarz = new Runnable[liczbaPisarzy];
        Runnable[] rCzytelnik = new Runnable[liczbaCzytelnikow];
        Thread[] threads = new Thread[l_watkow];

        for (int index = 0; index < liczbaCzytelnikow; index++) {
            rCzytelnik[index] = new Czytelnik(czytelnia, index);
        }

        for (int index = 0; index < liczbaPisarzy; index++) {
            rPisarz[index] = new Pisarz(czytelnia, index);
        }

        for (int i = 0; i < liczbaPisarzy; i++) {
            threads[i] = new Thread(rPisarz[i]);
        }

        for (int i = 0; i < liczbaCzytelnikow; i++) {
            threads[i + liczbaPisarzy] = new Thread(rCzytelnik[i]);
        }

        for (int i = 0; i < l_watkow; i++) {
            threads[i].start();
        }

        for (int i = 0; i < l_watkow; i++) {
            threads[i].join();
        }

    }
}

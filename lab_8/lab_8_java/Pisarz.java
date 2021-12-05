import java.util.Random;

public class Pisarz implements Runnable {
    private final Czytelnia czytelnia;
    private final int id;

    public Pisarz(Czytelnia c, int id) {
        czytelnia = c;
        this.id = id;
    }

    @Override
    public void run() {
        Random random = new Random();
        for (;;) {
            try {
                Thread.sleep(random.nextInt(1000));
                System.out.println(String.format("Pisarz %d przed zamkiem", id));
                czytelnia.chce_pisac();
                System.out.println(String.format("Pisarz %d wchodzi do czytelni", id));
                czytelnia.pisze();
                System.out.println(String.format("Pisarz %d wychodzi z czytelni", id));
                czytelnia.koniec_pisania();
                System.out.println(String.format("Pisarz %d po zamku", id));
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

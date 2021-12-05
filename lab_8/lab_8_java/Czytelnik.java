import java.util.Random;

public class Czytelnik implements Runnable {
    private final Czytelnia czytelnia;
    private final int id;

    public Czytelnik(Czytelnia c, int id) {
        czytelnia = c;
        this.id = id;
    }

    @Override
    public void run() {
        Random random = new Random();
        for (;;) {
            try {
                Thread.sleep(random.nextInt(1000));
                System.out.println(String.format("Czytelnik %d przed zamkiem", id));
                czytelnia.chce_czytac();
                System.out.println(String.format("Czytelnik %d wchodzi do czytelni", id));
                czytelnia.czytam();
                System.out.println(String.format("Czytelnik %d wychodzi z czytelni", id));
                czytelnia.koniec_czytania();
                System.out.println(String.format("Czytelnik %d po zamku", id));
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

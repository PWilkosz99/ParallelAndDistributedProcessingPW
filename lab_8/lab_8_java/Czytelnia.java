import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class Czytelnia {
    int liczba_czyt = 0;
    int liczba_pisz = 0;
    final ReentrantLock zamek = new ReentrantLock();
    Condition czytelnicy = zamek.newCondition();
    Condition pisarze = zamek.newCondition();

    public void chce_czytac() throws InterruptedException {
        zamek.lock();
        if (zamek.hasWaiters(pisarze) || liczba_pisz > 0) {
            czytelnicy.await();
        }
        liczba_czyt++;
        czytelnicy.signal();
    }

    public void czytam() throws InterruptedException {
        zamek.lock();
        System.out.println(String.format("Liczba czytelnikow: %d\tLiczba pisarzy: %d\n", liczba_czyt, liczba_pisz));
        zamek.unlock();
        Thread.sleep(500);
    }

    public void koniec_czytania() {
        zamek.lock();
        liczba_czyt--;
        if (liczba_czyt == 0) {
            pisarze.signal();
        }
        zamek.unlock();
    }

    public void chce_pisac() throws InterruptedException {
        zamek.lock();
        if (liczba_pisz > 0 || liczba_czyt > 0) {
            pisarze.await();
        }
        liczba_pisz++;
        zamek.unlock();
    }

    public void pisze() throws InterruptedException {
        zamek.lock();
        System.out.println(String.format("Liczba czytelnikow: %d\tLiczba pisarzy: %d\n", liczba_czyt, liczba_pisz));
        zamek.unlock();
        Thread.sleep(500);
    }

    public void koniec_pisania() {
        zamek.lock();
        liczba_pisz--;
        if (zamek.hasWaiters(czytelnicy)) {
            czytelnicy.signal();
        } else {
            pisarze.signal();
        }
        zamek.unlock();
    }
}
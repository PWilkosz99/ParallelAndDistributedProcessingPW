public class ObrazT1 extends Thread {
    private Obraz obraz;
    private int charNumber;

    ObrazT1(Obraz o, int c) {
        obraz = o;
        charNumber = c;
    }

    public void run(){
        obraz.calculate_histogram(charNumber);
    }
}

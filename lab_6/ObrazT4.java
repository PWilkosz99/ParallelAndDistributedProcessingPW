public class ObrazT4 implements Runnable {
    Obraz obraz;
    int row;
    int col;
    int id;
    int size;
    int colsPerRow;

    ObrazT4(Obraz o, int r, int c, int size, int id) {
        obraz = o;
        row = r;
        col = c;
        this.id = id;
        this.size = size;
        colsPerRow = size;
    }

    @Override
    public void run() {
        if (id < size) {
            obraz.calculate_histogram_squere(row * size, col * size, row * (size + 1), col * (size + 1), id);
        } else {
            obraz.calculate_histogram_squere(row * size * (id / size), col * size * (id / size),
                    row * (size + 1) * (id / size), col * (size + 1) * (id / size), id);
        }
        obraz.syncArrays2(id);
    }

}

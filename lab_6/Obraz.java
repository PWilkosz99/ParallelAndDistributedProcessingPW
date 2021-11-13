import java.util.Random;

class Obraz {

	private int size_n;
	private int size_m;
	private char[][] tab;
	private char[] tab_symb;
	private int[] histogram;
	private int[] hist_parallel;

	public Obraz(int n, int m) {

		this.size_n = n;
		this.size_m = m;
		tab = new char[n][m];
		tab_symb = new char[94];

		final Random random = new Random();

		// for general case where symbols could be not just integers
		for (int k = 0; k < 94; k++) {
			tab_symb[k] = (char) (k + 33); // substitute symbols
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				tab[i][j] = tab_symb[random.nextInt(94)]; // ascii 33-127
				// tab[i][j] = (char)(random.nextInt(94)+33); // ascii 33-127
				System.out.print(tab[i][j] + " ");
			}
			System.out.print("\n");
		}
		System.out.print("\n\n");
		histogram = new int[94];
		hist_parallel = new int[94];
		clear_histogram();
	}

	public void clear_histogram() {
		for (int i = 0; i < 94; i++) {
			histogram[i] = 0;
			hist_parallel[i] = 0;
		}
	}

	public void calculate_histogram() { // seq
		for (int i = 0; i < size_n; i++) {
			for (int j = 0; j < size_m; j++) {
				// optymalna wersja obliczania histogramu, wykorzystujÄca fakt, Ĺźe symbole w
				// tablicy
				// moĹźna przeksztaĹciÄ w indeksy tablicy histogramu
				// histogram[(int)tab[i][j]-33]++;

				// wersja bardziej ogĂłlna dla tablicy symboli nie utoĹźsamianych z indeksami
				// tylko dla tej wersji sensowne jest zrĂłwnoleglenie w dziedzinie zbioru
				// znakĂłw ASCII
				for (int k = 0; k < 94; k++) {
					if (tab[i][j] == tab_symb[k])
						histogram[k]++;
					// if(tab[i][j] == (char)(k+33)) histogram[k]++;
				}
			}
		}
	}

	public void calculate_histogram(int c) {
		for (int i = 0; i < size_n; i++) {
			for (int j = 0; j < size_m; j++) {
				if (tab[i][j] == tab_symb[c])
					hist_parallel[c]++;
			}
		}
	}

	public void print_histogram() {
		for (int i = 0; i < 94; i++) {
			System.out.print(tab_symb[i] + " " + histogram[i] + "\n");
			// System.out.print((char)(i+33)+" "+histogram[i]+"\n");
		}
	}

}
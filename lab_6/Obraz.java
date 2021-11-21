import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;

class Obraz {

	private int size_n;
	private int size_m;
	private char[][] tab;
	private char[] tab_symb;
	private int[] histogram;
	private int[] hist_parallel;
	private int[][] tTab;
	private int[][] bTab;
	public static int charCount = 10;

	public Obraz(int n, int m) {

		this.size_n = n;
		this.size_m = m;
		tab = new char[n][m];
		tab_symb = new char[charCount];

		final Random random = new Random();

		// for general case where symbols could be not just integers
		for (int k = 0; k < charCount; k++) {
			tab_symb[k] = (char) (k + 33); // substitute symbols
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				tab[i][j] = tab_symb[random.nextInt(charCount)]; // ascii 33-127
				// tab[i][j] = (char)(random.nextInt(charCount)+33); // ascii 33-127
				System.out.print(tab[i][j] + " ");
			}
			System.out.print("\n");
		}
		System.out.print("\n\n");
		histogram = new int[charCount];
		hist_parallel = new int[charCount];
		tTab=new int[size_n][];
		for (int i = 0; i < n; i++) {
			tTab[i] = new int[charCount];
		}
		clear_histogram();
	}

	public Obraz(int n, int m, int ids) {
		this(n, m);
		tTab=new int[ids][];
		for (int i = 0; i < ids; i++) {
			tTab[i] = new int[charCount];
		}
	}

	public void compareResults() {
		boolean error = false;
		for (int i = 0; i < hist_parallel.length; i++) {
			if (histogram[i] != hist_parallel[i]) {
				error = true;
				break;
			}
		}
		if (error) {
			System.out.println("Błąd obliczeń równoległych");
		} else {
			System.out.println("Obliczenia prawidłowe");
		}
	}

	public void clear_histogram() {
		for (int i = 0; i < charCount; i++) {
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
				for (int k = 0; k < charCount; k++) {
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

	public void calculate_histogram(int a, int b) { // par - block decomp(chars)
		for (int c = a; c < b; c++) {
			for (int i = 0; i < size_n; i++) {
				for (int j = 0; j < size_m; j++) {
					if (tab[i][j] == tab_symb[c])
						hist_parallel[c]++;
				}
			}
		}
	}

	public void calculate_histogram2(int r) { // par - block decomp(row)
		for (int j = 0; j < size_m; j++) {
			for (int k = 0; k < charCount; k++) {
				if (tab[r][j] == tab_symb[k]) {
					tTab[r][k]++;
				}
			}
		}
	}

	public void calculate_histogram_squere(int r, int c, int re, int ce, int id) {
		for (int rw = r; rw < re; rw++) {
			for (int cl = c; cl < ce; cl++) {
				for (int i = 0; i < charCount; i++) {
					if (tab[rw][cl] == tab_symb[i]) {
						bTab[id][i]++;
					}
				}
			}
		}
	}

	public synchronized void syncArrays(int r){
		for (int i = 0; i < charCount; i++) {
			hist_parallel[i]+=tTab[r][i];
		}
	}

	public synchronized void syncArrays2(int id){
		for (int i = 0; i < charCount; i++) {
			hist_parallel[i]+=bTab[id][i];
		}
	}

	public void print_histogram() {
		for (int i = 0; i < charCount; i++) {
			System.out.print("T" + Thread.currentThread().getId() + " " + tab_symb[i] + " " + histogram[i] + "\n");
		}
	}

	public void print_histogramp() {
		for (int i = 0; i < charCount; i++) {
			System.out.print("T" + Thread.currentThread().getId() + " " + tab_symb[i] + " " + hist_parallel[i] + "\n");
		}
	}

	public synchronized void print_histogram(int c) {
		System.out.print("T" + Thread.currentThread().getId() + " " + tab_symb[c] + " : ");
		for (int i = 0; i < hist_parallel[c]; i++) {
			System.out.print("=");
		}
		System.out.println();
	}
	
	public synchronized void print_histogram(int a, int b) {
		for (int c = a; c < b; c++) {
			System.out.print("T" + Thread.currentThread().getId() + " " + tab_symb[c] + " : ");
			for (int i = 0; i < hist_parallel[c]; i++) {
				System.out.print("=");
			}
			System.out.println();
		}
	}
}
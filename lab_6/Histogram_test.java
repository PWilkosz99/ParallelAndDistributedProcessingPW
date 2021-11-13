import java.util.Scanner;

class Histogram_test {

	public static void main(String[] args) {

		Scanner scanner = new Scanner(System.in);

		System.out.println("Set image size: n (#rows), m(#kolumns)");
		int n = scanner.nextInt();
		int m = scanner.nextInt();
		Obraz obraz_1 = new Obraz(n, m);
		scanner.close();

		// Sequentially

		obraz_1.calculate_histogram();
		// obraz_1.print_histogram();

		// Variant 1 : extend Threads

		int diffCharCount = 94;

		ObrazT1[] obrazT1s = new ObrazT1[diffCharCount];
		for (int i = 0; i < obrazT1s.length; i++) {
			(obrazT1s[i] = new ObrazT1(obraz_1, i)).start();
		}
		;

		for (int i = 0; i < obrazT1s.length; i++) {
			try {
				obrazT1s[i].join();
			} catch (InterruptedException e) {
			}
		}

		obraz_1.print_histogram();

		// System.out.println("Set number of threads");
		// int num_threads = scanner.nextInt();

		// Watek[] NewThr = new Watek[num_threads];

		// for (int i = 0; i < num_threads; i++) {
		// (NewThr[i] = new Watek(...,obraz_1)).start();
		// }

		// for (int i = 0; i < num_threads; i++) {
		// try {
		// NewThr[i].join();
		// } catch (InterruptedException e) {}
		// }

	}

}

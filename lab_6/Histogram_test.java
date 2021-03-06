import java.util.Scanner;

class Histogram_test {

	public static void main(String[] args) {

		Scanner scanner = new Scanner(System.in);
		int ps=5;

		System.out.println("Set image size: n (#rows), m(#kolumns)");
		int n = scanner.nextInt();
		int m = scanner.nextInt();
		Obraz obraz_1 = new Obraz(n, m);
		Obraz obraz_2 = new Obraz(n, m);
		Obraz obraz_3 = new Obraz(n, m);
		Obraz obraz_4 = new Obraz(n, m, ps);
		scanner.close();

		// Sequentially

		System.out.println("\n\t Wariant 1: \t\n ");

		obraz_1.calculate_histogram();
		obraz_1.print_histogram();

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

		obraz_1.compareResults();

		// Variant 2 : implements Runnable - block by char number

		System.out.println("\n\t Wariant 2: \t\n ");

		obraz_2.calculate_histogram();

		int p = 10;
		int nt = (int) Math.ceil((float) diffCharCount / p);
		int a, b;
		ObrazT2[] obrazT2s = new ObrazT2[p];
		Thread[] threadContainer = new Thread[p];

		for (int i = 0; i < obrazT2s.length; i++) {
			a = nt * i;
			b = nt * (i + 1);
			if (b > diffCharCount)
				b = diffCharCount;
			threadContainer[i] = (new Thread(new ObrazT2(obraz_2, a, b)));
			threadContainer[i].start();
		}

		for (int i = 0; i < obrazT2s.length; i++) {
			try {
				threadContainer[i].join();
			} catch (InterruptedException e) {
			}
		}

		obraz_2.compareResults();

		// Variant 3 : implements Runnable - block by rows

		System.out.println("\n\t Wariant 3: \t\n ");

		obraz_3.calculate_histogram();
		//obraz_3.print_histogram();

		ObrazT3[] obrazT3s = new ObrazT3[n];
		Thread[] threadContainer2 = new Thread[n];

		for (int i = 0; i < obrazT3s.length; i++) {
			threadContainer2[i] = (new Thread(new ObrazT3(obraz_3, i)));
			threadContainer2[i].start();
		}

		for (int i = 0; i < obrazT3s.length; i++) {
			try {
				threadContainer2[i].join();
			} catch (InterruptedException e) {
			}
		}

		obraz_3.print_histogram();
		obraz_3.compareResults();

		// Variant 4 : implements Runnable - block squeres

		System.out.println("\n\t Wariant 4: \t\n ");

		obraz_4.calculate_histogram();
		//obraz_4.print_histogram();

		ObrazT4[] obrazT4s = new ObrazT4[ps*ps];
		Thread[] threadContainer3 = new Thread[ps*ps];

		for (int i = 0; i < ps*ps; i++) {
			threadContainer3[i] = (new Thread(new ObrazT4(obraz_4, n,m, ps, i)));
			threadContainer3[i].start();
		}

		for (int i = 0; i < ps*ps; i++) {
			try {
				threadContainer3[i].join();
			} catch (InterruptedException e) {
			}
		}

		obraz_4.print_histogram();
		obraz_4.compareResults();
	}

}

import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class Histogram_test {

	public static void main(String[] args) {

		Scanner scanner = new Scanner(System.in);

		System.out.println("Set image size: n (#rows), m(#kolumns)");
		int n = scanner.nextInt();
		int m = scanner.nextInt();
		Obraz obraz_1 = new Obraz(n, m);
		Obraz obraz_2 = new Obraz(n, m);
		Obraz obraz_3 = new Obraz(n, m);
		scanner.close();

		// Sequentially

		System.out.println("\n\t Wariant 1: \t\n ");

		obraz_1.calculate_histogram();
		obraz_1.print_histogram();

		// Variant 1 : extend Threads

		int diffCharCount = 94;
		int threadCount = 10;

		ExecutorService executorService = Executors.newFixedThreadPool(threadCount);
		for (int i = 0; i < diffCharCount; i++) {
			executorService.execute(new ObrazT1(obraz_1, i));
		}

		executorService.shutdown();
		obraz_1.compareResults();

		// Variant 2 : implements Runnable - by char number

		System.out.println("\n\t Wariant 2: \t\n ");

		obraz_2.calculate_histogram();

		int p = 10;
		int nt = (int) Math.ceil((float) diffCharCount / p);
		int a, b;
		// ObrazT2[] obrazT2s = new ObrazT2[p];
		Thread[] threadContainer = new Thread[p];

		ExecutorService executorService2 = Executors.newFixedThreadPool(threadCount);

		for (int i = 0; i < p; i++) {
			a = nt * i;
			b = nt * (i + 1);
			if (b > diffCharCount)
				b = diffCharCount;
			executorService2.execute(new ObrazT2(obraz_2, a, b));
		}

		executorService2.shutdown();

		obraz_2.print_histogram();
		obraz_2.compareResults();

		// // Variant 3 : implements Runnable - block by rows

		System.out.println("\n\t Wariant 3: \t\n ");

		obraz_3.calculate_histogram();
		// obraz_3.print_histogram();

		ExecutorService executorService3 = Executors.newFixedThreadPool(threadCount);

		for (int i = 0; i < n; i++) {
			executorService3.execute(new ObrazT3(obraz_3, i));
		}
		executorService3.shutdown();

		obraz_3.print_histogram();
		obraz_3.compareResults();
	}

}

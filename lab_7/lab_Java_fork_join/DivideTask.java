import java.util.Arrays;
import java.util.concurrent.*;

class DivideTask extends RecursiveTask<int[]> {

    int[] arrayToDivide;

    public DivideTask(int[] arrayToDivide) {
        this.arrayToDivide = arrayToDivide;
    }

    protected int[] compute() {
  
        // MergeSort

        // .......
        int arrSize = arrayToDivide.length;
        int arrHalf = arrSize/2;
   
        DivideTask task1 = new DivideTask(Arrays.copyOfRange(arrayToDivide, 0, arrHalf));
        DivideTask task2 = new DivideTask(Arrays.copyOfRange(arrayToDivide, arrHalf+1, arrSize));
  
        // .......
  
        //Wait for results from both tasks
        int[] tab1 = task1.join();
        int[] tab2 = task2.join();
   
        return scal_tab(tab1, tab2, scal_tab);
  
    }

    private void scal_tab(int[] tab1, int[] tab2, int[] scal_tab) {

        int i = 0, j = 0, k = 0;

        while ((i < tab1.length) && (j < tab2.length)) {

            if (tab1[i] < tab2[j]) {
                scal_tab[k] = tab1[i++];
            } else {
                scal_tab[k] = tab2[j++];
            }

            k++;
        }

        if (i == tab1.length) {

            for (int a = j; a < tab2.length; a++) {
                scal_tab[k++] = tab2[a];
            }

        } else {

            for (int a = i; a < tab1.length; a++) {
                scal_tab[k++] = tab1[a];
            }

        }
    }

}
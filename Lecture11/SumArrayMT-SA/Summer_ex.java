
/**
 * Utility class containing a method to sum up an integer array using
 * threads.
 */
public class Summer {
    
    /**
     * Returns the sum of all the values in an array.  Internally
     * it spawns two threads to sum up each half of the array.
     * Note how the body of this method (a third thread!) waits for
     * the two threads to complete using the Thread.join method.
     */
    public static int sum(int[] data) throws InterruptedException {
        // Fill Code Here
        return 0;
    }
    
    private static class RangeSummer extends Thread {
        private int[] data;
        private int low;
        private int high;
        private int value = 0;
        
        public RangeSummer(int[] data, int low, int high) {
            this.data = data;
            this.low = low;
            this.high = high;
        }
        
        public int getValue() { return value; }
        
        public void run() {
            for (int i = low; i < high; i++) {
                value += data[i];
            }
        }
    }
    
    /**
     * Makes calls to sum just for fun.
     */
    public static void main(String[] args) throws Exception {
        assert sum(new int[]{1,2,3,4,5,6,7,8,9,10}) == 55;
        assert sum(new int[]{100, 200, 300, 400}) == 1000;
        assert sum(new int[]{}) == 0;
        
        int[] a = new int[20000];
        java.util.Arrays.fill(a, 7);
        assert sum(a) == 140000;
        System.out.println("All tests passed");
    }
}
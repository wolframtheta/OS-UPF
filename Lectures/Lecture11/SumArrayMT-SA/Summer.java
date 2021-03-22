
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
        RangeSummer r1 = new RangeSummer(data, 0, data.length/2);
        RangeSummer r2 = new RangeSummer(data, data.length/2, data.length);
        r1.start();
        r2.start();
        r1.join();
        r2.join();
        return r1.getValue() + r2.getValue();
    }
    
    /**
     * A thread that sums up a slice of an integer array, specifically
     * the slice [low, high>.  An empty slice will have a zero sum.  You
     * can call getValue() at any time to get the value that has been
     * computed so far.  Calling getValue() after the run() method
     * completes will return the correct sum of the slice.
     */
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
        
        public int getValue() {
            return value;
        }
        
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
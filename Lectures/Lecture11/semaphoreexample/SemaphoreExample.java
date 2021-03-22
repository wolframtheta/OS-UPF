package semaphoreexample;

import java.util.concurrent.Semaphore;
import java.util.logging.Level;
import java.util.logging.Logger;


public class SemaphoreExample {

    static Semaphore mutex = new Semaphore (1); 
    static Semaphore countIn = new Semaphore (3); 
    static Thread[] th;
    
    public static void main(String[] args) {

        try {
            mutex.acquire();
            // Critical Section
            mutex.release();
            
            int N = 3 + (int) (Math.random() * 5);
            th = new Thread[N];
            System.out.println("I'm creating " + N + " threads.");
            
            for(int i=0;i<N;i++) {
                th[i] = new Thread() {
                   public void run() {
                     try {
                        countIn.acquire();
                        System.out.println("I'm in!");
                        Thread.sleep( 400 + (int) (1000 * Math.random()) );
                        System.out.println("I'm finished!");
                        countIn.release();
                     } catch (InterruptedException ex) {}
                   }
                };
            }    
            for(int i=0;i<N;i++) th[i].start(); 
            for(int i=0;i<N;i++) th[i].join();  
        } 
        catch (InterruptedException ex) {}
        
    }
}




public class Worker extends Thread {
	private Barrier barrier;

	public Worker( Barrier b ) {
		barrier = b;
	}

	public void run() {
		System.out.println( "A" );
		barrier.waitForOthers();
		System.out.println( "B" );
	}
}

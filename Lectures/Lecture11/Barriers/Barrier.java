
public class Barrier {
	private int count; // number of threads to wait for

	public Barrier( int c ) {
		count = c;
	}

	public synchronized void waitForOthers() {
		--count;

		if (count <= 0) notifyAll();

		while ( count > 0 ) {
			try {
				wait();
			}
			catch (InterruptedException ie) {}
		}
	}
}


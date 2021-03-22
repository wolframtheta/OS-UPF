
public class TestBarrier {
	public static void main( String[] args ) {
		int N = 10;
		Barrier barrier = new Barrier( N );
		Thread threads[] = new Thread[N];

		for ( int i = 0; i < N; ++i ) {
			threads[i] = new Worker( barrier );
			threads[i].start();
		}

		for ( int i = 0; i < N; ++i ) {
			try {
				threads[i].join();
			} catch ( InterruptedException e ) {}
		}
	}
}

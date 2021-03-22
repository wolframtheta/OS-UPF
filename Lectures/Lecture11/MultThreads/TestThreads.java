
public class TestThreads {
	public static void main( String[] args ) {
		MyArray array = new ElementArray( 100 );
		MultThread thread1 = new MultThread( false, array );
		MultThread thread2 = new MultThread( true , array );

		thread1.start();
		thread2.start();

		try {
			thread1.join();
			thread2.join();
		} catch( InterruptedException e ) {}

		array.print();
	}
}

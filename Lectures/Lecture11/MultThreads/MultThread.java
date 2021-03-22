
public class MultThread extends Thread {
	private boolean reverse;
	private MyArray array;
	
	public MultThread( boolean r, MyArray a ) {
		reverse = r;
		array = a;
	}

	public void run() {
		for ( int i = 0; !reverse && i < array.length(); ++i )
			array.mult( i, i );
		for ( int i = array.length() - 1; reverse && i >= 0; --i )
			array.mult( i, i );
	}
}


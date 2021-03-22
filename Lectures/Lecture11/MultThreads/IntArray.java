
public class IntArray extends MyArray {
	private int array[];

	public IntArray( int capacity ) {
		array = new int[capacity];
		for ( int i = 0; i < capacity; ++i )
			array[i] = 1;
	}

	public int length() {
		return array.length;
	}

	public void mult( int i, int n ) {
		synchronized( this ) {
			array[i] *= n;
		}
	}

	public void print() {
		for ( int i = 0; i < length(); ++i )
			System.out.println( i + ": " + array[i] );
	}
}

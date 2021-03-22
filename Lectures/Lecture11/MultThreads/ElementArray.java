
public class ElementArray extends MyArray {
	private MyElement array[];

	public ElementArray( int capacity ) {
		array = new MyElement[capacity];
		for ( int i = 0; i < capacity; ++i )
			array[i] = new MyElement( 1 );
	}

	public int length() {
		return array.length;
	}

	public void mult( int i, int n ) {
		synchronized( array[i] ) {
			array[i].mult( n );
		}
	}

	public void print() {
		for ( int i = 0; i < length(); ++i )
			System.out.println( i + ": " + array[i].getValue() );
	}
}

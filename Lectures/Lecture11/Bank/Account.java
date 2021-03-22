
public class Account
{

	int id;
	float balance;

	public void deposit(float amount) {
		balance += amount;
	}

	public boolean withdraw(float amount) {
		boolean ok = balance - amount >= 0; 
		if(ok) balance -= amount;
		return ok;
	}


}

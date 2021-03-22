

public class Bank
{
	Account[] bank;
   

	public boolean transfer(Account from, Account to, float amount) {
		Account acc_lock_1 = from;
		Account acc_lock_2 = to;
		if (from.id > to.id) {
			acc_lock_1 = to;
			acc_lock_2 = from;
		}

		boolean ok;
		synchronized(acc_lock_1) {
			synchronized(acc_lock_2) {
				ok = from.withdraw(amount);
				if(ok) to.deposit(amount);
			}
		}
		return ok;
	}

	void do_transfers() {
		for (int i = 0; i < 1000; i++) {
			int j = 0; // add random number
			int k = 1; // add random number
			transfer(bank[j],bank[k],1);
		}

  	
	}


	Bank() {
		int N = 10;
  		bank = new Account[N];
  		for (int i = 0; i < N; i++) {
  			bank[i].balance = 100;	
        }
	}

  	public static void main(String[] args) throws Exception {
  		Bank my_bank;

  		my_bank.do_transfers();
  	}

}

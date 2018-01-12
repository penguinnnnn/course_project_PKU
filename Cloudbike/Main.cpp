#include "Typedef.h"

int main()
{
	connect_mysql(_HOST, _USER, _PWD, _NAME);
	welcome_in();

	User user;
	int id_order = 0;
	short logged = 0;
	string account_number;
	while (logged == 0)
	{
		cout << "Account number (or `create`):";
		cin >> account_number;
		cout << endl;
		if (account_number == "root")
			logged = manager_log_in();
		else if (account_number == "create")
				logged = user_sign_up(user);
			else
				logged = user_log_in(user, account_number);
	}
	
	if (logged == 1)
	{
		welcome_do(user.alias);
		string cmd;
		bool quit = false;
		while (cin >> cmd)
		{
			cout << endl;
			switch (encode(cmd))
			{
				case 1: help(); break;
				case 2: quit = true; break;
				case 3: user.find(); break;
				case 4: id_order = user.ride(id_order); break;
				case 5: user.stop(id_order); break;
				case 6: user.info(); break;
				case 7: user.edit(); break;
				case 8: user.chag(); break;
				case 9: user.ordr(); break;
				default: inva(); break;
			}
			if (quit) break;
			cout << "Waiting for command......" << endl << endl;
		}
		welcome_out(user.alias);
	}

	if (logged == -1)
	{
		cout << "Welcome! Administrator!" << endl;
		cout << "What do you want to do?" << endl << endl;
		cout << "Type `h` for information of all commands." << endl << endl;
		string cmd;
		bool quit = false;
		while (cin >> cmd)
		{
			cout << endl;
			switch (encode_admin(cmd))
			{
				case 1: help_admin(); break;
				case 2: manager_update(); break;
				case 3: manager_delete(); break;
				case 4: manager_insert(); break;
				case 5: manager_select(); break;
				case 6: manager_others(); break;
				case 7: quit = true; break;
				default: inva(); break;
			}
			if (quit) break;
			cout << "Waiting for command......" << endl << endl;
		}
	}
	
	disconnect();
	return 0;
}


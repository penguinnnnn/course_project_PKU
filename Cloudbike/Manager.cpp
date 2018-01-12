#include "Typedef.h"

int manager_log_in()
{
	string pwd;
	cout << "Password:";
	cin >> pwd;
	cout << endl;
	if (pwd == "mysql") return -1;
	cout << "Fail to log in the manager account!" << endl << endl;
	return 0;
}

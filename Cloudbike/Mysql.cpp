#include "Typedef.h"

MYSQL conn;			// The connection
MYSQL_RES *sql_res;	// Store the query result
MYSQL_ROW sql_row;	// Get row in result

void connect_mysql(const char* host, const char* user, const char* pwd, const char* db)
{
	// Initialize the struct
	mysql_init(&conn);

	// Connect mysql
	if (!mysql_real_connect(&conn, host, user, pwd, db, 0, NULL, CLIENT_MULTI_STATEMENTS))
	{
		fprintf(stderr, "Connection failed!\n");
		if (mysql_errno(&conn))
		{
			fprintf(stderr, "Connection error %d: %s\n", mysql_errno(&conn), mysql_error(&conn));
		}
		exit(EXIT_FAILURE);
	}

	return;
}

void disconnect()
{
	mysql_close(&conn);
	return;
}

// Correct = 1, incorrect = 0
bool check_user(string account, string pwd)
{
	string _query = "SELECT `pwd_user` from `user` WHERE `account_number` = \"";
	_query += account;
	_query += "\"";

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Get the results
		sql_row = mysql_fetch_row(sql_res);
		// Check password
		bool flag = strcmp(sql_row[0], pwd.c_str());

		mysql_free_result(sql_res);
		return !flag;
	}
}

// Exist = 1, not exist = 0
bool check_user_exist(string account)
{
	string _query = "SELECT `pwd_user` from `user` WHERE `account_number` = \"";
	_query += account;
	_query += "\"";

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Check if exists
		if (mysql_fetch_row(sql_res) == NULL)
		{
			mysql_free_result(sql_res);
			return 0;
		}
		mysql_free_result(sql_res);
		return 1;
	}
}

// Exist = 1, not exist = 0
bool check_bike_exist(int ID_bike)
{
	// For changing type to char*
	char tmp[64];

	string _query = "SELECT `pwd_bike` from `bike` WHERE `ID_bike` = ";
	sprintf(tmp, "%d", ID_bike);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Check if exists
		if (mysql_fetch_row(sql_res) == NULL)
		{
			mysql_free_result(sql_res);
			return 0;
		}
		mysql_free_result(sql_res);
		return 1;
	}
}

// Add this new user to database
bool add_to_db(User& user)
{
	// For changing type to char*
	char tmp[64];

	string _query = "INSERT INTO `user` (`account_number`, `pwd_user`, `alias`, `balance`, `job`, `gender`, `state_user`) VALUES (\"";
	_query += user.account_number;
	_query += "\", \"";
	_query += user.password;
	_query += "\", \"";
	_query += user.alias;
	_query += "\", ";
	sprintf(tmp, "%.02f", user.balance);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ", \"";
	_query += user.job;
	_query += "\", \"";
	_query += user.gender;
	_query += "\", ";
	sprintf(tmp, "%d", user.state_user);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ")";

	// Insert values
	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}

	return true;
}

bool get_info_db(User& user)
{
	string _query = "SELECT `ID_user`, `alias`, `balance`, `job`, `gender`, `state_user` FROM `user` WHERE `account_number` = \"";
	_query += user.account_number;
	_query += "\"";

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Get the results
		sql_row = mysql_fetch_row(sql_res);

		user.id_user = atoi(sql_row[0]);
		user.alias = sql_row[1];
		user.balance = (float)atof(sql_row[2]);
		user.job = sql_row[3];
		user.gender = sql_row[4];
		user.state_user = (short)atoi(sql_row[5]);
	}

	mysql_free_result(sql_res);
	return true;
}

bool update_db(User& user)
{
	// For changing type to char*
	char tmp[64];

	string _query = "UPDATE `user` SET `alias` = \"";
	_query += user.alias;
	_query += "\", `pwd_user` = \"";
	_query += user.password;
	_query += "\", `gender` = \"";
	_query += user.gender;
	_query += "\", `job` = \"";
	_query += user.job;
	_query += "\", `balance` = ";
	sprintf(tmp, "%.02f", user.balance);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ", `state_user` = ";
	sprintf(tmp, "%d", user.state_user);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += " WHERE `ID_user` = ";
	sprintf(tmp, "%d", user.id_user);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	// Update values
	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	return true;
}

void find_area(double max_long, double max_lat, double min_long, double min_lat, double longitude, double latitude, int& NUM, string* bikeid, string* bikelong, string* bikelat)
{
	// For changing type to char*
	char tmp[64];

	string _query = "SELECT `ID_bike`, `long`, `lat` FROM `bike` WHERE `long` < ";
	sprintf(tmp, "%.6f", max_long);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += " AND `long` > ";
	sprintf(tmp, "%.6f", min_long);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += " AND `lat` < ";
	sprintf(tmp, "%.6f", max_lat);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += " AND `lat` > ";
	sprintf(tmp, "%.6f", min_lat);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Get the results
		sql_row = mysql_fetch_row(sql_res);

		if (sql_row == NULL)
		{
			cout << "Sorry! There is no any bike nearby." << endl << endl;
		}
		else
		{
			int i = 0;
			bikeid[i] = sql_row[0];
			bikelong[i] = sql_row[1];
			bikelat[i] = sql_row[2];
			i++;
			cout << "Bike " << i << ": " << sql_row[0];
			cout << ", go north " << (atof(sql_row[1]) - longitude) * 340000;
			cout << ", go east " << (atof(sql_row[2]) - latitude) * 444000;
			cout << endl << endl;
			while ((sql_row = mysql_fetch_row(sql_res)) != NULL)
			{
				bikeid[i] = sql_row[0];
				bikelong[i] = sql_row[1];
				bikelat[i] = sql_row[2];
				i ++;
				cout << "Bike " << i << ": " << sql_row[0];
				cout << ", go north " << (atof(sql_row[1]) - longitude) * 340000;
				cout << ", go east " << (atof(sql_row[2]) - latitude) * 444000;
				cout << endl << endl;
			}
			NUM = i;
		}

		mysql_free_result(sql_res);
	}
	return;
}

void find_order(int ID_order)
{
	char tmp[64];

	string _query = "SELECT ID_user, ID_bike, loca_start, loca_finish, time_start, time_finish, long_start, long_finish, lat_start, lat_finish, cost, distance, time_total FROM `order` WHERE ID_order = ";
	sprintf(tmp, "%d", ID_order);
	_query += tmp;

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Get the results
		sql_row = mysql_fetch_row(sql_res);

		cout << "             id_order: " << tmp << endl;
		cout << "              id_user: " << sql_row[0] << endl;
		cout << "              id_bike: " << sql_row[1] << endl;
		cout << "           start time: " << sql_row[4] << endl;
		cout << "          finish time: " << sql_row[5] << endl;
		cout << "      start longitude: " << sql_row[6] << endl;
		cout << "       start latitude: " << sql_row[8] << endl;
		cout << "     finish longitude: " << sql_row[7] << endl;
		cout << "      finish latitude: " << sql_row[9] << endl;
		cout << "           total time: " << sql_row[0] << endl;
		cout << "             distance: " << sql_row[1] << endl;
		cout << "                 cost: " << sql_row[4] << endl;

		int l_s(atoi(sql_row[2]));
		int l_f(atoi(sql_row[3]));
		mysql_free_result(sql_res);

		cout << "       start district: " << "Haidian District" << endl;
		cout << "      finish district: " << "Haidian District" << endl;
		cout << "  start street in row: " << get_street_r(l_s) << endl;
		cout << " finish street in row: " << get_street_r(l_f) << endl;
		cout << "  start street in col: " << get_street_c(l_s) << endl;
		cout << " finish street in col: " << get_street_c(l_f) << endl;
		cout << endl;
	}

	return;
}

int create_order(int id_user, int id_bike, int id_location, double longitude, double latitude)
{
	char tmp[64];
	string time_now;
	int id_order;

	string _query = "INSERT INTO `order` (ID_user, ID_bike, state_order, time_start, time_finish, loca_start, loca_finish, long_start, long_finish, lat_start, lat_finish, cost, distance, time_total) VALUES (";
	sprintf(tmp, "%d", id_user);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ", ";
	sprintf(tmp, "%d", id_bike);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ", 1, \"";
	time_t t = time(0);
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime(&t));
	time_now = tmp;
	_query += tmp;
	_query += "\", \"";
	_query += tmp;
	_query += "\", ";
	sprintf(tmp, "%d", id_location);
	_query += tmp;
	_query += ", ";
	_query += tmp;
	_query += ", ";
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%.6f", longitude);
	_query += tmp;
	_query += ", ";
	_query += tmp;
	_query += ", ";
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%.6f", latitude);
	_query += tmp;
	_query += ", ";
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ", 0, 0, 0)";

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}

	_query = "UPDATE `bike` SET `state_bike` = 1 WHERE `ID_bike` = ";
	sprintf(tmp, "%d", id_bike);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}

	_query = "SELECT ID_order FROM `order` WHERE time_start = \"";
	_query += time_now;
	_query += "\" AND ID_user = ";
	sprintf(tmp, "%d", id_user);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += " AND ID_bike = ";
	sprintf(tmp, "%d", id_bike);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Get the results
		sql_row = mysql_fetch_row(sql_res);

		id_order = atoi(sql_row[0]);
		mysql_free_result(sql_res);
	}
	return id_order;
}

float finish_order(int id_order, int id_location, double longitude, double latitude)
{
	char tmp[64];
	int time_second;
	float price;
	float ret_cost;
	float ret_distance;
	string id_bike;

	string _query = "UPDATE `order` SET state_order = 0, loca_finish = ";
	sprintf(tmp, "%d", id_location);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ", long_finish = ";
	sprintf(tmp, "%.6f", longitude);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ", long_finish = ";
	sprintf(tmp, "%.6f", latitude);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ", time_finish = \"";
	time_t t = time(0);
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime(&t));
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += "\"";
	_query += " WHERE ID_order = ";
	sprintf(tmp, "%d", id_order);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}

	_query = "SELECT (UNIX_TIMESTAMP(time_finish) - UNIX_TIMESTAMP(time_start)), long_start, lat_start, ID_bike FROM `order` WHERE ID_order = ";
	sprintf(tmp, "%d", id_order);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Get the results
		sql_row = mysql_fetch_row(sql_res);
		
		time_second = atoi(sql_row[0]);
		ret_distance = (float)abs(atof(sql_row[1]) - longitude) * 340000 + (float)abs(atof(sql_row[2]) - latitude) * 444000;

		id_bike = sql_row[3];
		mysql_free_result(sql_res);
	}


	_query = "UPDATE `bike` SET `state_bike` = 0 WHERE `ID_bike` = ";
	_query += id_bike;

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}

	_query = "SELECT price FROM `bike` WHERE ID_bike = ";
	_query += id_bike;

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Get the results
		sql_row = mysql_fetch_row(sql_res);

		price = (float)atof(sql_row[0]);
		ret_cost = time_second * price / 360;
		mysql_free_result(sql_res);
	}

	_query = "UPDATE `order` SET cost = ";
	sprintf(tmp, "%.2f", ret_cost);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ", distance = ";
	sprintf(tmp, "%.2f", ret_distance);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += ", time_total = ";
	sprintf(tmp, "%d", time_second);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));
	_query += " WHERE ID_order = ";
	sprintf(tmp, "%d", id_order);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}

	return ret_cost;
}

// Input: ID_bike
// Output: Info of bike except pwd to SCREEN
// Return value: pwd
string find_bike(int ID_bike)
{
	string ret, loca_tmp;
	// For changing type to char*
	char tmp[64];

	string _query = "SELECT `type`, `state_bike`, `broken`, `pwd_bike`, `ID_location`, `long`, `lat`, `price` FROM `bike` WHERE `ID_bike` = ";
	sprintf(tmp, "%d", ID_bike);
	_query += tmp;

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Get the results
		sql_row = mysql_fetch_row(sql_res);

		// Save password and location
		ret = sql_row[3];
		loca_tmp = sql_row[4];

		cout << "        id_bike: " << tmp << endl;
		cout << "          price: " << sql_row[7] << endl;
		cout << "           type: " << sql_row[0] << endl;
		cout << "     available?: " << sql_row[1] << endl;
		cout << "        broken?: " << sql_row[2] << endl;
		cout << "      longitude: " << sql_row[5] << endl;
		cout << "       latitude: " << sql_row[6] << endl;

		int l(atoi(sql_row[4]));
		mysql_free_result(sql_res);

		cout << "       district: " << "Haidian District" << endl;
		cout << "  street in row: " << get_street_r(l) << endl;
		cout << "  street in col: " << get_street_c(l) << endl;
		cout << endl;
	}

	return ret;
}

int find_all_order(int id_user, int* order)
{
	// For changing type to char*
	char tmp[64];

	string _query = "SELECT `ID_order`, `time_start`, `time_finish` FROM `order` WHERE `ID_user` = ";
	sprintf(tmp, "%d", id_user);
	_query += tmp;
	_query += " ORDER BY `time_finish` DESC";

	int i = 0;
	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		system("pause");
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		
		// Get the results
		while ((sql_row = mysql_fetch_row(sql_res)) != NULL)
		{
			i++;
			order[i] = atoi(sql_row[0]);
			cout << "Order " << i << ", id = " << sql_row[0];
			cout << "; start time = " << sql_row[1] << ", finish time = " << sql_row[2];
			cout << endl << endl;
		}
		
		mysql_free_result(sql_res);
	}

	return i;
}

void manager_update()
{
	string _query;
	cout << "Please type in SQL query to UPDATE the tables (without ';', end with 'enter'): " << endl << endl;
	cin.ignore();
	getline(cin, _query);

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}

	cout << "Update success!" << endl << endl;
	return;
}

void manager_delete()
{
	char tmp[64];
	string aim;
	int id;

	cout << "Please type in which table you want to delete? " << endl;
	cout << "Type in user/bike" << endl << endl;
	cin >> aim;
	cout << "Please type in the ID: " << endl << endl;
	cin >> id;

	string _query = "DELETE FROM  `order` WHERE ID_";
	_query += aim;
	_query += " = ";
	sprintf(tmp, "%d", id);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}

	_query = "DELETE FROM `";
	_query += aim;
	_query += "` WHERE ID_";
	_query += aim;
	_query += " = ";
	sprintf(tmp, "%d", id);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}

	cout << "Delete success!" << endl << endl;
	return;
}

void manager_select()
{
	char tmp[64];
	string name_col[20];
	string aim;
	int id;

	cout << "Please type in which table you want to delete? " << endl;
	cout << "Type in user/bike/order/location" << endl << endl;
	cin >> aim;
	cout << "Please type in the ID: " << endl << endl;
	cin >> id;

	string _query = "SELECT * FROM `";
	_query += aim;
	_query += "` WHERE ID_";
	_query += aim;
	_query += " = ";
	sprintf(tmp, "%d", id);
	_query += tmp;
	memset(tmp, 0, sizeof(tmp));

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}
	else
	{
		// Save the results
		sql_res = mysql_use_result(&conn);
		// Get the results
		sql_row = mysql_fetch_row(sql_res);

		cout << "Name of Column: Value of Column" << endl << endl;
		MYSQL_FIELD *field_ptr;
		int i = 0;
		while ((field_ptr = mysql_fetch_field(sql_res)) != NULL)
		{
			name_col[i] = field_ptr->name;
			i ++;
		}

		for (int j = 0; j < i; j ++)
		{
			cout << name_col[j] << " : " << sql_row[j] << endl;
		}
		cout << endl;
	}

	mysql_free_result(sql_res);
	return;
}

void manager_insert()
{
	string _query;
	cout << "Please type in SQL query to INSERT INTO the tables (without ';', end with 'enter'): " << endl << endl;
	cin.ignore();
	getline(cin, _query);

	if (mysql_query(&conn, _query.c_str()))
	{
		fprintf(stderr, "%s\n\n", mysql_error(&conn));
		mysql_close(&conn);
		exit(EXIT_FAILURE);
	}

	cout << "Insert success!" << endl << endl;
	return;
}

void manager_others()
{
	cout << "This function includes 4 available functions: " << endl << endl;
	cout << "Type 1 to find out street which have less bikes than average; " << endl;
	cout << "Type 2 to find out users who have most orders during a certain period of time; " << endl;
	cout << "Type 3 to find out total cost of each user who have record in a certain street; " << endl;
	cout << "Type 4 to find out types of bike whose broken rates are larger than average and order them by broken rate and number of orders; " << endl;
	cout << "Type 5 to find out streets who have more broken bikes than normal bikes; " << endl << endl;
	
	string _query;
	int cmd;
	bool quit =  0;
	while (1)
	{
		cin >> cmd;
		switch(cmd)
		{
			case 1:
			{
				_query = "SELECT COUNT(*), `street-r`, `street-c` FROM bike, location WHERE bike.ID_location = location.ID_location GROUP BY `street-r`, `street-c` HAVING COUNT(*) < (SELECT AVG(c) FROM ((SELECT COUNT(*) as c FROM bike, location WHERE bike.ID_location = location.ID_location GROUP BY `street-r`, `street-c`) as num_bike))";
				if (mysql_query(&conn, _query.c_str()))
				{
					fprintf(stderr, "%s\n\n", mysql_error(&conn));
					mysql_close(&conn);
					system("pause");
					exit(EXIT_FAILURE);
				}
				else
				{
					// Save the results
					sql_res = mysql_use_result(&conn);
					
					// Get the results
					cout << "COUNT, street-r, street-c: " << endl;
					while ((sql_row = mysql_fetch_row(sql_res)) != NULL)
					{
						cout << sql_row[0] << ", " << sql_row[1] << ", " << sql_row[2] << endl;
					}
					cout << endl;
					
					mysql_free_result(sql_res);
				}
				quit = 1;
			} break;
			case 2:
			{
				string ts, tf;
				cout << "Please type in start time (YYYY-MM-DD HH-MM-SS): " << endl << endl;
				cin.ignore();
				getline(cin, ts);
				cout << "Please type in finish time (YYYY-MM-DD HH-MM-SS): " << endl << endl;
				getline(cin, tf);

				_query = "SELECT ID_user, COUNT(*) as c FROM `order` WHERE time_start >= \"";
				_query += ts;
				_query += "\" AND time_finish <= \"";
				_query += tf;
				_query += "\" GROUP BY ID_user HAVING c >= all (SELECT COUNT(*) FROM `order` WHERE time_start >= \"";
				_query += ts;
				_query += "\" AND time_finish <= \"";
				_query += tf;
				_query += "\" GROUP BY ID_user)";

				if (mysql_query(&conn, _query.c_str()))
				{
					fprintf(stderr, "%s\n\n", mysql_error(&conn));
					mysql_close(&conn);
					system("pause");
					exit(EXIT_FAILURE);
				}
				else
				{
					// Save the results
					sql_res = mysql_use_result(&conn);
					
					// Get the results
					cout << "ID_user, COUNT: " << endl;
					while ((sql_row = mysql_fetch_row(sql_res)) != NULL)
					{
						cout << sql_row[0] << ", " << sql_row[1] << endl;
					}
					cout << endl;
					
					mysql_free_result(sql_res);
				}
				quit = 1;
			} break;
			case 3:
			{
				string sr, sc;
				cout << "Please type in street in row (xxx street): " << endl << endl;
				cin.ignore();
				getline(cin, sr);
				cout << "Please type in street in column (xxx street): " << endl << endl;
				getline(cin, sc);

				_query = "SELECT ID_user, SUM(cost) FROM `order` WHERE ID_user IN (SELECT ID_user FROM `order`, location WHERE ((loca_start = ID_location AND `street-r` = \"";
				_query += sr;
				_query += "\" AND `street-c` = \"";
				_query += sc;
				_query += "\") OR (loca_finish = ID_location AND `street-r` = \"";
				_query += sr;
				_query += "\" AND `street-c` = \"";
				_query += sc;
				_query += "\"))) GROUP BY ID_user";

				if (mysql_query(&conn, _query.c_str()))
				{
					fprintf(stderr, "%s\n\n", mysql_error(&conn));
					mysql_close(&conn);
					system("pause");
					exit(EXIT_FAILURE);
				}
				else
				{
					// Save the results
					sql_res = mysql_use_result(&conn);
					
					// Get the results
					cout << "ID_user, COUNT: " << endl;
					while ((sql_row = mysql_fetch_row(sql_res)) != NULL)
					{
						cout << sql_row[0] << ", " << fixed << setprecision(2) << atof(sql_row[1]) << endl;
					}
					cout << endl;
					
					mysql_free_result(sql_res);
				}
				quit = 1;
			} break;
			case 4:
			{
				cout << "Group by broken rate: " << endl << endl;
				_query = "SELECT type, AVG(broken) as a FROM bike GROUP BY type HAVING AVG(broken) > (SELECT AVG(broken) FROM bike) ORDER BY a desc";

				if (mysql_query(&conn, _query.c_str()))
				{
					fprintf(stderr, "%s\n\n", mysql_error(&conn));
					mysql_close(&conn);
					system("pause");
					exit(EXIT_FAILURE);
				}
				else
				{
					// Save the results
					sql_res = mysql_use_result(&conn);
					
					// Get the results
					cout << "type, broken rate: " << endl;
					while ((sql_row = mysql_fetch_row(sql_res)) != NULL)
					{
						cout << sql_row[0] << ", " << sql_row[1] << endl;
					}
					cout << endl;
					
					mysql_free_result(sql_res);
				}

				cout << "Group by number of orders: " << endl << endl;
				_query = "SELECT B.type, B.`count(*)` as c FROM (SELECT type, AVG(broken) FROM bike GROUP BY type HAVING AVG(broken) > (SELECT AVG(broken) FROM bike)) as A, (SELECT type, count(*) FROM `order`, bike WHERE `order`.ID_bike = bike.ID_bike GROUP BY type) as B WHERE A.type = B.type ORDER BY c desc";

				if (mysql_query(&conn, _query.c_str()))
				{
					fprintf(stderr, "%s\n\n", mysql_error(&conn));
					mysql_close(&conn);
					system("pause");
					exit(EXIT_FAILURE);
				}
				else
				{
					// Save the results
					sql_res = mysql_use_result(&conn);
					
					// Get the results
					cout << "type, number of orders: " << endl;
					while ((sql_row = mysql_fetch_row(sql_res)) != NULL)
					{
						cout << sql_row[0] << ", " << sql_row[1] << endl;
					}
					cout << endl;
					
					mysql_free_result(sql_res);
				}
				quit = 1;
			} break;
			case 5:
			{
				_query = "SELECT `street-r`, `street-c`, AVG(`broken`) as a FROM bike, location WHERE bike.ID_location = location.ID_location GROUP BY `street-r`, `street-c` HAVING a > 0.5";

				if (mysql_query(&conn, _query.c_str()))
				{
					fprintf(stderr, "%s\n\n", mysql_error(&conn));
					mysql_close(&conn);
					system("pause");
					exit(EXIT_FAILURE);
				}
				else
				{
					// Save the results
					sql_res = mysql_use_result(&conn);
					
					// Get the results
					cout << "street in row, street in column, average broken rate: " << endl;
					while ((sql_row = mysql_fetch_row(sql_res)) != NULL)
					{
						cout << sql_row[0] << ", " << sql_row[1] << ", " << sql_row[2] << endl;
					}
					cout << endl;
					
					mysql_free_result(sql_res);
				}
				quit = 1;
			} break;
			default: cout << "Invalid input. Please type in numbers between 1 to 5." << endl << endl;
		}
		if (quit == 1) break;
	}

	return;
}

int User::ride(int id_order)
{
	if (state_user == 1)
	{
		cout << "You are now riding! Please stop your trip first!" << endl << endl;
		return id_order;
	}
	if (balance < 0.01)
	{
		cout << "Insufficient balance! Please charge first!" << endl << endl;
		return id_order;
	}
	cout << "Please input the bike ID! (Or any else to quit.)" << endl << endl;
	cout << "Bike ID:";
	string bike;
	cin >> bike;
	cout << endl;
	if (atoi(bike.c_str()) == 0) return id_order;
	bool exist = check_bike_exist(atoi(bike.c_str()));
	while (!exist)
	{
		cout << "Bike doesn't exist! Please try again, or input any else to quit." << endl << endl;
		cout << "Bike ID:";
		cin >> bike;
		cout << endl;
		if (atoi(bike.c_str()) == 0) return id_order;
		exist = check_bike_exist(atoi(bike.c_str()));
	}
	string pwd_bike = find_bike(atoi(bike.c_str()));
	string use;
	cout << "Are you sure to use this bike? (y/n)" << endl << endl;
	cin >> use;
	cout << endl;
	if (use == "n") return id_order;
	if (use == "y")
	{
		cout << "Password : " << pwd_bike << endl << endl;
		location = get_location();
		longitude = get_longitude(location);
		latitude = get_latitude(location);
		int id = create_order(id_user, atoi(bike.c_str()), location, longitude, latitude);
		state_user = 1;
		update_db(*this);

		pid_t fpid = fork();
		if (fpid < 0)
		{
			cout << "Error in fork()! " << endl << endl;
			exit(EXIT_FAILURE);
		}
		if (fpid == 0)
		{
			char tmp[64];
			double long_now, lat_now;
			string _query;
			time_t t;

			while (1)
			{
				sleep(2);
				
				long_now = get_longitude(get_location());
				lat_now = get_latitude(get_location());
				_query = "UPDATE `order` SET time_finish = \"";
				t = time(0);
				strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime(&t));
				_query += tmp;
				memset(tmp, 0, sizeof(tmp));
				_query += "\", long_finish = ";
				sprintf(tmp, "%.6f", long_now);
				_query += tmp;
				memset(tmp, 0, sizeof(tmp));
				_query += ", lat_finish = ";
				sprintf(tmp, "%.6f", lat_now);
				_query += tmp;
				memset(tmp, 0, sizeof(tmp));
				_query += " WHERE ID_order = ";
				sprintf(tmp, "%d", id);
				_query += tmp;
				memset(tmp, 0, sizeof(tmp));
				cout << _query << endl;

				if (mysql_query(&conn, _query.c_str()))
				{
					fprintf(stderr, "%s\n\n", mysql_error(&conn));
					mysql_close(&conn);
					system("pause");
					exit(EXIT_FAILURE);
				}

				_query = "SELECT state_order FROM `order` WHERE ID_order = ";
				sprintf(tmp, "%d", id);
				_query += tmp;
				memset(tmp, 0, sizeof(tmp));
				cout << _query << endl;

				if (mysql_query(&conn, _query.c_str()))
				{
					fprintf(stderr, "%s\n\n", mysql_error(&conn));
					mysql_close(&conn);
					system("pause");
					exit(EXIT_FAILURE);
				}
				else
				{
					// Save the results
					sql_res = mysql_use_result(&conn);
					// Get the results
					sql_row = mysql_fetch_row(sql_res);

					if (atoi(sql_row[0]) == 2)
					{
						cout << "Please remember to stop your trip." << endl << endl;
					}
					
					mysql_free_result(sql_res);
				}
			}
		}
		if (fpid > 0)
		{
			mychild = fpid;
			return id;
		}
	}
	cout << "Invalid command!" << endl << endl;
	return id_order;
}

void User::stop(int id_order)
{
	if (state_user == 0)
	{
		cout << "You have no trip to stop!" << endl << endl;
		return;
	}
	if (id_order == 0)
	{
		cout << "Something wrong with your last order." << endl << endl;
		state_user = 0;
		balance -= 5;
		update_db(*this);
		return;
	}
	location = get_location();
	longitude = get_longitude(location);
	latitude = get_latitude(location);
	float c = finish_order(id_order, location, longitude, latitude);
	cout << "This trip will cost you $" << fixed << setprecision(2) << c << "." << endl << endl;
	cout.unsetf(ios_base::fixed);
	while (c > balance)
	{
		cout << "Balance not enough, please charge first!" << endl << endl;
		chag();
	}
	balance -= c;
	cout << "After this trip, there is $" << fixed << setprecision(2) << balance << " left in your account." << endl << endl;
	cout.unsetf(ios_base::fixed);
	state_user = 0;

	if (kill(mychild, SIGKILL) < 0)
	{
		cout << "Kill error!" << endl << endl;
		exit(EXIT_FAILURE);
	}
	if (sql_res != NULL) mysql_free_result(sql_res);
	update_db(*this);
	return;
}

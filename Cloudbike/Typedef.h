//  I/O
#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdio.h>

// String
#include <stdlib.h>
#include <string>
#include <string.h>

// Data structure
#include <stack>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <deque>

// Algorithm
#include <algorithm>
#include <math.h>
#include <limits>

// Unix API
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>

// MySQL API
#include "mysql.h"
#include "errmsg.h"
#include "mysqld_error.h"
#include "mysql_version.h"

// XML
#include "tinyxml.h"
#include "tinystr.h"

using namespace std;

// Constant
#define _HOST   "localhost" // Server host
#define _USER   "root"      // Login as root
#define _PWD    "159357"    // Password of root
#define _NAME   "cloudbike" // Name of database

#define maxuser     1000
#define maxbike     10000
#define maxorder    10000
#define maxlocation 121

struct User
{
	int id_user;
	string account_number, password, alias, gender, job;
	float balance;
	int location;
	double longitude, latitude;
	short state_user;
	pid_t mychild;
	
	void find();
	int ride(int id_order);
	void stop(int id_order);
	void info();
	void chag();
	void edit();
	void ordr();

	bool edit_alias();
	bool edit_password();
	bool edit_gender();
	bool edit_job();
};

int user_sign_up(User& user);
int user_log_in(User& user, string account);

int manager_log_in();

void welcome_in();
void welcome_do(string alias);
void welcome_out(string alias);

int encode(string s);
int encode_admin(string s);
void help();
void help_admin();
void inva();

int get_location();
double get_longitude(int location);
double get_latitude(int location);
string get_street_r(int l);
string get_street_c(int l);

void connect_mysql(const char* host, const char* user, const char* pwd, const char* db);
void disconnect();
bool check_user(string account, string pwd);
bool check_user_exist(string account);
bool check_bike_exist(int ID_bike);
bool add_to_db(User& user);
bool get_info_db(User& user);
bool update_db(User& user);
void find_area(double max_long, double max_lat, double min_long, double min_lat, double longitude, double latitude, int& NUM, string* bikeid, string* bikelong, string* bikelat);
int create_order(int id_user, int id_bike, int id_location, double longitude, double latitude);
float finish_order(int id_order, int id_location, double longitude, double latitude);
string find_bike(int ID_bike);
void find_order(int ID_order);
int find_all_order(int id_user, int* order);

void manager_update();
void manager_delete();
void manager_select();
void manager_insert();
void manager_others();

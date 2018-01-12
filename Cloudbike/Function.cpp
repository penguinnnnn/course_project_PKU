#include "Typedef.h"

string street[22] = { "Wanshoulu Street", "Shangdi Street" ,"Zhongguancun Street"
, "Balizhuang Street", "Beixiaguan Street", "Beitaipingzhuang Street"
, "Xueyuanlu Street", "Shuguang Street", "Yongdinglu Street"
, "Haidian Street", "Qinghuayuan Street", "Yanyuan Street"
, "Qinghe Street", "Ganjiakou Street", "Tiancunlu Street"
, "Zizhuyuan Street", "Yangfangdian Street", "Huayuanlu Street"
, "Xisanqi Street", "Qinglongqiao Street", "Xiangshan Street"
, "Malianwa Street" };

void welcome_in()
{
	cout << "Welcome to use our product----Cloudbike!" << endl << endl;
	cout << "Please log in or sign up....." << endl << endl;
	cout << "1) If you already have an account, please input your account number;" << endl << endl;
	cout << "2) If you don not have an account yet, please input `create` to sign up." << endl << endl;
	return;
}

void welcome_do(string alias)
{
	cout << "Hello, " << alias << "!" << endl << endl;
	cout << "You have already logged in, and you can enjoy our Cloudbike now!" << endl << endl;
	cout << "Type `h` for information of all commands" << endl << endl;
	cout << "Waiting for command......" << endl << endl;
	return;
}

void welcome_out(string alias)
{
	cout << "Goodbye, " << alias << "!" << endl << endl;
	cout << "Thank you for your using our product! Hope to serve next time!" << endl << endl;
	sleep(2);
	return;
}

int encode(string s)
{
	if (s == "help" || s == "h") return 1;
	if (s == "quit" || s == "q") return 2;
	if (s == "find" || s == "f") return 3;
	if (s == "ride" || s == "r") return 4;
	if (s == "stop" || s == "s") return 5;
	if (s == "info" || s == "i") return 6;
	if (s == "edit" || s == "e") return 7;
	if (s == "chag" || s == "c") return 8;
	if (s == "ordr" || s == "o") return 9;

	if (s == "alias" || s == "a") return -1;
	if (s == "password" || s == "p") return -2;
	if (s == "gender" || s == "g") return -3;
	if (s == "job" || s == "j") return -4;

	return 0;
}

int encode_admin(string s)
{
	if (s == "help" || s == "h") return 1;
	if (s == "upda" || s == "u") return 2;
	if (s == "dele" || s == "d") return 3;
	if (s == "inse" || s == "i") return 4;
	if (s == "sele" || s == "s") return 5;
	if (s == "othe" || s == "o") return 6;
	if (s == "quit" || s == "q") return 7;

	return 0;
}

void help()
{
	cout << "Type `h` or `help` for this menu;" << endl;
	cout << "Type `q` or `quit` to end the programme;" << endl;
	cout << "Type `f` or `find` for an available bike;" << endl;
	cout << "Type `r` or `ride` to unlock a bike;" << endl;
	cout << "Type `s` or `stop` to stop your trip." << endl;
	cout << "Type `i` or `info` to show your info." << endl;
	cout << "Type `e` or `edit` to edit your info." << endl;
	cout << "Type `c` or `chag` to charge your balance." << endl;
	cout << "Type `o` or `ordr` to show your history orders." << endl;
	cout << endl;
	return;
}

void help_admin()
{
	cout << "Type `h` or `help` for this menu;" << endl;
	cout << "Type `s` or `sele` to select user/bike/order/location;" << endl;
	cout << "Type `u` or `upda` to update user/bike/order/location;" << endl;
	cout << "Type `i` or `inse` to insert user/bike;" << endl;
	cout << "Type `d` or `dele` to delete user/bike;" << endl;
	cout << "Type `o` or `othe` to use other functions." << endl;
	cout << "Type `q` or `quit` to end the programme;" << endl;
	cout << endl;
	return;
}

void inva()
{
	cout << "Invalid Command!" << endl << endl;
	return;
}

int get_location()
{
	int a;
	srand(int(time(0)));
	a = rand();
	return (a % maxlocation) + 1;
}

double get_longitude(int location)
{
	int col = (location - 1) % 11;
	int a, c;
	double x;
	srand(int(time(0)));
	a = rand();
	c = a % 20001;
	x = 116.1 + 0.02 * col + c * 0.000001 - 0.01;
	return x;
}

double get_latitude(int location)
{
	int row = (location - 1) / 11;
	int b, d;
	double y;
	srand(int(time(0)));
	b = rand();
	d = b % 20001;
	y = 39.9 + 0.02 * row + d * 0.000001 - 0.01;
	return y;
}

string get_street_r(int l)
{
	return street[(l - 1) / 11];
}

string get_street_c(int l)
{
	return street[(l - 1) % 11 + 11];
}
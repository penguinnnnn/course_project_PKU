#include "Typedef.h"

int user_sign_up(User& user)
{
	string account_number, password, alias, gender, job;
	bool toolong, exist, valid, success;

	cout << "Please set your new account number, no more than 16 characters and more than 6 characters." << endl << endl;
	cout << "New account number:";
	cin >> account_number;
	cout << endl;
	toolong = (account_number.length() > 16) || (account_number.length() < 7);
	while (toolong)
	{
		cout << "The account number is too long(short) for the database! Please try a shorter(longer) one!" << endl << endl;
		cout << "Please set your new account number, no more than 16 characters and more than 6 characters." << endl << endl;
		cout << "New account number:";
		cin >> account_number;
		cout << endl;
		toolong = (account_number.length() > 16) || (account_number.length() < 7);
	}
	exist = check_user_exist(account_number);
	while (exist)
	{
		cout << "Account number exists! Please try another!" << endl << endl;
		cout << "Please set your new account number, no more than 16 characters and more than 6 characters." << endl << endl;
		cout << "New account number:";
		cin >> account_number;
		cout << endl;
		while (toolong)
		{
			cout << "The account number is too long(short) for the database! Please try a shorter(longer) one!" << endl << endl;
			cout << "Please set your new account number, no more than 16 characters and more than 6 characters." << endl << endl;
			cout << "New account number:";
			cin >> account_number;
			cout << endl;
			toolong = (account_number.length() > 16) || (account_number.length() < 7);
		}
		exist = check_user_exist(account_number);
	}
	cout << "Account number available!" << endl << endl;

	cout << "Please set your alias, no more than 16 characters." << endl << endl;
	cout << "Alias:";
	cin >> alias;
	cout << endl;
	toolong = (alias.length() > 16);
	while (toolong)
	{
		cout << "The alias is too long for the database! Please try a shorter one!" << endl << endl;
		cout << "Please set your alias, no more than 16 characters." << endl << endl;
		cout << "Alias:";
		cin >> alias;
		cout << endl;
		toolong = (alias.length() > 16);
	}
	cout << "Set alias Successfully!" << endl << endl;

	cout << "Please set your password, no more than 16 characters." << endl << endl;
	cout << "Password:";
	cin >> password;
	cout << endl;
	toolong = (password.length() > 16);
	while (toolong)
	{
		cout << "The password is too long for the database! Please try a shorter one!" << endl << endl;
		cout << "Please set your password, no more than 16 characters." << endl << endl;
		cout << "Password:";
		cin >> password;
		cout << endl;
		toolong = (password.length() > 16);
	}
	cout << "Set password Successfully!" << endl << endl;

	cout << "Please set your gender, `m` for male or `f` for female." << endl << endl;
	cout << "Gender:";
	cin >> gender;
	cout << endl;
	valid = true;
	if (gender == "m")
		gender += "ale";
	else if (gender == "f")
		gender += "emale";
	else
		valid = false;
	while (!valid)
	{
		cout << "Gender invalid! Please try again!" << endl << endl;
		cout << "Please set your gender, `m` for male or `f` for female." << endl << endl;
		cout << "Gender:";
		cin >> gender;
		cout << endl;
		if (gender == "m")
		{
			gender += "ale";
			valid = true;
		}
		if (gender == "f")
		{
			gender += "emale";
			valid = true;
		}
	}
	cout << "Gender valid!" << endl << endl;

	cout << "Please set your job, no more than 32 characters." << endl << endl;
	cout << "Job:";
	cin >> job;
	cout << endl;
	toolong = (job.length() > 32);
	while (toolong)
	{
		cout << "The job is too long for the database! Please try a shorter one!" << endl << endl;
		cout << "Please set your job, no more than 32 characters." << endl << endl;
		cout << "Job:";
		cin >> job;
		cout << endl;
		toolong = (job.length() > 32);
	}
	cout << "Set job Successfully!" << endl << endl;

	user.account_number = account_number;
	user.password = password;
	user.alias = alias;
	user.gender = gender;
	user.job = job;
	user.state_user = 0;
	user.location = get_location();
	user.longitude = get_longitude(user.location);
	user.latitude = get_latitude(user.location);
	user.balance = 5;
	success = add_to_db(user);

	if (success)
	{
		get_info_db(user);
		cout << "Sign up successfully!" << endl << endl;
		cout << "$5 for our new user!" << endl << endl;
		return 1;
	}

	cout << "Fail to sign up!" << endl << endl;
	cout << "Please input `create` again for a new account! Or input existing account number to log in." << endl << endl;
	return 0;
}

int user_log_in(User& user, string account)
{
	if (!check_user_exist(account))
	{
		cout << "No such an account number! Please try again!" << endl << endl;
		return 0;
	}
	string pwd;
	cout << "Password:";
	cin >> pwd;
	cout << endl;
	if (check_user(account, pwd))
	{
		user.account_number = account;
		user.password = pwd;
		get_info_db(user);
		user.location = get_location();
		user.longitude = get_longitude(user.location);
		user.latitude= get_latitude(user.location);
		cout << "Log in Successfully!" << endl << endl;
		return 1;
	}
	cout << "Fail to log in!" << endl << endl;
	cout << "Please input your account number and password again! Or you can input `create` for a new account." << endl << endl;
	return 0;
}

// printf bike list bearby
void User::find()
{
	location = get_location();
	longitude = get_longitude(location);
	latitude = get_latitude(location);
	double max_long = longitude + 0.002;
	double min_long = longitude - 0.002;
	double max_lat = latitude + 0.002;
	double min_lat = latitude - 0.002;
	int NUM = 0;
	string bikeid[100];
	string bikelong[100];
	string bikelat[100];
	find_area(max_long, max_lat, min_long, min_lat, longitude, latitude, NUM, bikeid, bikelong, bikelat);

	//���xml
	TiXmlDocument *writeDoc = new TiXmlDocument; //xml�ĵ�ָ��
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	writeDoc->LinkEndChild(decl); //д���ĵ�
	TiXmlElement *RootElement = new TiXmlElement("findbike");//��Ԫ��
	writeDoc->LinkEndChild(RootElement);

	//userԪ�� 
	TiXmlElement *userElement = new TiXmlElement("user");//user 
														 //��������
	userElement->SetAttribute("ID_user", id_user);
	RootElement->LinkEndChild(userElement);//���ڵ�д���ĵ�
										   //account_number
	TiXmlElement *account_numberElement = new TiXmlElement("account_number");
	userElement->LinkEndChild(account_numberElement);
	TiXmlText *account_numberContent = new TiXmlText(account_number.c_str());
	account_numberElement->LinkEndChild(account_numberContent);
	//state
	TiXmlElement *state_userElement = new TiXmlElement("state_user");
	userElement->LinkEndChild(state_userElement);
	string sta_user;
	if (state_user == 0)
		sta_user = "0";
	else
		sta_user = "1";
	TiXmlText *state_userContent = new TiXmlText(sta_user.c_str());
	state_userElement->LinkEndChild(state_userContent);
	//location
	TiXmlElement *locationElement = new TiXmlElement("location");
	//��������
	locationElement->SetAttribute("ID_location", location);
	userElement->LinkEndChild(locationElement);
	//Longitude 
	TiXmlElement *LongitudeElement = new TiXmlElement("Longitude");
	locationElement->LinkEndChild(LongitudeElement);
	char stra[15];
	gcvt(get_longitude(location), 10, stra);
	TiXmlText *LongitudeContent = new TiXmlText(stra);
	LongitudeElement->LinkEndChild(LongitudeContent);
	//Lattitude
	TiXmlElement *LattitudeElement = new TiXmlElement("Lattitude");
	locationElement->LinkEndChild(LattitudeElement);
	char strb[15];
	gcvt(get_latitude(location), 10, strb);
	TiXmlText *LattitudeContent = new TiXmlText(strb);
	LattitudeElement->LinkEndChild(LattitudeContent);
	//district
	TiXmlElement *districtElement = new TiXmlElement("district");
	locationElement->LinkEndChild(districtElement);
	TiXmlText *districtContent = new TiXmlText("Haidian District");
	districtElement->LinkEndChild(districtContent);
	//street-r
	TiXmlElement *street_rElement = new TiXmlElement("street-r");
	locationElement->LinkEndChild(street_rElement);
	TiXmlText *street_rContent = new TiXmlText(get_street_r(location).c_str());
	street_rElement->LinkEndChild(street_rContent);
	//street-c
	TiXmlElement *street_cElement = new TiXmlElement("street-c");
	locationElement->LinkEndChild(street_cElement);
	TiXmlText *street_cContent = new TiXmlText(get_street_c(location).c_str());
	street_cElement->LinkEndChild(street_cContent);

	//Bikelist
	TiXmlElement *BikelistElement = new TiXmlElement("Bikelist");//user
	RootElement->LinkEndChild(BikelistElement);//���ڵ�д���ĵ�
	for (int i = 0; i < NUM; i++) {
		//Bike
		TiXmlElement *BikeElement = new TiXmlElement("Bike");
		//��������
		BikeElement->SetAttribute("ID_bike", bikeid[i].c_str());
		BikelistElement->LinkEndChild(BikeElement);
		//location 
		TiXmlElement *locationElement = new TiXmlElement("location");
		BikeElement->LinkEndChild(locationElement);
		//Longitude 
		TiXmlElement *LongitudeElement = new TiXmlElement("Longitude");
		locationElement->LinkEndChild(LongitudeElement);
		TiXmlText *LongitudeContent = new TiXmlText(bikelong[i].c_str());
		LongitudeElement->LinkEndChild(LongitudeContent);
		//Lattitude
		TiXmlElement *LattitudeElement = new TiXmlElement("Latitude");
		locationElement->LinkEndChild(LattitudeElement);
		TiXmlText *LattitudeContent = new TiXmlText(bikelat[i].c_str());
		LattitudeElement->LinkEndChild(LattitudeContent);
	}
	writeDoc->SaveFile("findbike.xml");
	delete writeDoc;
	return;
}

void User::info()
{
	cout << "Account number : " << account_number << endl;
	cout << "      Password : ";
	for (int i = 0 ; i < password.length() ; i ++)
		cout << '*';
	cout << endl;
	cout << "        Gender : " << gender << endl;
	cout << "           Job : " << job << endl;
	cout << "    State_user : ";
	if (state_user) cout << "Riding" << endl;
	else cout << "Not riding" << endl;
	cout << "       Balance : " << fixed << setprecision(2) << balance << endl;
	cout.unsetf(ios_base::fixed);
	location = get_location();
	longitude = get_longitude(location);
	latitude = get_latitude(location);
	cout << "      District : " << "Haidian District" << endl;
	cout << "      Street_r : " << get_street_r(location) << endl;
	cout << "      Street_c : " << get_street_c(location) << endl;
	cout << "     Longitude : " << fixed << setprecision(6) << longitude << endl;
	cout.unsetf(ios_base::fixed);
	cout << "      Latitude : " << fixed << setprecision(6) << latitude << endl;
	cout.unsetf(ios_base::fixed);
	cout << endl;
	return;
}

void User::edit()
{
	string cmd;
	bool valid = false;
	while (!valid)
	{
		cout << "1) Type `a` or `alias` to edit your alias;" << endl;
		cout << "2) Type `p` or `password` to edit your password;" << endl;
		cout << "3) Type `g` or `gender` to edit your gender;" << endl;
		cout << "4) Type `j` or `job` to edit your job;" << endl;
		cout << "5) Type `q` or `quit` to quit editing." << endl;
		cout << endl;
		cin >> cmd;
		cout << endl;
		switch (encode(cmd))
		{
		case -1: valid = edit_alias(); break;
		case -2: valid = edit_password(); break;
		case -3: valid = edit_gender(); break;
		case -4: valid = edit_job(); break;
		case 2: valid = true; break;
		default: inva(); cout << "Please type again!" << endl << endl; break;
		}
	}
	update_db(*this);
	return;
}

bool User::edit_alias()
{
	string a;
	cout << "New Alias";
	cin >> a;
	cout << endl;
	if (a.length() > 16)
	{
		cout << "Fail to edit your alias because it is too long!" << endl << endl;
		return false;
	}
	else
	{
		alias = a;
		return true;
	}
}

bool User::edit_password()
{
	string p;
	cout << "New Password";
	cin >> p;
	cout << endl;
	if (p.length() > 16)
	{
		cout << "Fail to edit your password because it is too long!" << endl << endl;
		return false;
	}
	else
	{
		password = p;
		return true;
	}
}

bool User::edit_gender()
{
	string g;
	cout << "New Gender(`male` or `female`) : ";
	cin >> g;
	cout << endl;
	if (g == "male" || g == "female")
	{
		gender = g;
		cout << "Current gender is " << g << "." << endl << endl;
		return true;
	}
	else
	{
		cout << "Fail to edit your gender!" << endl << endl;
		return false;
	}
}

bool User::edit_job()
{
	string j;
	cout << "New Job";
	cin >> j;
	cout << endl;
	if (j.length() > 32)
	{
		cout << "Fail to edit your job because it is too long!" << endl << endl;
		return false;
	}
	else
	{
		job = j;
		return true;
	}
}

void User::chag()
{
	string c;
	cout << "How much $ would you like to charge? (Or any else to quit.)" << endl << endl;
	cin >> c;
	cout << endl;
	if (atoi(c.c_str()) == 0) return;
	balance += atoi(c.c_str());
	cout << "You have charge $" << fixed << setprecision(2) << atoi(c.c_str()) << " to balance." << endl << endl;
	cout.unsetf(ios_base::fixed);
	update_db(*this);
	cout << "Current balance is $" << fixed << setprecision(2) << balance << "." << endl << endl;
	cout.unsetf(ios_base::fixed);
	return;
}

void User::ordr()
{
	int order[100];
	int NUM;
	NUM = find_all_order(id_user, order);
	string id_order;
	cout << "You can input the order number to get detail or input any else to quit." << endl << endl;
	cin >> id_order;
	cout << endl;
	while (atoi(id_order.c_str()) != 0)
	{
		if (atoi(id_order.c_str()) > NUM || atoi(id_order.c_str()) < 0)
			cout << "Invalid number!" << endl << endl;
		else
			find_order(order[atoi(id_order.c_str())]);
		cout << "You can input the order ID to get detail or input any else to quit." << endl << endl;
		cin >> id_order;
		cout << endl;
	}
	return;
}
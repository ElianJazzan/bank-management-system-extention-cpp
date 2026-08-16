#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

const string clientsFile = "clients.txt";
const string userFile = "users.txt";

struct stClient
{
	string accountNumber;
	string pinCode;
	string fullName;
	string phoneNumber;
	double accountBalance;
	bool markDelete = false;
};
struct stUser
{
	string userName;
	string passWord;
	int permission;
	bool markDelete = false;
};

stUser currentUser;


enum enMainMeneu { enList = 1, enAdd = 2, enDelete = 3, enUpdate = 4, enFind = 5, enTrans = 6, enManage = 7, enLogout = 8 };

enum enPermission { pAll = -1, pList = 1, pAdd = 2, pDelete = 4, pUpdate = 8, pFind = 16, pTrans = 32, pManage = 64 };

enum enManageMeneu { eList = 1, eAdd = 2, eDelete = 3, eUpdate = 4, eFind = 5, eMainMeneu = 6 };

enum enTransMeneu { enDeposit = 1, enWithdraw = 2, enShowTotalBalance = 3, enShowMainMeneuFromTrans = 4 };

//Forward declarations needed because the meneu functions call each other
//(showMainMeneu -> performMainMeneuOption -> showTransMeneu -> goBackToMainMeneu -> showMainMeneu ...)
void showMainMeneu();
void showTransMeneu();
void showManageUsersMeneu();
bool checkAccessPermission(enPermission permission);
void login();

vector <string> splitString(string s, string space)
{
	string sWord;
	short pos = 0;
	vector<string> vString;

	while ((pos = s.find(space)) != std::string::npos)
	{
		sWord = s.substr(0, pos);

		if (sWord != " ")
		{
			vString.push_back(sWord);
		}
		s.erase(0, pos + space.length());
	}
	if (s != " ")
	{
		vString.push_back(s);
	}
	return vString;
}

stClient convertLineClientToRecord(string line, string space = "///")
{
	stClient client;
	vector<string>vClients = splitString(line, space);

	client.accountNumber = vClients[0];
	client.pinCode = vClients[1];
	client.fullName = vClients[2];
	client.phoneNumber = vClients[3];
	client.accountBalance = stod(vClients[4]);

	return client;
}
string convertRecordClientToLine(stClient client, string space = "///")
{
	string line;

	line += client.accountNumber + space;
	line += client.pinCode + space;
	line += client.fullName + space;
	line += client.phoneNumber + space;
	line += to_string(client.accountBalance);

	return line;
}

stUser convertLineUserToRecord(string line, string space = "///")
{
	stUser user;
	vector<string>vUsers = splitString(line, space);

	user.userName = vUsers[0];
	user.passWord = vUsers[1];
	user.permission = stoi(vUsers[2]);

	return user;
}
string convertRecordUserToLine(stUser user, string space = "///")
{
	string line;

	line += user.userName + space;
	line += user.passWord + space;
	line += to_string(user.permission);

	return line;
}

bool clientInFileByAccountNumber(string fileName, vector <stClient> vClients, string accountNumber)
{


	fstream myFile;
	myFile.open(fileName, ios::in);

	if (myFile.is_open())
	{
		stClient client;
		string line;

		while (getline(myFile, line))
		{
			client = convertLineClientToRecord(line);

			if (client.accountNumber == accountNumber)
			{
				myFile.close();
				return true;
			}
			vClients.push_back(client);
		}
		myFile.close();
	}
	return false;
}
bool userInFileByUserName(string fileName, vector<stUser>vUsers, string userName)
{


	fstream myFile;
	myFile.open(fileName, ios::in);
	if (myFile.is_open())
	{
		stUser user;
		string line;

		while (getline(myFile, line))
		{
			user = convertLineUserToRecord(line);
			if (user.userName == userName)
			{
				myFile.close();
				return true;
			}
			vUsers.push_back(user);
		}
		myFile.close();
	}
	return false;
}

vector<stClient> loadFromClientsFile(string fileName)
{
	vector <stClient> vClients;
	fstream myFile;

	myFile.open(fileName, ios::in);
	if (myFile.is_open())
	{
		stClient client;
		string line;

		while (getline(myFile, line))
		{
			client = convertLineClientToRecord(line);
			vClients.push_back(client);
		}
		myFile.close();
	}
	return vClients;
}
vector<stUser> loadFromUserFile(string fileName)
{
	vector <stUser> vUser;
	fstream myFile;

	myFile.open(fileName, ios::in);
	if (myFile.is_open())
	{
		stUser user;
		string line;

		while (getline(myFile, line))
		{
			user = convertLineUserToRecord(line);
			vUser.push_back(user);
		}
		myFile.close();
	}
	return vUser;
}


bool isUserFound(string userName, string passWord, stUser& user)
{
	vector<stUser> vUser = loadFromUserFile(userFile);

	for (stUser& u : vUser)
	{
		if (u.userName == userName && u.passWord == passWord)
		{
			user = u;
			return true;
		}
	}
	return false;
}
bool loadUserInfo(string userName, string passWord)
{
	if (isUserFound(userName, passWord, currentUser))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int numberOfPermission()
{
	int permission = 0;
	char answer = 'n';

	cout << "Do you want to give full access? Y/N. ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		return -1;
	}

	cout << "Do you want to give access to list of cliets? Y/N. ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		permission += enPermission::pList;
	}

	cout << "Do you want to give access to add new client? Y/N. ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		permission += enPermission::pAdd;
	}

	cout << "Do you want to give access to delete client? Y/N. ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		permission += enPermission::pDelete;
	}

	cout << "Do you want to give access to update client? Y/N. ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		permission += enPermission::pUpdate;
	}

	cout << "Do you want to give access to find client? Y/N. ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		permission += enPermission::pFind;
	}

	cout << "Do you want to give access to Tansaction? Y/N. ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		permission += enPermission::pTrans;
	}

	cout << "Do you want to give access to manage users? Y/N. ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y')
	{
		permission += enPermission::pManage;
	}

	return permission;
}
bool checkAccessPermission(enPermission permission)
{
	if (currentUser.permission == enPermission::pAll)
	{
		return true;
	}
	if ((permission & currentUser.permission) == permission)
	{
		return true;
	}
	return false;
}

void showAccessDeniedMessage()
{
	cout << "\n------------------------------------\n";
	cout << "ACCESS DENIED,\nYOU DONT HAVE PERMISSION TO DO THIS,\nPLEASE CONTACT YOUR ADMIN.";
	cout << "\n------------------------------------\n";
}

void printClientCard(stClient client)
{
	cout << "-- INFO CLIENT --" << endl;

	cout << "ACCOUNT NUMBER: " << client.accountNumber << endl;
	cout << "PIN CODE: " << client.pinCode << endl;
	cout << "FULL NAME: " << client.fullName << endl;
	cout << "PHONE NUMBER: " << client.phoneNumber << endl;
	cout << "ACCOUNT BALANCE: " << client.accountBalance << endl;

}
void printUserCard(stUser user)
{
	cout << "USERNAME: " << user.userName << endl;
	cout << "PASSWORD: " << user.passWord << endl;
	cout << "PERMISSION: " << user.permission << endl;
}

//This part for client
void printClientInfo(stClient client)
{
	cout << "| " << setw(15) << left << client.accountNumber;
	cout << "| " << setw(10) << left << client.pinCode;
	cout << "| " << setw(40) << left << client.fullName;
	cout << "| " << setw(12) << left << client.phoneNumber;
	cout << "| " << setw(12) << left << client.accountBalance;

}
void showClientList()
{
	if (!checkAccessPermission(enPermission::pList))
	{
		showAccessDeniedMessage();
		return;
	}

	vector <stClient> vClients = loadFromClientsFile(clientsFile);

	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	for (stClient& c : vClients)
	{
		printClientInfo(c);
		cout << endl;
	}

	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}
//This part for user
void printUserInfo(stUser user)
{
	cout << "| " << setw(15) << left << user.userName;
	cout << "| " << setw(10) << left << user.passWord;
	cout << "| " << setw(40) << left << user.permission;
}
void showUserList()
{
	vector <stUser> vUser = loadFromUserFile(userFile);

	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Username";
	cout << "| " << left << setw(10) << "PassWord";
	cout << "| " << left << setw(40) << "Permission";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	for (stUser& u : vUser)
	{
		printUserInfo(u);
		cout << endl;
	}

	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

//This part for client
stClient readClient()
{
	stClient client;

	cout << "Enter account number: ";
	getline(cin >> ws, client.accountNumber);

	while (clientInFileByAccountNumber(clientsFile, vector<stClient>{}, client.accountNumber))
	{
		cout << "The client is exist, enter another one: ";
		getline(cin >> ws, client.accountNumber);
	}

	cout << "Enter pin code: ";
	getline(cin, client.pinCode);
	cout << "Enter full name: ";
	getline(cin, client.fullName);
	cout << "Enter phone number: ";
	getline(cin, client.phoneNumber);
	cout << "Enter account balance: ";
	cin >> client.accountBalance;

	return client;
}
void addToFile(string fileName, string line)
{
	fstream myFile;

	myFile.open(fileName, ios::out | ios::app);

	if (myFile.is_open())
	{
		myFile << line << endl;

	}
	myFile.close();
}
void addNewClient()
{
	stClient client = readClient();
	addToFile(clientsFile, convertRecordClientToLine(client));
}
void addNewClients()
{
	char readMore = 'y';

	do
	{
		cout << "-ADDING CLIENT-";
		addNewClient();

		cout << "THE ADDING IS DONE! ";
		cout << "Do you want to add more? Y/N. ";
		cin >> readMore;
	} while (readMore == toupper('y'));
}
void showAddNewClientScreen()
{
	if (!checkAccessPermission(enPermission::pAdd))
	{
		showAccessDeniedMessage();
		return;
	}

	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Client Screen";
	cout << "\n-----------------------------------\n";

	addNewClients();
}
//This part for user
stUser readUser()
{

	stUser user;

	cout << "Enter username: ";
	getline(cin >> ws, user.userName);

	while (userInFileByUserName(userFile, vector<stUser>{}, user.userName))
	{
		cout << "The user is exist, enter another one: ";
		getline(cin >> ws, user.userName);
	}

	cout << "Enter password: ";
	getline(cin, user.passWord);

	user.permission = numberOfPermission();

	return user;
}
void addNewUser()
{
	stUser user = readUser();
	addToFile(userFile, convertRecordUserToLine(user));
}
void addNewUsers()
{
	char readMore = 'y';

	do
	{
		cout << "-ADDING USERS-";
		addNewUser();

		cout << "THE ADDING IS DONE! ";
		cout << "Do you want to add more? Y/N. ";
		cin >> readMore;
	} while (readMore == toupper('y'));
}
void showAddNewUserScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tAdd New User Screen";
	cout << "\n-----------------------------------\n";

	addNewUsers();
}

//This part for client
vector <stClient> saveClientAgainToFile(string fileName, vector <stClient>& vClients)
{
	fstream myFile;
	myFile.open(fileName, ios::out);
	string line;

	if (myFile.is_open())
	{
		for (stClient& c : vClients)
		{
			if (c.markDelete == false)
			{
				line = convertRecordClientToLine(c);
				myFile << line << endl;
			}
		}
	}
	myFile.close();

	return vClients;
}
bool markClientForDeleteByAccountNumber(vector<stClient>& vClients, string accountNumber)
{
	for (stClient& c : vClients)
	{
		if (c.accountNumber == accountNumber)
		{
			c.markDelete = true;
			return true;
		}
	}
	return false;
}
//This part for client - needed so delete/update/find can actually read the client's data
bool findClientByAccountNumber(vector<stClient> vClients, string accountNumber, stClient& client)
{
	for (stClient& c : vClients)
	{
		if (c.accountNumber == accountNumber)
		{
			client = c;
			return true;
		}
	}
	return false;
}
bool deleteClientByAccountNumber(vector<stClient>& vClients, string accountNumber)
{
	char answer = 'n';
	stClient client;
	if (findClientByAccountNumber(vClients, accountNumber, client))
	{
		printClientCard(client);

		cout << "ARE YOU SURE YOU WANT TO DELETE THIS CLIENT? Y/N. ";
		cin >> answer;

		if (answer == 'Y' || answer == 'y')
		{
			markClientForDeleteByAccountNumber(vClients, accountNumber);
			saveClientAgainToFile(clientsFile, vClients);

			vClients = loadFromClientsFile(clientsFile);
			cout << "THE CLIENT DELETE!";
			return true;
		}
	}
	else
	{
		cout << "THE CLIENT NOT FOUND!";
		return false;
	}
	return false;
}
string readAccountNumber()
{
	string accountNumber;
	cout << "Enter account number: ";
	cin >> accountNumber;

	return accountNumber;
}
void showDeleteClientScreen()
{
	if (!checkAccessPermission(enPermission::pDelete))
	{
		showAccessDeniedMessage();
		return;
	}

	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n-----------------------------------\n";
	vector<stClient>vClients = loadFromClientsFile(clientsFile);
	string accountNumber = readAccountNumber();

	deleteClientByAccountNumber(vClients, accountNumber);
}
//This part for user
bool markUserForDelete(vector<stUser>& vUser, string userName)
{
	for (stUser& u : vUser)
	{
		if (u.userName == userName)
		{
			u.markDelete = true;
			return true;
		}
	}
	return false;
}
//This part for user - needed so delete/update/find can actually read the user's data
bool findUserByUserName(vector<stUser> vUser, string userName, stUser& user)
{
	for (stUser& u : vUser)
	{
		if (u.userName == userName)
		{
			user = u;
			return true;
		}
	}
	return false;
}
vector<stUser>saveUserAgainToFile(string fileName, vector<stUser>& vUser)
{
	fstream myFile;
	myFile.open(fileName, ios::out);
	string line;

	if (myFile.is_open())
	{
		for (stUser& u : vUser)
		{
			if (u.markDelete == false)
			{
				line = convertRecordUserToLine(u);
				myFile << line << endl;
			}
		}
	}
	myFile.close();

	return vUser;
}
string readUserName()
{
	string userName;
	cout << "Enter user name: ";
	cin >> userName;

	return userName;
}
bool deleteUserByUserName(vector<stUser>& vUser, string userName)
{
	char answer = 'n';
	stUser user;

	if (userName == "Admin")
	{
		cout << "YOU CANNOT DELETE THIS USER!";
		return false;
	}

	if (findUserByUserName(vUser, userName, user))
	{
		printUserCard(user);
		cout << "ARE YOU SURE YOU WANT TO DELETE THIS USER? Y/N. ";
		cin >> answer;

		if (answer == 'y' || answer == 'Y')
		{
			markUserForDelete(vUser, userName);
			saveUserAgainToFile(userFile, vUser);

			vUser = loadFromUserFile(userFile);

			cout << "THE USER DELETED! ";
			return true;
		}
	}
	else
	{
		cout << "THE USER NOT FOUND! ";
		return false;
	}
	return false;
}
void showDeleteUserScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Users Screen";
	cout << "\n-----------------------------------\n";

	vector <stUser> vUsers = loadFromUserFile(userFile);
	string user = readUserName();

	deleteUserByUserName(vUsers, user);
}

//This part for client
stClient changeClientRecord(string accountNumber)
{
	stClient client;
	client.accountNumber = accountNumber;
	cout << "Enter pin code: ";
	getline(cin >> ws, client.pinCode);

	cout << "Enter full name: ";
	getline(cin, client.fullName);

	cout << "Enter phone number: ";
	getline(cin, client.phoneNumber);

	cout << "Enter account balance: ";
	cin >> client.accountBalance;

	return client;

}
bool upadateClientbyAccountNumber(vector<stClient>& vClient, string accountNumber)
{
	stClient client;
	char answer = 'n';

	if (findClientByAccountNumber(vClient, accountNumber, client))
	{
		printClientCard(client);
		cout << "ARE YOU SURE YOU WANT TO UPDATE? Y/N.";
		cin >> answer;

		if (answer == 'y' || answer == 'Y')
		{
			for (stClient& c : vClient)
			{
				if (c.accountNumber == accountNumber)
				{
					c = changeClientRecord(accountNumber);
					break;
				}
			}
			saveClientAgainToFile(clientsFile, vClient);

			cout << "THE DATA UPDATED! ";

			return true;
		}
	}
	else
	{
		cout << "THE CLIENT NOT FOUND! ";
		return false;
	}
	return false;
}
void showUpdateClientScreen()
{
	if (!checkAccessPermission(enPermission::pUpdate))
	{
		showAccessDeniedMessage();
		return;
	}

	cout << "\n-----------------------------------\n";
	cout << "\tUpdate clients Screen";
	cout << "\n-----------------------------------\n";

	string accountNumber = readAccountNumber();
	vector<stClient> vClients = loadFromClientsFile(clientsFile);

	upadateClientbyAccountNumber(vClients, accountNumber);
}
//This part for user
stUser changeUserRecord(string userName)
{
	stUser user;
	user.userName = userName;

	cout << "Enter password: ";
	getline(cin >> ws, user.passWord);

	user.permission = numberOfPermission();

	return user;
}
bool updateUserByUserName(vector<stUser>& vUser, string userName)
{
	stUser user;

	char answer = 'n';

	if (findUserByUserName(vUser, userName, user))
	{
		printUserCard(user);
		cout << "ARE YOU SURE YOU WANT TO UPDATE? Y/N.";
		cin >> answer;

		if (answer == 'y' || answer == 'Y')
		{
			for (stUser& u : vUser)
			{
				if (u.userName == userName)
				{
					u = changeUserRecord(userName);
					break;
				}
			}
			saveUserAgainToFile(userFile, vUser);

			cout << "THE DATA UPDATED! ";

			return true;
		}
	}
	else
	{
		cout << "THE USER NOT FOUND! ";
		return false;
	}
	return false;
}
void showUpdateuSERScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate User Screen";
	cout << "\n-----------------------------------\n";

	string userName = readUserName();
	vector<stUser> vUser = loadFromUserFile(userFile);

	updateUserByUserName(vUser, userName);
}

//This part for client - find
void showFindClientScreen()
{
	if (!checkAccessPermission(enPermission::pFind))
	{
		showAccessDeniedMessage();
		return;
	}

	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";

	vector<stClient> vClients = loadFromClientsFile(clientsFile);
	stClient client;
	string accountNumber = readAccountNumber();

	if (findClientByAccountNumber(vClients, accountNumber, client))
		printClientCard(client);
	else
		cout << "THE CLIENT NOT FOUND!";
}
//This part for user - find
void showFindUserScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tFind User Screen";
	cout << "\n-----------------------------------\n";

	vector<stUser> vUser = loadFromUserFile(userFile);
	stUser user;
	string userName = readUserName();

	if (findUserByUserName(vUser, userName, user))
		printUserCard(user);
	else
		cout << "THE USER NOT FOUND!";
}

//This part for transactions (deposit/withdraw/total balance)
bool depositBalanceToClientByAccountNumber(string accountNumber, double amount, vector<stClient>& vClients)
{
	char answer = 'n';
	cout << "ARE YOU SURE YOU WANT TO PERFORM THIS TRANSACTION? Y/N. ";
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		for (stClient& c : vClients)
		{
			if (c.accountNumber == accountNumber)
			{
				c.accountBalance += amount;
				saveClientAgainToFile(clientsFile, vClients);
				cout << "DONE SUCCESSFULLY. NEW BALANCE IS: " << c.accountBalance;
				return true;
			}
		}
	}
	return false;
}
void showDepositScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n-----------------------------------\n";

	stClient client;
	vector<stClient> vClients = loadFromClientsFile(clientsFile);
	string accountNumber = readAccountNumber();

	while (!findClientByAccountNumber(vClients, accountNumber, client))
	{
		cout << "THE CLIENT NOT FOUND, TRY AGAIN.\n";
		accountNumber = readAccountNumber();
	}

	printClientCard(client);

	double amount = 0;
	cout << "Enter deposit amount: ";
	cin >> amount;

	depositBalanceToClientByAccountNumber(accountNumber, amount, vClients);
}
void showWithdrawScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n-----------------------------------\n";

	stClient client;
	vector<stClient> vClients = loadFromClientsFile(clientsFile);
	string accountNumber = readAccountNumber();

	while (!findClientByAccountNumber(vClients, accountNumber, client))
	{
		cout << "THE CLIENT NOT FOUND, TRY AGAIN.\n";
		accountNumber = readAccountNumber();
	}

	printClientCard(client);

	double amount = 0;
	cout << "Enter withdraw amount: ";
	cin >> amount;

	while (amount > client.accountBalance)
	{
		cout << "AMOUNT EXCEEDS THE BALANCE, YOU CAN WITHDRAW UP TO: " << client.accountBalance << endl;
		cout << "Enter another amount: ";
		cin >> amount;
	}

	depositBalanceToClientByAccountNumber(accountNumber, amount * -1, vClients);
}
void showTotalBalanceScreen()
{
	vector<stClient> vClients = loadFromClientsFile(clientsFile);
	double totalBalance = 0;

	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	for (stClient& c : vClients)
	{
		cout << "| " << setw(15) << left << c.accountNumber;
		cout << "| " << setw(40) << left << c.fullName;
		cout << "| " << setw(12) << left << c.accountBalance;
		totalBalance += c.accountBalance;
		cout << endl;
	}

	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "TOTAL BALANCE = " << totalBalance << endl;
}

void goBackToMainMeneu()
{
	cout << "\n\nPress any key to go back to Main Meneu...";
	system("pause>0");
	showMainMeneu();
}
void goBackToTransMeneu()
{
	cout << "\n\nPress any key to go back to Transactions Meneu...";
	system("pause>0");
	showTransMeneu();
}
void goBackToManageUsersMeneu()
{
	cout << "\n\nPress any key to go back to Manage Users Meneu...";
	system("pause>0");
	showManageUsersMeneu();
}

int readTransMeneuOption()
{
	int option;
	cout << "Choose an option from the meneu [1 - 4]: ";
	cin >> option;
	return option;
}
void performTransMeneuOption(enTransMeneu option)
{
	switch (option)
	{
	case enTransMeneu::enDeposit:
		system("cls");
		showDepositScreen();
		goBackToTransMeneu();
		break;

	case enTransMeneu::enWithdraw:
		system("cls");
		showWithdrawScreen();
		goBackToTransMeneu();
		break;

	case enTransMeneu::enShowTotalBalance:
		system("cls");
		showTotalBalanceScreen();
		goBackToTransMeneu();
		break;

	case enTransMeneu::enShowMainMeneuFromTrans:
		showMainMeneu();
		break;
	}
}
void showTransMeneu()
{
	if (!checkAccessPermission(enPermission::pTrans))
	{
		showAccessDeniedMessage();
		goBackToMainMeneu();
		return;
	}

	system("cls");
	cout << "===========================================\n";
	cout << "\t\tTransactions Meneu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Meneu.\n";
	cout << "===========================================\n";

	performTransMeneuOption((enTransMeneu)readTransMeneuOption());
}

int readManageMeneuOption()
{
	int option;
	cout << "Choose an option from the meneu [1 - 6]: ";
	cin >> option;
	return option;
}
void performManageUsersMeneuOption(enManageMeneu option)
{
	switch (option)
	{
	case enManageMeneu::eList:
		system("cls");
		showUserList();
		goBackToManageUsersMeneu();
		break;

	case enManageMeneu::eAdd:
		system("cls");
		showAddNewUserScreen();
		goBackToManageUsersMeneu();
		break;

	case enManageMeneu::eDelete:
		system("cls");
		showDeleteUserScreen();
		goBackToManageUsersMeneu();
		break;

	case enManageMeneu::eUpdate:
		system("cls");
		showUpdateuSERScreen();
		goBackToManageUsersMeneu();
		break;

	case enManageMeneu::eFind:
		system("cls");
		showFindUserScreen();
		goBackToManageUsersMeneu();
		break;

	case enManageMeneu::eMainMeneu:
		showMainMeneu();
		break;
	}
}
void showManageUsersMeneu()
{
	if (!checkAccessPermission(enPermission::pManage))
	{
		showAccessDeniedMessage();
		goBackToMainMeneu();
		return;
	}

	system("cls");
	cout << "===========================================\n";
	cout << "\t\tManage Users Meneu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Meneu.\n";
	cout << "===========================================\n";

	performManageUsersMeneuOption((enManageMeneu)readManageMeneuOption());
}

void performMainMeneuOption(enMainMeneu option)
{
	switch (option)
	{
	case enMainMeneu::enList:
		system("cls");
		showClientList();
		goBackToMainMeneu();
		break;

	case enMainMeneu::enAdd:
		system("cls");
		showAddNewClientScreen();
		goBackToMainMeneu();
		break;

	case enMainMeneu::enDelete:
		system("cls");
		showDeleteClientScreen();
		goBackToMainMeneu();
		break;

	case enMainMeneu::enUpdate:
		system("cls");
		showUpdateClientScreen();
		goBackToMainMeneu();
		break;

	case enMainMeneu::enFind:
		system("cls");
		showFindClientScreen();
		goBackToMainMeneu();
		break;

	case enMainMeneu::enTrans:
		system("cls");
		showTransMeneu();
		break;

	case enMainMeneu::enManage:
		system("cls");
		showManageUsersMeneu();
		break;

	case enMainMeneu::enLogout:
		system("cls");
		login();
		break;
	}
}
int readMainMeneuOption()
{
	int option;
	cout << "Choose an option from the meneu [1 - 8]: ";
	cin >> option;

	return option;
}
void showMainMeneu()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "===========================================\n";

	performMainMeneuOption((enMainMeneu)readMainMeneuOption());
}

void login()
{
	bool loginFaild = false;

	string userName, passWord;

	do
	{
		system("cls");

		cout << "--------------------------------------------" << endl;
		cout << "\t\tLOGIN SCREEN" << endl;
		cout << "--------------------------------------------" << endl;

		if (loginFaild)
		{
			cout << "INVALID USERNAME / PASSWORD! ";
		}
		cout << "Enter username: ";
		cin >> userName;
		cout << "Enter passWord: ";
		cin >> passWord;

		loginFaild = !loadUserInfo(userName, passWord);

	} while (loginFaild);
	showMainMeneu();
}

int main()
{
	login();

	system("pause>0");
	return 0;
}

#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <iterator>

using namespace std;

class Train {
protected:
	string TripID;
	string destination;
	string origin;
	double price;
	friend class Admin;

private:
	int NumberOfSeats;
	int ReservedSeats;
	int RemainingSeats;
public:
	Train() {

	}

	Train(string TID, string dstn, string org, double p, int NOS) {
		this->TripID = TID;
		this->destination = dstn;
		this->origin = org;
		this->price = p;
		this->NumberOfSeats = NOS;
		this->ReservedSeats = 0;
		this->RemainingSeats = NOS - ReservedSeats;
	}

	virtual void DisplayInfo() {
		cout << "Train Destination: " << destination << endl;
		cout << "Train Origin: " << origin << endl;
		cout << "Ticket Price: " << price << endl;
		cout << "Number of Seats: " << NumberOfSeats << endl;
		cout << "Remaining Seats: " << RemainingSeats << endl << endl;
	}

	int getRemainingSeats() {
		return RemainingSeats;
	}
	
	//Used by Admin and Passenger to control available number of seats on a trip
	//If maximum seating is reached, no more bookings are allowed(condition included in Passenger's bookTicket())
	void setSeats(int option, int NumOfSts) {
		if (option == 1) {
			ReservedSeats = ReservedSeats + NumOfSts;
			RemainingSeats = NumberOfSeats - ReservedSeats;
		}
		else {
			ReservedSeats = ReservedSeats - NumOfSts;
			RemainingSeats = NumberOfSeats - ReservedSeats;
		}
	}

	virtual string getDstn() {
		return destination;
	}

	virtual string getOrgn() {
		return origin;
	}

	virtual double getPrice() {
		return price;
	}

	virtual string getTID() {
		return TripID;
	}
};

class ticket : public Train {
private:
	int NumberOfSeatsReserved;
public:
	ticket(string TID, string dstn, string org, double p, int NOSR) {
		this->TripID = TID;
		this->destination = dstn;
		this->origin = org;
		this->price = p;
		this->NumberOfSeatsReserved = NOSR;
	}

	void DisplayInfo() {
		cout << "Train Destination: " << destination << endl;
		cout << "Train Origin: " << origin << endl;
		cout << "Ticket Price: " << price << endl;
		cout << "Number of Reserved Seats: " << NumberOfSeatsReserved << endl;
	}

	int getNOSR() {
		return NumberOfSeatsReserved;
	}
};


class User {
protected:
	string name;
	string Password;
public:
	virtual void DisplayInfo() {
		cout << "\nName: " << name << endl;
		cout << "Password: " << Password << endl;
	}

	string getName() {
		return name;
	}

	string getPwd() {
		return Password;
	}
};


class Passenger : public User {
private:
	string Email;
public:
	vector<ticket> Tickets;
	int tempNumOfSts;
	Passenger(string n, string em, string pwd) {
		this->name = n;
		this->Email = em;
		this->Password = pwd;
	}
	//Uses data of selected trip from trainRecord vector to create ticket instances under user profile
	//Doesn't allow booking if booking already exists
	//Doesnt allow booking if maximum seating is reached
	void bookTickets(vector<Train>& T) {
		bool alreadyBooked = false;
		if (T.size() != 0) {
			int indx;
			for (int i = 0; i < T.size(); ++i) {
				cout << i << ")" << endl;
				T[i].DisplayInfo();
			}
			cout << "\nSelect a Trip or Enter a value that doesn't correspond to any trip to go back: ";
			cin >> indx;
			if (indx < T.size()) {
				for (int i = 0; i < Tickets.size(); ++i) {
					if (T[indx].getTID() == Tickets[i].getTID()) {
						alreadyBooked = true;
					}
					else alreadyBooked = false;
				}
				if (alreadyBooked == false) {
					cout << "Enter Number of Seats to book: ";
					cin >> tempNumOfSts;
					cout << "\n\n";
					if (T[indx].getRemainingSeats() - tempNumOfSts >= 0) {
						Tickets.push_back(ticket(T[indx].getTID(), T[indx].getDstn(), T[indx].getOrgn(), T[indx].getPrice(), tempNumOfSts));
						T[indx].setSeats(1, tempNumOfSts);
					}
					else cout << "*************************\nNot enough Seats Remaining\n*************************\n\n";
				}
				else cout << "-----------------------------\nYou already have a booking on the selected trip\n-----------------------------\n\n";
			}
			else cout << "************************\nNo Trips Booked\n************************\n\n";

		}
		else cout << "************************\nNo Available Trips\n************************\n\n";
	}

	//Cancels trip by comparing trip ID with selected trip as trip index is not reliable if bookings aren't made in order
	//allows user to go back from cancelling a trip
	void cancelTicket(vector<Train>& T) {
		if (Tickets.size() != 0) {
			for (int i = 0; i < Tickets.size(); ++i) {
				cout << i << ")" << endl;
				Tickets[i].DisplayInfo();
			}
			cout << "\nSelect One of the reservations to cancel or a value that doesn't correspond to any trip to go back: ";
			int indx;
			cin >> indx;
			if (indx < Tickets.size()) {
				for (int i = 0; i < T.size(); ++i) {
					if (Tickets[indx].getTID() == T[i].getTID()) {
						T[i].setSeats(0, Tickets[indx].getNOSR());
					}
				}
				Tickets.erase(Tickets.begin() + indx);
				cout << "------------------------\n\n";
			}
			else {
				cout << "*****************************\nNo Reservation Deleted \n*****************************\n\n";
			}

		}
		else {
			cout << "*****************************\nNo Reservations Made!\n*****************************\n\n";
		}

	}
	//iterates through each passenger owned tickets vector to view bookings
	void viewTickets() {
		if (Tickets.size() != 0) {
			for (int i = 0; i < Tickets.size(); ++i) {
				cout << i << ")" << endl;
				Tickets[i].DisplayInfo();
			}
			cout << "-----------------------------------------\n\n";
		}
		else {
			cout << "********************\nNo Reservations Made!\n********************\n\n";
		}

	}

	string getEmail() {
		return Email;
	}

	~Passenger() {
		
	}
};


class Admin: public User {
private:
	int ID;
public:
	Admin(string n, int  id, string pwd) {
		this->name = n;
		this->Password = pwd;
		this->ID = id;
	}

	void DisplayInfo() {
		cout << "\nName: " << name << endl;
		cout << "Password: " << Password << endl;
		cout << "Admin ID: " << ID << endl;
	}

	//Iterates through all tickets vectors inside the Passengers vector to find matching Trip IDs and print them
	void viewTrainPassengers(vector<Train> T, vector<Passenger> P) {
		int indx;
		for (int i = 0; i < T.size(); ++i) {
			cout << i << ")" << endl;
			T[i].DisplayInfo();
		}
		cout << "\n\nSelect a Trip to view passengers of: ";
		cin >> indx;
		cout << "***************************************\n\n";
		for (int i = 0; i < P.size(); ++i) {
			for (int z = 0; z < P[i].Tickets.size(); ++z) {
				if (T[indx].getTID() == P[i].Tickets[z].getTID()) {
					cout << "Name: " << P[i].getName() << "\t\tEmail: " << P[i].getEmail() << "\t Reserved Seats: " << P[i].Tickets[z].getNOSR() << endl;
				}
			}
		}
		cout << "---------------------------------------\n\n";
	}

	//Adds new instance of train to vector trainRecords for the user to select from
	void Addtrain(vector<Train> &T, string Tid, string dstn, string org, double p, int NOS) {
		T.push_back(Train(Tid, dstn, org, p, NOS));
	}

	//Deletes selected trip from Train Records and corresponding Passenger bookings
	void RemoveTrain(vector<Train> &T, vector<Passenger> &P) {
		if (T.size() != 0) {
			int indx;
			string temp;
			for (int i = 0; i < T.size(); ++i) {
				cout << i << ")" << endl;
				T[i].DisplayInfo();
			}
			cout << "\nEnter the number of the train to delete: ";
			cin >> indx;

			for (int i = 0; i < P.size(); ++i) {
				for (int x = 0; x < P[i].Tickets.size(); ++x) {
					if (P[i].Tickets[x].getTID() == T[indx].getTID()) {
						P[i].Tickets.erase(P[i].Tickets.begin() + x);
					}
				}
			}

			cout << endl << endl;
			T.erase(T.begin() + indx);
	
		}
		else {
			cout << "\nNo Available Trips\n\n" << endl;
		}
	}

	int getID() {
		return ID;
	}
};


int main() {

	int c1, c2;
	bool b = false;
	int tempID(0), tempNOS;
	double tempPrice;
	string tempName, tempPwd, tempEmail, tempDstn, tempOrg,TempTID, tempHolder;
	vector<Admin> AloginData;
	vector<Passenger> PloginData;
	vector<Train> trainRecord;
	int accIndex;

	while (true) {
		//main menu
		cout << "*************************" << endl;
		cout << " Train Management System" << endl;
		cout << "*************************" << endl << endl;
		cout << "1 - Sign in as USER" << endl << endl;
		cout << "2 - Sign in as ADMIN" << endl << endl;
		cout << "3 - Create a new USER" << endl << endl;
		cout << "4 - Create a new ADMIN" << endl << endl;
		cout << "5 - Exit App" << endl << endl;
		cout << "Enter your Selection: ";
		cin >> c1;

		switch (c1) {
		case 1:
			//User Log in
			cout << "Log in:-" << endl;
			cout << "Username: ";
			getline(cin >> ws, tempName);
			cout << "Email: ";
			cin >> tempEmail;
			cout << "Password: ";
			cin >> tempPwd;
			for (int i = 0; i < PloginData.size(); ++i)
			{
				if (tempName == PloginData[i].getName() && tempEmail == PloginData[i].getEmail() && tempPwd == PloginData[i].getPwd()) {
					accIndex = i;
					b = true;
					break;
				}
			}
			if (b == false) {
				cout << "\n**********************\nInvalid Login Data...\n**********************\n\n";
			}
			else cout << "\n**********************\nLogged in Successfully\n**********************\n\n";
			
			while (b) {
				//User Menu
				cout << "1 - Book a Ticket\n\n";
				cout << "2 - Cancel Reservation\n\n";
				cout << "3 - View Reservations\n\n";
				cout << "4 - Sign Out\n\n";
				cout << "Enter Your Selection: ";
				cin >> c2;

				switch (c2) {
				case 1:
					//Takes data from selected from trainRecord trips and adds it to Tickets vector
					PloginData[accIndex].bookTickets(trainRecord);
					break;
				case 2:
					//Reads selected trip ID and compares it to data inside Tickets vector to delete corresponding booking
					PloginData[accIndex].cancelTicket(trainRecord);
					break;
				case 3:
					PloginData[accIndex].viewTickets();
					break;
				case 4:
					//Signing out of user profile - exit loop
					b = false;
					cout << "\n---------------------------------\n";
					break;
				default:
					cout << "Invalid Selection!\n";
					break;
				}
			}
			break;

		case 2:
			//Admin sign in
			cout << "Log in:-" << endl;
			cout << "Username: ";
			getline(cin >> ws, tempName);
			cout << "Admin ID: ";
			cin >> tempID;
			cout << "Password: ";
			cin >> tempPwd;
			for (int i = 0; i < AloginData.size(); ++i)
			{
				if (tempName == AloginData[i].getName() && tempID == AloginData[i].getID() && tempPwd == AloginData[i].getPwd()) {
					accIndex = i;
					b = true;
					break;
				}
			}
			if (b == false) {
				cout << "\n**********************\nInvalid Login Data...\n**********************\n\n";
			}
			else cout << "\n**********************\nLogged in Successfully\n**********************\n\n";

			while (b) {
				//Admin Menu - grants admin privileges 
				cout << "1 - Add a New Trip\n\n";
				cout << "2 - Remove a Trip\n\n";
				cout << "3 - View Passenger List\n\n";
				cout << "4 - Sign Out\n\n";
				cout << "Enter Your Selection: ";
				cin >> c2;
				cout << "-----------------------------" << endl;
				switch (c2) {
				case 1:
					//Adding a new trip by constructing new Train instance and adding it to trainRecord vector declared inside main()
					cout << "Enter trip Details:-\n";
					cout << "Trip ID: ";
					cin >> TempTID;
					cout << "Destination: ";
					cin >> tempDstn;
					cout << "Origin: ";
					cin >> tempOrg;
					cout << "Number of Seats: ";
					cin >> tempNOS;
					cout << "Ticket Price: ";
					cin >> tempPrice;
					AloginData[accIndex].Addtrain(trainRecord, TempTID, tempDstn, tempOrg, tempPrice, tempNOS);
					cout << "-----------------------------\n\n";
					break;
				case 2:
					//Removes selected trip from locally declared vector trainRecord and goes through every passenger to delete booking
					cout << "\n----------------------------\n";
					AloginData[accIndex].RemoveTrain(trainRecord, PloginData);
					break;
				case 3:
					//Compares selected trip ID against passenger booking data (Tickets vector) and prints passenger info if it matches anything
					AloginData[accIndex].viewTrainPassengers(trainRecord, PloginData);
					break;
				case 4:
					//Used to exit Admin loop as in sign out of profile
					b = false;
					cout << "\n---------------------------------\n\n";
					break;
				default:
					cout << "Invalid Selection!\n";
					break;
				}
			}
			break;
		case 3:
			//Passenger log in - Saves data to passenger (PloginData) vector
			cout << "------------------------\n\n";
			cout << "Enter Full Name: ";
			cin.clear();
			cin.ignore(10000, '\n');
			getline(cin >> ws, tempName);
			cout << "\nEnter Email: ";
			cin >> tempEmail;
			cout << "\nEnter Password: ";
			cin >> tempPwd;
			PloginData.push_back(Passenger(tempName, tempEmail, tempPwd));
			cout << "\n--------------------------------\nProfile created successfully!\n--------------------------------\n\n";
			
			break;
		
		case 4:
			//Admin log in - Saves data to Admin (AloginData) vector
			cout << "------------------------\n\n";
			cout << "Enter Full Name: ";
			getline(cin >> ws, tempName);
			cout << "\nEnter your ID: ";
			cin >> tempID;
			cout << "\nEnter Password: ";
			cin >> tempPwd;
			AloginData.push_back(Admin(tempName, tempID, tempPwd));
			cout << "\n--------------------------------\nProfile created successfully!\n--------------------------------\n\n";
			break;

		case 5:
			//Exit app when 5 is entered in the main menu
			return 0;
			break;

		default:
			//Informs user that main menu input is invalid
			cout << "Invalid option\n\n";
			break;
		}
	}
}

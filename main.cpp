#include <iostream>
#include <iomanip>
#include <fstream>
#include<string>
using namespace::std;

struct MemberRecord
{
   char email[ 40 ];
   char password[ 24 ];
   char IDNumber[ 12 ];
   char name[ 12 ];
   char phone[ 12 ];
};

struct Movie
{
   int movieCode;
   int prices[ 4 ]; // prices[0]:adult, prices[1]:concession, prices[2]:disability, prices[3]:elderly
   bool dates[ 9 ];
   bool sessionTimes[ 17 ];
   bool occupiedSeats[ 9 ][ 17 ][ 8 ][ 12 ]; // occupiedSeats[i][j] is the occupied Seats for all accounts
};                                           // at j-th session time on i-th date

struct BookingInfo
{
   char email[ 40 ];
   int movieCode;
   int dateCode;
   int sessionTimeCode;
   int numTickets[ 4 ]; // numTickets[0]: the number of adult tickets,
                        // numTickets[1]: the number of concession tickets,
                        // numTickets[2]: the number of disability tickets,
                        // numTickets[3]: the number of elderly tickets
   char seletedSeats[ 24 ][ 4 ]; // seleted seats for the user with the specified email
};

char hours[ 17 ][ 8 ] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00",
                          "18:00", "19:00", "20:00", "21:00", "22:00", "23:00", "00:00", "01:00" };

void loadMemberInfo( MemberRecord memberDetails[], int &numMembers );
int inputAnInteger( int begin, int end );
void signIn( MemberRecord memberDetails[], int numMembers, Movie movies[], char movieNames[][ 60 ], int numMovies,
             BookingInfo bookingHistories[], int &numBookings, char availableDates[][ 12 ], int numDates );
bool illegal( char email[], char password[], MemberRecord memberDetails[], int numMembers, int &recordNumber );
void accountInfor( MemberRecord memberDetails[], int numMembers, int recordNumber );
void newMember( MemberRecord memberDetails[], int &numMembers );
bool existingID( char newIDNumber[], MemberRecord memberDetails[], int &numMembers );
bool existingEmail( char newEmail[], MemberRecord memberDetails[], int &numMembers );

void loadBookingHistories( BookingInfo bookingHistories[], int &numBookings );
void loadAvailableDates( char availableDates[][ 12 ], int &numDates );
void loadMovies( Movie movies[], int &numMovies );
void loadMovieNames( char movieNames[][ 60 ], int numMovies );
void buyTickets( BookingInfo bookingHistories[], int &numBookings, Movie movies[], char movieNames[][ 60 ],
                  int numMovies, char availableDates[][ 12 ], int numDates, char email[] );
void selectSeats( BookingInfo bookingHistories[], int numBookings, Movie movies[] );
void display( Movie movies[], BookingInfo bookingHistory );
void displaySessionTimes( Movie movies[], char movieNames[][ 60 ], int numMovies,
                          char availableDates[][ 12 ], int numDates );
void displayBookingHistory( BookingInfo bookingHistories[], int numBookings, Movie movies[],
                            char movieNames[][ 60 ], char availableDates[][ 12 ], char email[] );
void saveMemberInfo( MemberRecord memberDetails[], int numMembers );
void saveMovies( Movie movies[], int numMovies );
void saveBookingHistories( BookingInfo bookingHistories[], int numBookings );
void memberFileSetted(MemberRecord memberDetails[], int numMembers, int recordNumber, BookingInfo bookingHistories[], int &numBookings,
	Movie movies[], char movieNames[][60], int numMovies, char availableDates[][12], int numDates, char email[]);

int main()
{
   MemberRecord memberDetails[ 100 ] = {};
   int numMembers = 0;
   loadMemberInfo( memberDetails, numMembers );

   BookingInfo bookingHistories[ 1000 ] = {};
   int numBookings = 0;
   loadBookingHistories( bookingHistories, numBookings );

   Movie movies[ 30 ] = {};
   int numMovies = 0;
   loadMovies( movies, numMovies );

   char movieNames[ 30 ][ 60 ] = {};
   loadMovieNames( movieNames, numMovies );

   char availableDates[ 10 ][ 12 ];
   int numDates;
   loadAvailableDates( availableDates, numDates );

   cout << "Welcome to Vieshow Cinemas Taipei QSquare system\n\n";

   int choice;

   while( true )
   {
      cout << "Enter your choice:" << endl;
      cout << "1. Sign In\n";
      cout << "2. New Member\n";
      cout << "3. End\n? ";

      choice = inputAnInteger( 1, 3 );
      cout << endl;

      switch ( choice )
      {
      case 1:
         signIn( memberDetails, numMembers, movies, movieNames, numMovies, bookingHistories, numBookings,
                 availableDates, numDates );
         break;

      case 2:
         newMember( memberDetails, numMembers );
         break;

      case 3:
         saveMemberInfo( memberDetails, numMembers );
         saveMovies( movies, numMovies );
         saveBookingHistories(  bookingHistories, numBookings );
         cout << "Thank you...\n\n";
         system( "pause" );
         return 0;

      default:
         cout << "Input Error!\n\n";
         break;
      }
   }

   system( "pause" );
}

void loadMemberInfo(MemberRecord memberDetails[], int &numMembers)
{
	ifstream memberInfo("Member Info.dat", ios::in|ios::binary );
	if (!memberInfo)
	{
		cout << "Member Info can not be opened." << endl;
		system("pause");
	}

	while (!memberInfo.eof())
	{
		memberInfo.read(reinterpret_cast<char*>(&memberDetails[numMembers]), sizeof(MemberRecord));
		if (!memberInfo.eof())
		{
			numMembers++;
		}
	}
}

int inputAnInteger(int begin, int end)
{
	char choice;
	int choiceInt = 0;
	cin >> choice;
	choice -= '0';
	(int)choiceInt = choice;
	if (choice>=begin && choice<=end)
		return choiceInt;
	else
	{
		return -1;
	}
}

void signIn(MemberRecord memberDetails[], int numMembers, Movie movies[], char movieNames[][60], int numMovies,
	BookingInfo bookingHistories[], int &numBookings, char availableDates[][12], int numDates)
{
	char email[40];
	char password[24];
	
	cout << "Enter email address: ";
	cin >> email;
	cout << "Enter password: ";
	cin >> password;
	for (int i = 0; i < numMembers; i++)
	{
		if (!illegal(email, password, memberDetails, numMembers, i))
		{
			memberFileSetted(memberDetails, numMembers, i, bookingHistories, numBookings,
				movies, movieNames, numMovies, availableDates, numDates, email);
			//return;
		}
	}
	cout << "Sorry, unrecognized email or password." << endl;
}

bool illegal(char email[], char password[], MemberRecord memberDetails[], int numMembers, int &recordNumber)
{
	if (strcmp(email, memberDetails[recordNumber].email) != 0 || strcmp(password, memberDetails[recordNumber].password) != 0)
	{
		return true;
	}
	return false;
}

void accountInfor(MemberRecord memberDetails[], int numMembers, int recordNumber)
{
	cout << "1. Name: " << memberDetails[recordNumber].name << endl;
	cout << "2. Email Address: " << memberDetails[recordNumber].email << endl;
	cout << "3. Phone Number: " << memberDetails[recordNumber].phone << endl;
	cout << "4. ID Number: " << memberDetails[recordNumber].IDNumber << endl;
	cout << "5. Password: " << memberDetails[recordNumber].password << endl;

	cout << "Which one do you want to modify (0 ¡V not modify)?";
	
	while (true)
	{
		int choice;
		choice = inputAnInteger(0, 5);
		cout << endl;
		switch (choice)
		{
		case 0:
			return;
		case 1:
			cout << "Enter correct data: ";
			cin >> memberDetails[recordNumber].name;
			cout << "Successful!" << endl;
			break;
		case 2:
			char newEmail[40];
			cout << "Enter correct data: ";
			cin >> newEmail;
			if (existingEmail(newEmail, memberDetails, numMembers))
			{
				cout << "An account already exists with the e-mail!" << endl;
				return;
			}
			cout << "Successful!" << endl;
			break;
		case 3:
			cout << "Enter correct data: ";
			cin >> memberDetails[recordNumber].phone;
			cout << "Successful!" << endl;
			break;
		case 4:
			char newIDNumber[12];
			cout << "Enter correct data: ";
			cin >> newIDNumber;
			if (existingID(newIDNumber, memberDetails, numMembers))
			{
				cout << "An account already exists with the ID number!" << endl;
				return;
			}
			cout << "Successful!" << endl;
			break;
		case 5:
			cout << "Enter correct data: ";
			cin >> memberDetails[recordNumber].password;
			cout << "Successful!" << endl;
			break;
		default:
			cout << "Input Error! Please try again." << endl;
		}
		if (choice != -1)
			break;
	}
}

void newMember(MemberRecord memberDetails[], int &numMembers)
{
	MemberRecord newMember;

	cout << "Enter your ID number: ";
	cin >> newMember.IDNumber;
	if (existingID(newMember.IDNumber, memberDetails, numMembers))
	{
		cout << "An account already exists with the ID number!" << endl;
		return;
	}
	cout << "Enter your name: ";
	cin >> newMember.name;
	cout << "Enter an email address: ";
	cin >> newMember.email;
	if (existingEmail(newMember.email, memberDetails, numMembers))
	{
		cout << "An account already exists with the e-mail!" << endl;
		return;
	}
	cout << "Enter a password: ";
	cin >> newMember.password;
	cout << "Enter your phone number: ";
	cin >> newMember.phone;
	cout << "Successful!" << endl;

	memberDetails[numMembers] = newMember;
	numMembers++;

	return;
}

bool existingID(char newIDNumber[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++)
	{
		if (strcmp(newIDNumber, memberDetails[i].IDNumber) == 0)
			return true;
	}
	return false;
}

bool existingEmail(char newEmail[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++)
	{
		if (strcmp(newEmail, memberDetails[i].email) == 0)
			return true;
	}
	return false;
}

void loadBookingHistories(BookingInfo bookingHistories[], int &numBookings)
{
	ifstream historyDetails("Booking Histories.dat", ios::in | ios::binary);
	if (!historyDetails)
	{
		cout << "Booking Histories can not be opened." << endl;
		system("pause");
	}

	while (!historyDetails.eof())
	{
		historyDetails.read(reinterpret_cast<char*>(&bookingHistories[numBookings]), sizeof(BookingInfo));
		if (!historyDetails.eof())
		{
			numBookings++;
		}
	}
}

void loadAvailableDates(char availableDates[][12], int &numDates)
{
	ifstream dateDetails("Available Dates.txt", ios::in);
	if (!dateDetails)
	{
		cout << "Available Dates can not be opened." << endl;
		system("pause");
	}

	numDates = 0;
	while (dateDetails.getline(availableDates[numDates], sizeof(availableDates[numDates])))
	{
		numDates++;
	}
	numDates--;
}

void loadMovies(Movie movies[], int &numMovies)
{
	ifstream movieDetails;
	movieDetails.open("Movies.dat", ios::in | ios::binary);
	if (!movieDetails)
	{
		cout << "Movies can not be opened." << endl;
		system("pause");
	}

	while (!movieDetails.eof())
	{
		movieDetails.read(reinterpret_cast<char*>(&movies[numMovies]), sizeof(Movie));
		if (!movieDetails.eof())
		{
			numMovies++;
		}
	}
}

void loadMovieNames(char movieNames[][60], int numMovies)
{
	ifstream movieNamesDetails("Movie Names.txt", ios::in);
	if (!movieNamesDetails)
	{
		cout << "Movie Names can not be opened." << endl;
		system("pause");
	}

	numMovies = 0;
	while (movieNamesDetails.getline(movieNames[numMovies], sizeof(movieNames[numMovies])))
	{
		numMovies++;
	}
	numMovies--;
}

void buyTickets(BookingInfo bookingHistories[], int &numBookings, Movie movies[], char movieNames[][60], int numMovies, char availableDates[][12], int numDates, char email[])
{
	displaySessionTimes(movies, movieNames, numMovies, availableDates, numDates);
	for (int i = 0; i < sizeof(email); i++)
	{
		bookingHistories[numBookings].email[i] = email[i];
	}

	int choiceMovie;
	int choiceDate;
	int choiceTime;
	int adultTicket;
	int concessionTicket;
	int disabilityTicket;
	int elderlyTicket;
	cout << endl;
	while (true)
	{
		cout << "Enter movie code (0-11): ";
		choiceMovie = inputAnInteger(0, 11);
		if (choiceMovie != -1)
			break;
	}
	bookingHistories[numBookings].movieCode = choiceMovie;
	while (true)
	{
		cout << "Enter date code (0-8): ";
		choiceDate = inputAnInteger(0, 8);
		if (choiceDate != -1)
		{
			if (movies[choiceMovie].dates[choiceDate] == 1)
				break;
		}
	}
	bookingHistories[numBookings].dateCode = choiceDate;
	while (true)
	{
		cout << "Enter session time code (0-16): ";
		choiceTime = inputAnInteger(0, 16);
		if (choiceTime != -1)
		{
			if (movies[choiceMovie].sessionTimes[choiceTime] == 1)
				break;
		}
	}
	bookingHistories[numBookings].sessionTimeCode = choiceTime;

	cout << endl;
	cout << "Movie: " << movieNames[choiceMovie] << endl;
	cout << "Date: " << availableDates[choiceDate] << endl;
	cout << "Show Time: " << hours[choiceTime] << endl;
	cout << "Price: Adult-"<< movies[choiceMovie].prices[0] << ", Concession-" << movies[choiceMovie].prices[1] << ", Disability-" << movies[choiceMovie].prices[2] << ", Elderly-" << movies[choiceMovie].prices[3] << endl;
	cout << endl;

	while (true)
	{
		while (true)
		{
			cout << "Enter the number of adult tickets (0-6):";
			adultTicket = inputAnInteger(0, 6);
			if (adultTicket != -1)
				break;
		}
		bookingHistories[numBookings].numTickets[0] = adultTicket;
		while (true)
		{
			cout << "Enter the number of concession tickets (0-6):";
			concessionTicket = inputAnInteger(0, 6);
			if (concessionTicket != -1)
				break;
		}
		bookingHistories[numBookings].numTickets[1] = concessionTicket;
		while (true)
		{
			cout << "Enter the number of disability tickets (0-6):";
			disabilityTicket = inputAnInteger(0, 6);
			if (adultTicket != -1)
				break;
		}
		bookingHistories[numBookings].numTickets[2] = disabilityTicket;
		while (true)
		{
			cout << "Enter the number of elderly tickets (0-6):";
			elderlyTicket = inputAnInteger(0, 6);
			if (elderlyTicket != -1)
				break;
		}
		bookingHistories[numBookings].numTickets[3] = elderlyTicket;

		if (adultTicket != 0 || concessionTicket != 0 || disabilityTicket != 0 || elderlyTicket != 0)
			break;
	}

	cout << endl;
	display(movies, bookingHistories[numBookings]);
	cout << endl;
	selectSeats(bookingHistories, numBookings, movies);

	numBookings++;
}

void selectSeats(BookingInfo bookingHistories[], int numBookings, Movie movies[])
{
	int numTicket = bookingHistories[numBookings].numTickets[0] + bookingHistories[numBookings].numTickets[1] + bookingHistories[numBookings].numTickets[2] + bookingHistories[numBookings].numTickets[3];
	
	cout << " ";
	for (int i = 65; i < 77; i++)
	{
		cout << " " << (char)i;
	}
	cout << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << i << " ";
		for (int j = 0; j < 12; j++)
		{
			cout << movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][i][j] << " ";
		}
		cout << endl;
	}

	cout << "select " << numTicket << " seats (e.g. 0A): " << endl;
	for (int i = 0; i < numTicket; i++)
	{	
		string selectSeat;

		cout << "?";
		cin >> selectSeat;
		char t = selectSeat.at(0)-'0';
		char k = selectSeat.at(1)-'A';
		if (movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][t][k] == 1)
		{
			cout << "This seat bas been occupied. Please select another seat." << endl;
			i--;
		}
		else
		{
			bookingHistories[numBookings].seletedSeats[i][0] = selectSeat.at(0);
			bookingHistories[numBookings].seletedSeats[i][1] = selectSeat.at(1);

			movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][t][k] = 1;
		}
	}
	cout << endl;
	cout << "Sucessful!" << endl;
}

void display(Movie movies[], BookingInfo bookingHistory)
{
	cout << setw(27) << "No. of Tickets" << setw(7) << "Price" << setw(10) << "Subtotal" << endl;
	if (bookingHistory.numTickets[0]>0)
	{
		cout << "Adult" << setw(22) << bookingHistory.numTickets[0] << setw(7) << movies[bookingHistory.movieCode].prices[0] << setw(10) << bookingHistory.numTickets[0] * movies[bookingHistory.movieCode].prices[0] << endl;
	}
	if (bookingHistory.numTickets[1]>0)
	{
		cout << "Concession ticket" << setw(10) << bookingHistory.numTickets[1] << setw(7) << movies[bookingHistory.movieCode].prices[1] << setw(10) << bookingHistory.numTickets[1] * movies[bookingHistory.movieCode].prices[1] << endl;
	}
	if (bookingHistory.numTickets[2] > 0)
	{
		cout << "Disability ticket" << setw(10) << bookingHistory.numTickets[2] << setw(7) << movies[bookingHistory.movieCode].prices[2] << setw(10) << bookingHistory.numTickets[2] * movies[bookingHistory.movieCode].prices[2] << endl;
	}
	if (bookingHistory.numTickets[3] > 0)
	{
		cout << "Elderly" << setw(20) << bookingHistory.numTickets[3] << setw(7) << movies[bookingHistory.movieCode].prices[3] << setw(10) << bookingHistory.numTickets[3] * movies[bookingHistory.movieCode].prices[3] << endl << endl;
	}
	int total = bookingHistory.numTickets[0] * movies[bookingHistory.movieCode].prices[0] + bookingHistory.numTickets[1] * movies[bookingHistory.movieCode].prices[1] + bookingHistory.numTickets[2] + bookingHistory.numTickets[2] * movies[bookingHistory.movieCode].prices[2] + bookingHistory.numTickets[3] * movies[bookingHistory.movieCode].prices[3];
	cout << "Total Amount For Tickets: " << total << endl;
	cout << endl;
}

void displaySessionTimes(Movie movies[], char movieNames[][60], int numMovies,
	char availableDates[][12], int numDates)
{
	for (int j = 0; j < numMovies; j++)
	{
		cout << j <<". " << "Movie: " << movieNames[j] << endl;
		cout << "   Date: ";
		for (int i = 0; i < numDates; i++)
		{
			if (movies[j].dates[i] == 1)
			{
				cout << i <<"."<< availableDates[i] << ",  ";
			}
		}
		cout << endl << "   Session Time: ";
		for (int k = 0; k < sizeof(hours); k++)
		{
			if (movies[j].sessionTimes[k] == 1)
			{
				cout << k <<"."<< hours[k] << ",  ";
			}
		}
		cout << endl;
	}
}

void displayBookingHistory(BookingInfo bookingHistories[], int numBookings, Movie movies[],
	char movieNames[][60], char availableDates[][12], char email[])
{
	
	cout << "Booking History: " << endl << endl;
	bool find = false;
	for (int i = 0; i < numBookings; i++)
	{
		if (strcmp(email, bookingHistories[i].email) == 0)
		{
			int numTicket = bookingHistories[i].numTickets[0] + bookingHistories[i].numTickets[1] + bookingHistories[i].numTickets[2] + bookingHistories[i].numTickets[3];
			cout << "Movie: " << movieNames[bookingHistories[i].movieCode] << endl;
			cout << "Date: " << availableDates[bookingHistories[i].dateCode] << endl;
			cout << "Show Time: " << hours[bookingHistories[i].sessionTimeCode] << endl;
			cout << "Seats: ";
			for (int j = 0; j < numTicket; j++)
			{
				cout << bookingHistories[i].seletedSeats[j] << "  ";
			}

			cout << endl << endl;
			display(movies, bookingHistories[i]);
			find = true;
		}
	}
	if(!find)
	cout << "No Bookings!" << endl;
}

void saveMemberInfo(MemberRecord memberDetails[], int numMembers)
{
	ofstream outMemberInfo("Member Info.dat", ios::out|ios::binary);
	if (!outMemberInfo)
		return;

	for(int i=0;i<numMembers&&outMemberInfo;i++)
	{
		outMemberInfo.write(reinterpret_cast<char*>(&memberDetails[i]), sizeof(MemberRecord));
	}
}

void saveMovies(Movie movies[], int numMovies)
{
	ofstream outMovies("Movies.dat", ios::out|ios::binary);
	if (!outMovies)
		return;

	for(int i=0;i<numMovies&&outMovies;i++)
	{
		outMovies.write(reinterpret_cast<char*>(&movies[i]), sizeof(Movie));
	}
}

void saveBookingHistories(BookingInfo bookingHistories[], int numBookings)
{
	ofstream outBookingHistories("Booking Histories.dat", ios::out | ios::binary);
	if (!outBookingHistories)
		return;

	for(int i=0;i<numBookings&&outBookingHistories;i++)
	{
		outBookingHistories.write(reinterpret_cast<char*>(&bookingHistories[i]), sizeof(BookingInfo));
	}
}

void memberFileSetted(MemberRecord memberDetails[], int numMembers, int recordNumber, BookingInfo bookingHistories[], int &numBookings, 
						Movie movies[], char movieNames[][60], int numMovies, char availableDates[][12], int numDates, char email[])
{
	while (true)
	{
		cout << "Enter your choice:" << endl;
		cout << "1. Account Information" << endl;
		cout << "2. Buy Tickets" << endl;
		cout << "3. My Bookings" << endl;
		cout << "4. Sign Out\n?";

		int choice = inputAnInteger(1, 4);
		cout << endl;
		switch (choice)
		{
		case 1:
			accountInfor(memberDetails, numMembers, recordNumber);
			break;

		case 2:
			buyTickets(bookingHistories, numBookings, movies, movieNames, numMovies, availableDates, numDates, email);
			break;

		case 3:
			displayBookingHistory(bookingHistories, numBookings, movies, movieNames, availableDates, email);
			break;

		case 4:
			cout << "Thank you...\n\n";
			break;

		default:
			cout << "Input Error! Please try again.";
			break;
		}
		if (choice == 4)
			break;
	}
}
#include<iostream>
#include<string>
#include<stack>
#include <vector>

using namespace std;
#define MAXCITY 30
#define MAXFLIGHT 100
#define ROUNDTRIP 0
#define ONEWAY 1
#define INF 999999 


struct flightnode {
    int flightno;            	
    char *startcity;          
    int timedepart;
    char *destinationcity;          	
    int timearrival;           	
    int noofpassengers;       	
    flightnode *nextdeparture; 	
    flightnode *nextarrival; 	
};
//new char[strlen(departCity)+1].''  for city names//
               
struct citylistnode {
    char *cityname;           	   
    flightnode *nextdeparture;    
    flightnode *nextarrival;      
};

    citylistnode citylist[MAXCITY]; 




             
struct flightlistnode {
    int flightNo;             
    flightnode *flight;        
};

 flightlistnode flightlist[MAXFLIGHT];  

struct routeNode {
    int Day;
    int nHops;
    int FlightNo[10];
};

struct reservationNode {
    char *firstName;
    char *lastName;
    int tripType;
    routeNode routeForward;
    routeNode routeBack;
    reservationNode *nextReserve;
};

reservationNode *reserveHead = nullptr;
reservationNode *reserveTail = nullptr;


//improvemnt if bigger dataset

//set max city to 31 odd to prevent collision 
 int Hash(const char *cityName) {
    int hash = 0;
    while (*cityName) {
        hash = (hash*31 + *cityName) % MAXCITY;
        cityName++;
    }
    return hash;
}





void addcity(const char *cityname) {
    int index = Hash(cityname);

    if (!citylist[index].cityname)
    
     {
        citylist[index].cityname = new char[strlen(cityname) + 1];
        strcpy(citylist[index].cityname, cityname);
        citylist[index].nextdeparture = nullptr;
        citylist[index].nextarrival = nullptr;
    }
}




flightnode* makeflightnode(int flightno, const char *startcity, int timedepart, const char *destinationcity, int timearrival) {
    
    
    flightnode *newnode = new flightnode;

    newnode->flightno = flightno;

    newnode->startcity = new char[strlen(startcity) + 1];
    strcpy(newnode->startcity, startcity);
    newnode->timedepart = timedepart;

    newnode->destinationcity = new char[strlen(destinationcity) + 1];
    strcpy(newnode->destinationcity, destinationcity);
    newnode->timearrival = timearrival;


    newnode->noofpassengers = 0;
    newnode->nextdeparture = nullptr;
    newnode->nextarrival = nullptr;

    return newnode;
}


void linkflight(flightnode *flight) {
    int depindex = Hash(flight->startcity);
    int arrindex = Hash(flight->destinationcity);

    
    flight->nextdeparture = citylist[depindex].nextdeparture;
    citylist[depindex].nextdeparture = flight;

    
    flight->nextarrival = citylist[arrindex].nextarrival;
    citylist[arrindex].nextarrival = flight;
}


void displaydeparturelist(const char *cityname) {
    int index = Hash(cityname);
    flightnode *current = citylist[index].nextdeparture;

    vector<flightnode*> flights;

    while (current) {
        flights.push_back(current);
        current = current->nextdeparture;
    }

    for (size_t i = 0; i < flights.size() - 1; i++) {
        for (size_t j = 0; j < flights.size() - i - 1; j++) {
            if (flights[j]->timedepart > flights[j + 1]->timedepart) {
                swap(flights[j], flights[j + 1]);
            }
        }
    }

    cout << "Departures from " << cityname << ":\n";
    for (size_t i = 0; i < flights.size(); i++) {
        cout << "Flight " << flights[i]->flightno << " to " << flights[i]->destinationcity
             << " at " << flights[i]->timedepart << "\n";
    }
}

void displayarrivallist(const char *cityname) {
    int index = Hash(cityname);
    flightnode *current = citylist[index].nextarrival;

    vector<flightnode*> flights;

    while (current) {
        flights.push_back(current);
        current = current->nextarrival;
    }

    for (size_t i = 0; i < flights.size() - 1; i++) {
        for (size_t j = 0; j < flights.size() - i - 1; j++) {
            if (flights[j]->timearrival > flights[j + 1]->timearrival) {
                swap(flights[j], flights[j + 1]);
            }
        }
    }

    cout << "\nArrivals at " << cityname << ":\n";
    for (size_t i = 0; i < flights.size(); i++) {
        cout << "Flight No: " << flights[i]->flightno << ", Departure: " << flights[i]->startcity 
             << " at " << flights[i]->timedepart << " (Arrival: " << flights[i]->timearrival << ")\n";
    }
}




void displaycities() {
    cout << "Cities :\n";
    for (int i = 0; i < MAXCITY; i++) {
        if (citylist[i].cityname) {
            cout << "- " << citylist[i].cityname << "\n";
        }
    }
}


reservationNode* MakeReserveNode(const char *firstName, const char *lastName, int tripType, const routeNode &routeForward, const routeNode &routeBack) {
    reservationNode *newNode = new reservationNode;
    newNode->firstName = new char[strlen(firstName) + 1];
    strcpy(newNode->firstName, firstName);
    newNode->lastName = new char[strlen(lastName) + 1];
    strcpy(newNode->lastName, lastName);
    newNode->tripType = tripType;
    newNode->routeForward = routeForward;
    newNode->routeBack = routeBack;
    newNode->nextReserve = nullptr;
    return newNode;
}

void AddReservation(const char *firstName, const char *lastName, int tripType, const routeNode &routeForward, const routeNode &routeBack) {
    reservationNode *newReservation = MakeReserveNode(firstName, lastName, tripType, routeForward, routeBack);
    if (!reserveHead) {
        reserveHead = reserveTail = newReservation;
    } else {
        reserveTail->nextReserve = newReservation;
        reserveTail = newReservation;
    }
}

void PrintReservation(reservationNode *pReserve) {
    if (!pReserve) {
        cout << "No reservation found." << endl;
        return;
    }
    cout << "Passenger Name: " << pReserve->firstName << " " << pReserve->lastName << endl;
    cout << "Trip Type: " << (pReserve->tripType == ROUNDTRIP ? "Round Trip" : "One Way") << endl;
    cout << "Forward Route Hops: " << pReserve->routeForward.nHops << endl;
    if (pReserve->tripType == ROUNDTRIP) {
        cout << "Return Route Hops: " << pReserve->routeBack.nHops << endl;
    }
    cout << "--------------------\n";
}

void PrintAllReservations() {
    cout << "\nAll Reservations:\n";
    cout << "=================\n";
    reservationNode *current = reserveHead;
    while (current) {
        PrintReservation(current);
        current = current->nextReserve;
    }
}
void PrintPassengers(int flightNo) {
    cout << "\nPassengers on Flight " << flightNo << ":\n";
    cout << "===============================\n";
    reservationNode *current = reserveHead;
    bool found = false;
    while (current) {
        for (int i = 0; i < current->routeForward.nHops; i++) {
            if (current->routeForward.FlightNo[i] == flightNo) {
                cout << current->firstName << " " << current->lastName << endl;
                found = true;
                break;
            }
        }
        current = current->nextReserve;
    }
    if (!found) {
        cout << "No passengers found on this flight." << endl;
    }
}

void PrintSchedule() {
    char firstName[50], lastName[50];
    cout << "Enter passenger's first name: ";
    cin >> firstName;
    cout << "Enter passenger's last name: ";
    cin >> lastName;

    reservationNode *current = reserveHead;
    while (current) {
        if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0) {
            PrintReservation(current);
            return;
        }
        current = current->nextReserve;
    }
    cout << "No reservation found for " << firstName << " " << lastName << "." << endl;
}

void DeleteReservation(const char *firstName, const char *lastName) {
    reservationNode *current = reserveHead, *prev = nullptr;
    while (current) {
        if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0) {
            if (prev) {
                prev->nextReserve = current->nextReserve;
            } else {
                reserveHead = current->nextReserve;
            }
            if (current == reserveTail) {
                reserveTail = prev;
            }
            delete[] current->firstName;
            delete[] current->lastName;
            delete current;
            cout << "Reservation deleted successfully.\n";
            return;
        }
        prev = current;
        current = current->nextReserve;
    }
    cout << "Reservation not found.\n";
}

void DFS(int cityIndex, bool visited[])
 { 
 
   
    
	 if (visited[cityIndex]) return;
   
      visited[cityIndex] = true;
    
	     cout << "- " << citylist[cityIndex].cityname << "\n";

    flightnode *currentFlight = citylist[cityIndex].nextdeparture;
    
    while (currentFlight) {
        
		int nextCityIndex = Hash(currentFlight->destinationcity);
         
		 if (!visited[nextCityIndex]) {
           
		    DFS(nextCityIndex, visited);
        }
       
	     currentFlight = currentFlight->nextdeparture;
    }
}

void displayreachablecity(char *city) {
     
	  
	       bool visited[MAXCITY] = {false};
    
	int startIndex = Hash(city);

    if (citylist[startIndex].cityname == NULL || strcmp(citylist[startIndex].cityname, city) != 0)
	 {
     
	     cout << "City \"" << city << "\" is not found in the database.\n";
     
	       return;
    }

    
	  cout << "Cities reachable from " << city << ":\n";
    
	
	DFS(startIndex, visited);
}



void shortestroutedisplay(char *departCity, char *arrivalCity) {
    int start = Hash(departCity);
    
       
	   int end = Hash(arrivalCity);

    if (citylist[start].cityname == NULL || strcmp(citylist[start].cityname, departCity) != 0 || citylist[end].cityname == NULL || strcmp(citylist[end].cityname, arrivalCity) != 0) {
        
		cout << "One or both cities are not found in the database"<<endl;
        
		  return;
    }

    
	   int distance[MAXCITY],  parent[MAXCITY];
   
          bool visited[MAXCITY] = {false};

    for (int i = 0; i < MAXCITY; i++) 
	{
         
		 distance[i] = INF;
        
		parent[i] = -1;
    }
      
	     distance[start] = 0;

    for (int i = 0; i < MAXCITY; i++) 
	{
    
	    int distanceminimum = INF, currentNode = -1;
        for (int j = 0; j < MAXCITY; j++) {
            if (!visited[j] && distance[j] < distanceminimum) {
                distanceminimum = distance[j];
                currentNode = j;
            }
        }

        if (currentNode == -1) 
		break;
        
		visited[currentNode] = true;


        flightnode *currentFlight = citylist[currentNode].nextdeparture;
      
	    while (currentFlight)
		 {
             int neighbor = Hash(currentFlight->destinationcity);
            
			if (!visited[neighbor] && distance[currentNode] + (currentFlight->timearrival - currentFlight->timedepart) < distance[neighbor])
			 {
            
			     distance[neighbor] = distance[currentNode] + (currentFlight->timearrival - currentFlight->timedepart);
             
			    parent[neighbor] = currentNode;
            }
            currentFlight = currentFlight->nextdeparture;
        }
    }

    if (distance[end] == INF) {
        cout << "No path exists between " << departCity << " and " << arrivalCity << endl;
        
        return;
    }

    cout << "Shortest path from " << departCity << " to " << arrivalCity << endl;
    
    stack<int> path;
    
	int current = end;
    while (current != -1) {
      
	    path.push(current);
      
	    current = parent[current];
    }

    while (!path.empty()) {
        int cityIndex = path.top();
        path.pop();
        cout << citylist[cityIndex].cityname;
        if (!path.empty()) cout << " -> ";
    }
    
    cout << "\nTotal flight time: " << distance[end] << " minutes."<<endl;
}

void readflightdata()
{
        
    
    addcity("Karachi");
    addcity("Lahore");
    addcity("Islamabad");
    addcity("Quetta");
    addcity("Peshawar");
    addcity("Skardu");

    
    flightlist[0].flight = makeflightnode(101, "Karachi", 900, "Lahore", 1030);
    flightlist[1].flight = makeflightnode(102, "Karachi", 1200, "Islamabad", 1300);
    flightlist[2].flight = makeflightnode(103, "Karachi", 1400, "Peshawar", 1530);
    flightlist[3].flight = makeflightnode(104, "Karachi", 1600, "Skardu", 1830);
    flightlist[4].flight = makeflightnode(105, "Islamabad", 1100, "Peshawar", 1120);
    flightlist[5].flight = makeflightnode(106, "Lahore", 1400, "Peshawar", 1600);

    
    flightlist[6].flight = makeflightnode(107, "Lahore", 1100, "Karachi", 1230);
    flightlist[7].flight = makeflightnode(108, "Islamabad", 1400, "Karachi", 1500);
    flightlist[8].flight = makeflightnode(109, "Peshawar", 1600, "Karachi", 1730);
    flightlist[9].flight = makeflightnode(110, "Skardu", 1900, "Karachi", 2130);
    flightlist[10].flight = makeflightnode(111, "Peshawar", 1300, "Islamabad", 1320);
    flightlist[11].flight = makeflightnode(112, "Peshawar", 1700, "Lahore", 1900);

    
    for (int i = 0; i < 12; i++) {
        linkflight(flightlist[i].flight);
    }

}



void findAllRoutes(int current, int end, vector<int> &currentRoute, vector<vector<int>> &allRoutes, bool visited[]) 
{
    if (current == end)
        {
            allRoutes.push_back(currentRoute); 
            return;
        }

    visited[current] = true;

    flightnode *currentFlight = citylist[current].nextdeparture;
    while (currentFlight)
        {
            int nextCityIndex = Hash(currentFlight->destinationcity);
            if (!visited[nextCityIndex])
            {
                currentRoute.push_back(nextCityIndex);
                findAllRoutes(nextCityIndex, end, currentRoute, allRoutes, visited);
                currentRoute.pop_back(); 
            }
            currentFlight = currentFlight->nextdeparture;
        }

    visited[current] = false; 
}


void displayAllRoutes(const char *departCity, const char *arrivalCity) {
    int start = Hash(departCity);
    int end = Hash(arrivalCity);

    if (citylist[start].cityname == NULL || strcmp(citylist[start].cityname, departCity) != 0 || citylist[end].cityname == NULL || strcmp(citylist[end].cityname, arrivalCity) != 0) 
        {
        cout << "One or both cities are not found in the database." << endl;
        return;
        }

    vector<int> currentRoute = {start};
    vector<vector<int>> allRoutes;
    bool visited[MAXCITY] = {false};

    findAllRoutes(start, end, currentRoute, allRoutes, visited);

    if (allRoutes.empty()) 
    {
        cout << "No routes found between " << departCity << " and " << arrivalCity << "." << endl;
        return;
    }

    cout << "All possible routes from " << departCity << " to " << arrivalCity << ":" << endl;

    for (vector<int> route : allRoutes) 
    {
        for (size_t i = 0; i < route.size(); ++i)
        {
            cout << citylist[route[i]].cityname;
            if (i < route.size() - 1)
            {
                cout << " -> ";
            }

        }
    cout << endl;
}

}

void adminmode() {
    const int adminPassword = 1234; 
    int enteredPassword;

    cout << "Enter admin password: ";
    cin >> enteredPassword;

    if (enteredPassword != adminPassword) {
        cout << "Incorrect password. Access denied.\n";
        return;
    }

    cout << "\nAdmin Mode Access Granted\n";

    while (true) {
        cout << "\nAdmin Options:\n";
        cout << "1. Add City\n";
        cout << "2. Add Flight\n";
        cout << "3. Display All Cities\n";
        cout << "4. Exit Admin Mode\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: 
            {
                cout << "Enter city name: ";
                string cityName;
                cin.ignore();
                getline(cin, cityName);
                addcity(cityName.c_str());
                cout << "City \"" << cityName << "\" added successfully.\n";
                break;
            }
            case 2: {
                int flightNo, timeDepart, timeArrival;
                string startCity, destinationCity;

                cout << "Enter flight number: ";
                cin >> flightNo;
                cin.ignore();

                cout << "Enter departure city: ";
                getline(cin, startCity);

                cout << "Enter departure time (in 24-hour format, e.g., 1400): ";
                cin >> timeDepart;
                cin.ignore();

                cout << "Enter destination city: ";
                getline(cin, destinationCity);

                cout << "Enter arrival time (in 24-hour format, e.g., 1600): ";
                cin >> timeArrival;

    
                if (!citylist[Hash(startCity.c_str())].cityname || !citylist[Hash(destinationCity.c_str())].cityname) {
                    cout << "One or both cities do not exist. Please add the cities first.\n";
                    break;
                }

                
                flightnode* newFlight = makeflightnode(flightNo, startCity.c_str(), timeDepart, destinationCity.c_str(), timeArrival);
                linkflight(newFlight);
                cout << "Flight " << flightNo << " from " << startCity << " to " << destinationCity << " added successfully.\n";
                break;
            }
            case 3:
                displaycities();
                break;
            case 4:
                cout << "Exiting Admin Mode.\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
}





#include <fstream>

// Function to save passenger details to a file
void SavePassengersToFile(int flightNo) {
    ofstream outFile("passenger_details.txt", ios::app); // Append mode
    if (!outFile) {
        cout << "Error opening file for writing.\n";
        return;
    }

    reservationNode *current = reserveHead;
    bool found = false;
    outFile << "Flight Number: " << flightNo << "\n";
    outFile << "Passengers:\n";

    while (current) {
        for (int i = 0; i < current->routeForward.nHops; i++) {
            if (current->routeForward.FlightNo[i] == flightNo) {
                outFile << current->firstName << " " << current->lastName << "\n";
                found = true;
                break;
            }
        }
        current = current->nextReserve;
    }

    if (!found) {
        outFile << "No passengers found on this flight.\n";
    }

    outFile << "===============================\n";
    outFile.close();
    cout << "Passenger details saved to file successfully.\n";
}

// Function to read passenger details from a file
void ReadPassengersFromFile()
 {
    
    ifstream inFile("passenger_details.txt");
    if (!inFile) {

        cout << "Error opening file for reading.\n";

        return;
    }

    string line;

     cout << "\nReading Passenger Details from File:\n";
 
    cout << "=====================================\n";
  
     while (getline(inFile, line)) {
  
         cout << line << "\n";
   
    }
    inFile.close();
}



int main()
{
   readflightdata(); // Initialize cities and flights

    while (true) {
       
         cout << "\nMain Menu:\n";
       
         cout << "1. Display all cities\n";
           cout << "2. Display departures from a city\n";
        
           cout << "3. Display arrivals at a city\n";

 cout << "4. Display reachable cities from a city\n";
    
    cout << "5. Display shortest route between two cities\n";
         cout << "6. Display all routes between two cities\n";
     
           cout << "7. Admin mode\n";
     
      cout << "8. Add reservation\n";
        
        cout << "9. Display all reservations\n";
      
           cout << "10. Display a passenger's schedule\n";
      
          cout << "11. Delete a reservation\n";
      
        cout << "12. Exit\n";
       
        cout << "Enter your choice: ";

        int choice;
       
        cin >> choice;

        switch (choice) 
        {
           
            case 1:
                displaycities();
                break;

            case 2: 
            {
                cout << "Enter city name: ";
                string city;
              
                cin.ignore();
              
                getline(cin, city);
              
                displaydeparturelist(city.c_str());
              
                break;
            }

            case 3: {
                cout << "Enter city name: ";
                string city;
                cin.ignore();
                getline(cin, city);
                displayarrivallist(city.c_str());
                break;
            }

            case 4: {
                cout << "Enter city name: ";
                string city;
                
                cin.ignore();
              
                getline(cin, city);
                  displayreachablecity((char*)city.c_str());
              
                break;
            }

            case 5: {
                 
                 cout << "Enter departure city: ";
                   
                    string departCity;
                
                 cin.ignore();
                
                 getline(cin, departCity);

                
                 cout << "Enter arrival city: ";
                
                 string arrivalCity;
                
                 getline(cin, arrivalCity);

                shortestroutedisplay((char*)departCity.c_str(), (char*)arrivalCity.c_str());
            
                break;
            
            }

            case 6: {
                cout << "Enter departure city: ";
                
                string departCity;
                
                 cin.ignore();
                
                getline(cin, departCity);

                cout << "Enter arrival city: ";
                 
                 string arrivalCity;
            
            getline(cin, arrivalCity);

                displayAllRoutes(departCity.c_str(), arrivalCity.c_str());
                break;
            }

            case 7:
                adminmode();
                break;

            case 8: {
                cout << "Enter first name: ";
                string firstName;
                cin.ignore();
                getline(cin, firstName);

                cout << "Enter last name: ";
                string lastName;
                getline(cin, lastName);

                cout << "Enter trip type (0 for Round Trip, 1 for One Way): ";
                int tripType;
                cin >> tripType;

                routeNode routeForward, routeBack;
                routeForward.nHops = 0;
                routeBack.nHops = 0;

                if (tripType == ROUNDTRIP) {
                    cout << "Enter number of hops for forward route: ";
                    cin >> routeForward.nHops;
                    cout << "Enter flight numbers for forward route: ";
                    for (int i = 0; i < routeForward.nHops; i++) {
                        cin >> routeForward.FlightNo[i];
                    }

                    cout << "Enter number of hops for return route: ";
                    cin >> routeBack.nHops;
                    cout << "Enter flight numbers for return route: ";
                    for (int i = 0; i < routeBack.nHops; i++) {
                        cin >> routeBack.FlightNo[i];
                    }
                } else {
                    cout << "Enter number of hops for the route: ";
                    cin >> routeForward.nHops;
                    cout << "Enter flight numbers for the route: ";
                    for (int i = 0; i < routeForward.nHops; i++) {
                        cin >> routeForward.FlightNo[i];
                    }
                }

                AddReservation(firstName.c_str(), lastName.c_str(), tripType, routeForward, routeBack);
                break;
            }

            case 9:
                PrintAllReservations();
                break;

            case 10:
                PrintSchedule();
                break;

            case 11: {
                cout << "Enter first name: ";
                string firstName;
                cin.ignore();
                getline(cin, firstName);

                cout << "Enter last name: ";
                string lastName;
                getline(cin, lastName);

                DeleteReservation(firstName.c_str(), lastName.c_str());
                break;
            }

            case 12:
                cout << "Exiting program. Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
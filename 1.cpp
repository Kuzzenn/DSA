#include <bits/stdc++.h>
using namespace std;
#define MAXCITY 30
#define MAXFLIGHT 100
#define ROUNDTRIP 0
#define ONEWAY 1
#define INF 999999 // value to represent infinity

//setting flight databse;

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
        hash = (hash*31 + *cityName) % MAXCITY;//multiply by random number
        cityName++;
    }
    return hash;
}



//adding city

void addcity(const char *cityname) {
    int index = Hash(cityname);

    if (!citylist[index].cityname)//condition to check if city already exists
    
     {
        citylist[index].cityname = new char[strlen(cityname) + 1];
        strcpy(citylist[index].cityname, cityname);
        citylist[index].nextdeparture = nullptr;
        citylist[index].nextarrival = nullptr;
    }
}


//setting a flight

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

    
    flight->nextdeparture = citylist[depindex].nextdeparture;// Link to departure list
    citylist[depindex].nextdeparture = flight;

    
    flight->nextarrival = citylist[arrindex].nextarrival;// Link to arrival list
    citylist[arrindex].nextarrival = flight;
}


void displaydeparturelist(const char *cityname) {
    int index = Hash(cityname);
    flightnode *current = citylist[index].nextdeparture;

    cout << "Departures from " << cityname << ":\n";
    while (current) {
        cout << "Flight " << current->flightno << " to " << current->destinationcity
             << " at " << current->timedepart << "\n";
        current = current->nextdeparture;
    }
}

void displayarrivallist(const char *cityname) {
    int index = Hash(cityname);
    flightnode *current = citylist[index].nextarrival;

    cout << "\nArrivals at " << cityname << ":\n";
    while (current) {
        cout << "Flight No: " << current->flightno << ", Departure: " << current->startcity 
                  << " at " << current->timedepart << " (Arrival: " << current->timearrival << ")\n";
        current = current->nextarrival;
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
//  display cities from one reachable city to another using DFS
void displayreachablecity(char *city) {
	
	
        bool isvisit[MAXCITY] = {false};  // checks and tracks visited cities by traversing
  
  
        int firstindex = Hash(city);

    if (citylist[firstindex].cityname == nullptr || strcmp (citylist[firstindex].cityname, city) != 0) {
        
	    	cout << "City " << city << " is not found in the database"<<endl;
     
	 
	    return;
    }

    cout << "Cities reachable from " << city << ":"<<endl;

    // DFS search
    void DFS(int cityIndex) 
	  { 
           if (isvisit[cityIndex]) 
		   
		   return;  
       
	       isvisit[cityIndex] = true;

        cout << "- " << citylist[cityIndex].cityname << endl;  // Display city

           flightnode *currentFlight = citylist[cityIndex].nextdeparture;
           
         while (currentFlight)
		  {
		  	 int nextCityIndex = Hash(currentFlight->destinationcity);
		  	 
            if (!isvisit[nextCityIndex])
			 {
			 	
                DFS(nextCityIndex);
            }
            
            currentFlight = currentFlight->nextdeparture;
        }
    }

    DFS(firstindex);  // Start DFS from the given city
}

// Display shortest path //NEED TO MAKE CHANGES!!!!!!!!!!!!!!
void DisplayShortestPath(char *departCity,  char *arrivalCity) {
    
int start = Hash(departCity);
    
      int end = Hash(arrivalCity);

    if (citylist[start].cityname == nullptr || strcmp(citylist[start].cityname, departCity) != 0 ||
        
            
            citylist[end].cityname == nullptr || strcmp(citylist[end].cityname, arrivalCity) != 0) {
       
        cout << "One or both cities are not found in the database.\n";
       
       
        return;
   
   
    }

    int distance[MAXCITY], parent[MAXCITY];

          bool visited[MAXCITY] = {false};

    for (int i = 0; i < MAXCITY; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }
    distance[start] = 0;


    for (int i = 0; i < MAXCITY; i++) {
        
         int minDist = INF, currentNode = -1;
       
       
           for (int j = 0; j < MAXCITY; j++) {
          
            if (!visited[j] && distance[j] < minDist) {
          
                minDist = distance[j];
          
                currentNode = j;
          
            }
        
        }

        if (currentNode == -1) break;

        visited[currentNode] = true;

        flightnode *currentFlight = citylist[currentNode].nextdeparture;

        while (currentFlight) {
          
            int neighbor = Hash(currentFlight->destinationcity);
          
            if (!visited[neighbor] && distance[currentNode] + (currentFlight->timearrival - currentFlight->timedepart) < distance[neighbor]) {
          
                   distance[neighbor] = distance[currentNode] + (currentFlight->timearrival - currentFlight->timedepart);
             
                   parent[neighbor] = currentNode;
            
            }
           
            currentFlight = currentFlight->nextdeparture;
        
        }
    }

    if (distance[end] == INF) {
       
        cout << "No path exists between " << departCity << " and " << arrivalCity << ".\n";
       
        return;
    
    }

    
     cout << "Shortest path from " << departCity << " to " << arrivalCity << ":\n";
    
    int current = end;
    
    stack<int> path;
    
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
    
    
    cout << "\nTotal flight time: " << distance[end] << " minutes.\n";
}


int main()
{
    //option for admin to add city
    // Adding cities
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
    flightlist[4].flight = makeflightnode(105, "Islamabad", 1100, "Peshawar", 1230);
    flightlist[5].flight = makeflightnode(106, "Lahore", 1400, "Peshawar", 1600);

    
    flightlist[6].flight = makeflightnode(107, "Lahore", 1100, "Karachi", 1230);
    flightlist[7].flight = makeflightnode(108, "Islamabad", 1400, "Karachi", 1500);
    flightlist[8].flight = makeflightnode(109, "Peshawar", 1600, "Karachi", 1730);
    flightlist[9].flight = makeflightnode(110, "Skardu", 1900, "Karachi", 2130);
    flightlist[10].flight = makeflightnode(111, "Peshawar", 1300, "Islamabad", 1430);
    flightlist[11].flight = makeflightnode(112, "Peshawar", 1700, "Lahore", 1900);

    
    for (int i = 0; i < 12; i++) {
        linkflight(flightlist[i].flight);
    }

    
    displaycities();

    
    displaydeparturelist("Karachi");
    displayarrivallist("Karachi");

    
    displaydeparturelist("Skardu");
    displayarrivallist("Skardu");

// Display cities reachable from Karachi
    displayreachablecity("Karachi");

        DisplayShortestPath("Karachi", "Peshawar");


    return 0;
    
    
}
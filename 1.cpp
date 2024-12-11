#include <bits/stdc++.h>
using namespace std;


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

#define MAXCITY 30                
struct citylistnode {
    char *cityname;           	   
    flightnode *nextdeparture;    
    flightnode *nextarrival;      
};

    citylistnode citylist[MAXCITY]; 




#define MAXFLIGHT 100             
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

 //hashing function

 int Hash(const char *cityname) {
    int hash = 0;
    while (*cityname) {
        hash = (hash + *cityname) % MAXCITY;
        cityname++;
    }
    return hash;
}

//improvemnt if bigger dataset
/*
set max city to 31 odd to prevent collision 
 int Hash(const char *cityName) {
    int hash = 0;
    while (*cityName) {
        hash = (hash*31 + *cityName) % MAXCITY;//multiply by random number
        cityName++;
    }
    return hash;
}

*/

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



















 





int main()
{
    //option for admin to add city
    // Hardcoded Cities
    addcity("Karachi");
    addcity("Lahore");
    addcity("Islamabad");
    addcity("Quetta");
    addcity("Peshawar");

    
   //making flights
flightlist[0].flight = makeflightnode(101, "Karachi", 900, "Lahore", 1030);
flightlist[1].flight = makeflightnode(102, "Karachi", 1200, "Islamabad", 1300);
flightlist[2].flight = makeflightnode(103, "Lahore", 1400, "Quetta", 1530);
flightlist[3].flight = makeflightnode(104, "Islamabad", 1800, "Peshawar", 1900);
flightlist[4].flight = makeflightnode(105, "Quetta", 2000, "Karachi", 2100);


    // Linking the flights 
    for (int i = 0; i < 5; i++) {
        linkflight(flightlist[i].flight);
    }

    
    displaycities();

    
    displaydeparturelist("Karachi");
    displayarrivallist("Karachi");

    return 0;

    
    
}
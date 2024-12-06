#include<iostream>
using namespace std;

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
//''new char[strlen(departCity)+1].''  for city names//

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
 





int main()
{
    flightnode *flight1 = new flightnode;
flight1->flightno = 101;
flight1->startcity = new char[strlen("Karachi") + 1];
strcpy(flight1->startcity, "Karachi");
flight1->timedepart = 900;
flight1->destinationcity = new char[strlen("Lahore") + 1];
strcpy(flight1->destinationcity, "Lahore");
flight1->timearrival = 1030;
flight1->noofpassengers = 100;
flight1->nextdeparture = nullptr;
flight1->nextarrival = nullptr;

flightnode *flight2 = new flightnode;
flight2->flightno = 202;
flight2->startcity = new char[strlen("Karachi") + 1];
strcpy(flight2->startcity, "Karachi");
flight2->timedepart = 1400;
flight2->destinationcity = new char[strlen("Dubai") + 1];
strcpy(flight2->destinationcity, "Dubai");
flight2->timearrival = 1600;
flight2->noofpassengers = 80;
flight2->nextdeparture = nullptr;
flight2->nextarrival = nullptr;

// connection of flights from karachi
flight1->nextdeparture = flight2;

    
    
}
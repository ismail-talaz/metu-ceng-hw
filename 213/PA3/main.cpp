#include "CENGFlight.h"
#include "MultiGraph.h"
#include "HashTable.h"
#include <iostream>
using namespace std;
int main(int argc, const char* argv[])
{
    CENGFlight system("input.txt");
    vector<string> airlineNames;
    system.FindSisterAirlines(airlineNames,"AnadoluJet","Istanbul(IST)");

    
    






    return 0;
}

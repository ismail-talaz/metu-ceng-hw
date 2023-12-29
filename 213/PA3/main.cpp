#include "CENGFlight.h"
#include "MultiGraph.h"
#include "HashTable.h"
#include <iostream>
using namespace std;
int main(int argc, const char* argv[])
{
    /*CENGFlight system("input.txt");
    vector<string> airlineNames;
    system.FindSisterAirlines(airlineNames,"BoraJet","Istanbul(IST)");*/

    MultiGraph myGraph("input.txt");
    myGraph.RemoveVertex("Ankara(ESB)");
    myGraph.PrintEntireGraph();

    /*myGraph.InsertVertex("Bursa");
    myGraph.InsertVertex("İstanbul");
    myGraph.InsertVertex("İzmir");
    myGraph.InsertVertex("Ankara");
    myGraph.InsertVertex("Adana");
    myGraph.AddEdge("Ankara-Bursa Otoyolu","Ankara","Bursa",60.2,70.0);
    myGraph.AddEdge("İzmir-İstanbul Otoyolu","İzmir","İstanbul",60.2,70.0);
    myGraph.AddEdge("Adana-Ankara Otoyolu","Adana","Ankara",60.2,70.0);
    myGraph.AddEdge("İstanbul-Adana Otoyolu","İstanbul","Adana",60.2,70.0);
    myGraph.AddEdge("Bursa-İzmir Otoyolu","Bursa","İzmir",60.2,70.0);*/


    
    






    return 0;
}

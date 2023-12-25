#include "CENGFlight.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void CENGFlight::PrintCanNotHalt(const std::string& airportFrom,
                                 const std::string& airportTo,
                                 const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void CENGFlight::PrintCanNotResumeFlight(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void CENGFlight::PrintFlightFoundInCache(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  " is found in cache." << std::endl;
}

void CENGFlight::PrintFlightCalculated(const std::string& airportFrom,
                                       const std::string& airportTo,
                                       bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  "." << std::endl;
}

void CENGFlight::PrintPathDontExist(const std::string& airportFrom,
                                    const std::string& airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo <<"\"." << std::endl;
}

void CENGFlight::PrintSisterAirlinesDontCover(const std::string& airportFrom)
{
    std::cout << "Could not able to generate sister airline list from \""
              << airportFrom <<"\"." << std::endl;
}

void CENGFlight::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void CENGFlight::PrintCache()
{
    lruTable.PrintTable();
}

CENGFlight::CENGFlight(const std::string& flightMapPath)
    : navigationMap(flightMapPath)
{}

//=======================//
//          TODO         //
//=======================//
void CENGFlight::HaltFlight(const std::string& airportFrom,
                            const std::string& airportTo,
                            const std::string& airlineName)
{
    std::vector<float> information = navigationMap.getInformation(airportFrom,airportTo,airlineName);
    for(int i=0;i<4;i++){
        if(information[i]==-1){
            PrintCanNotHalt(airportFrom,airportTo,airlineName);
            return;
        }
    }
    navigationMap.RemoveEdge(airlineName,airportFrom,airportTo);
    HaltedFlight halted{airportFrom,airportTo,airlineName,information[2],information[3]};
    haltedFlights.push_back(halted);
    
}

    // (Direct Function call)
void CENGFlight::ContinueFlight(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName)
{
    bool found=false;
    for(int i=0;i<haltedFlights.size();i++){
        HaltedFlight current=haltedFlights[i];
        if(current.airportFrom == airportFrom && current.airportTo==airportTo && current.airline == airlineName){
            haltedFlights.erase(haltedFlights.begin()+i);
            navigationMap.AddEdge(airlineName,airportFrom,airportTo,current.w0,current.w1);
            found=true;
        }
    }
    if(!found){
        PrintCanNotResumeFlight(airportFrom,airportTo,airlineName);
    }
}

void CENGFlight::FindFlight(const std::string& startAirportName,
                            const std::string& endAirportName,
                            float alpha)
{
    std::vector<int> indices=navigationMap.getIndices(startAirportName,endAirportName);
    int startInt = indices[0];
    int endInt = indices[1];
    std::vector<int> path;
    if(alpha == 1){ // NOT COST WEIGHTED
       if(lruTable.Find(path,startInt,endInt,false,true)){
           PrintFlightFoundInCache(startAirportName,endAirportName,false);
           navigationMap.PrintPath(path,alpha,true);
       }
       else{
           PrintFlightCalculated(startAirportName,endAirportName,false);
           std::vector<int> ordered;
           navigationMap.HeuristicShortestPath(ordered,startAirportName,endAirportName,alpha);
           navigationMap.PrintPath(ordered,alpha,false);
           lruTable.Insert(ordered,false);
       }
    }
    else if(alpha == 0){ // COSTWEIGHTED
        if(lruTable.Find(path,startInt,endInt,true,true)){
           PrintFlightFoundInCache(startAirportName,endAirportName,true);
           navigationMap.PrintPath(path,alpha,true);
        }
        else{ 
           PrintFlightCalculated(startAirportName,endAirportName,true);
           std::vector<int> ordered;
           navigationMap.HeuristicShortestPath(ordered,startAirportName,endAirportName,alpha);
           navigationMap.PrintPath(ordered,alpha,true);
           lruTable.Insert(ordered,true);
        }
    }
    else{
        std::vector<int> ordered;
        navigationMap.HeuristicShortestPath(ordered,startAirportName,endAirportName,alpha);
        navigationMap.PrintPath(ordered,alpha,true);
    }
}

void CENGFlight::FindSpecificFlight(const std::string& startAirportName,
                                    const std::string& endAirportName,
                                    float alpha,
                                    const std::vector<std::string>& unwantedAirlineNames) const
{
    std::vector<int> ordered;
    navigationMap.FilteredShortestPath(ordered,startAirportName,endAirportName,alpha,unwantedAirlineNames);
    navigationMap.PrintPath(ordered,alpha,true);
}

void CENGFlight::FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const
{
    navigationMap.findComplement(airlineNames,startAirlineName,airportName);
}

int CENGFlight::FurthestTransferViaAirline(const std::string& airportName,
                                           const std::string& airlineName) const
{
    int index = navigationMap.getIndex(airportName);
    if(index == -1){return -1;}
    
    int count = navigationMap.MaxDepthViaEdgeName(airportName,airlineName);
    return count;
}
#ifndef MULTI_GRAPH_H
#define MULTI_GRAPH_H

#include <vector>
#include <string>

struct GraphEdge
{
    std::string name;       // Name of the vertex
    float       weight[2];  // Weights of the edge
                            // (used on shortest path)
    int         endVertexIndex;
};

struct GraphVertex
{
    std::vector<GraphEdge> edges; // Adjacency List
    std::string            name;  // Name of the vertex
};

class MultiGraph
{
    private:
    std::vector<GraphVertex>    vertexList;

    static float Lerp(float w0, float w1, float alpha);

    protected:
    public:
    // Constructors & Destructor
                MultiGraph();
                MultiGraph(const std::string& filePath);

    // Connect Vertices
    void        InsertVertex(const std::string& vertexName);
    void        RemoveVertex(const std::string& vertexName);

    // Connect Vertices
    void        AddEdge(const std::string& edgeName,
                        const std::string& vertexFromName,
                        const std::string& vertexToName,
                        float weight0, float weight1);
    void        RemoveEdge(const std::string& edgeName,
                           const std::string& vertexFromName,
                           const std::string& vertexToName);

    // Shortest Path Functions
    bool        HeuristicShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                      const std::string& vertexNameFrom,
                                      const std::string& vertexNameTo,
                                      float heuristicWeight) const;
    bool        FilteredShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                     const std::string& vertexNameFrom,
                                     const std::string& vertexNameTo,
                                     float heuristicWeight,
                                     const std::vector<std::string>& edgeNames) const;

    // Other functions
    int         BiDirectionalEdgeCount() const;
    int         MaxDepthViaEdgeName(const std::string& vertexName,
                                    const std::string& edgeName) const;
    
    std::vector<float> getInformation(const std::string vertexFromName,
                               const std::string vertexToName,
                               const std::string edgeName);
                               
    std::vector<int> getIndices(const std::string vertexFromName,
                               const std::string vertexToName);
    void findComplement(std::vector<std::string>& edgeNames, const std::string vertexFromName, const std::string edgeName) const;
                               
    int getIndex(const std::string vertexFromName) const;
    // Implemented Functions for Debugging
    void        PrintPath(const std::vector<int>& orderedVertexEdgeIndexList,
                          float heuristicWeight,
                          bool sameLine = false) const;
    void        PrintEntireGraph() const;
};

#include "MultiGraph.h"
#include "Exceptions.h"
#include "IntPair.h"
#include <iostream>
#include <iomanip>
#include <fstream>

//=======================//
// Implemented Functions //
//=======================//
MultiGraph::MultiGraph()
{}

MultiGraph::MultiGraph(const std::string& filePath)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    // Tokens
    std::string tokens[5];
    std::ifstream mapFile(filePath.c_str());

    if(!mapFile.is_open())
    {
        std::cout << "Unable to open " << filePath << std::endl;
        return;
    }

    // Read line by line
    std::string line;
    while (std::getline(mapFile, line))
    {
        // Empty Line Skip
        if(line.empty()) continue;
        // Comment Skip
        if(line[0] == '#') continue;

        // Tokenize the line
        int i = 0;
        std::istringstream stream(line);
        while(stream >> tokens[i]) i++;

        // Single token (Meaning it is a vertex)
        if(i == 1)
        {
            InsertVertex(tokens[0]);
        }
        // Exactly 5 tokens (Meaning it is an edge)
        else if(i == 5)
        {
            // Rename vars for readablity
            const std::string& vertexFromName = tokens[0];
            const std::string& vertexToName = tokens[1];
            const std::string& edgeName = tokens[2];
            float weight0 = static_cast<float>(std::atof(tokens[3].c_str()));
            float weight1 = static_cast<float>(std::atof(tokens[4].c_str()));
            AddEdge(edgeName, vertexFromName, vertexToName,
                    weight0, weight1);
        }
        else std::cerr << "Token Size Mismatch" << std::endl;
    }
}

void MultiGraph::PrintPath(const std::vector<int>& orderedVertexEdgeIndexList,
                           float heuristicWeight,
                           bool sameLine) const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this file !       //
    // ============================= //
    // Name is too long
    const std::vector<int>& ove = orderedVertexEdgeIndexList;
    // Invalid list
    // At least three items should be available
    if(ove.size() < 3) return;

    // Check vertex and an edge
    for(size_t i = 0; i < orderedVertexEdgeIndexList.size(); i += 2)
    {
        int vertexId = ove[i];
        if(vertexId >= static_cast<int>(vertexList.size()))
        {
            // Return if there is a bad vertex id
            std::cout << "VertexId " << vertexId
                      << " not found!" << std::endl;
            return;
        }


        const GraphVertex& vertex = vertexList[vertexId];
        std::cout << vertex.name;
        if(!sameLine) std::cout << "\n";
        // Only find and print the weight if next is available
        if(i == ove.size() - 1) break;
        int nextVertexId = ove[i + 2];
        if(nextVertexId >= static_cast<int>(vertexList.size()))
        {
            // Return if there is a bad vertex id
            std::cout << "VertexId " << vertexId
                    << " not found!" << std::endl;
            return;
        }

        // Find the edge between these two vertices
        int localEdgeId = ove[i + 1];
        if(localEdgeId >= static_cast<int>(vertex.edges.size()))
        {
            // Return if there is a bad vertex id
            std::cout << "EdgeId " << localEdgeId
                      << " not found in " << vertexId << "!" << std::endl;
            return;
        }

        const GraphEdge& edge = vertex.edges[localEdgeId];

        // Combine with heuristic (linear interpolation)
        float weight = Lerp(edge.weight[0], edge.weight[1],
                            heuristicWeight);

        std::cout << "-" << std::setfill('-')
                  << std::setw(4)
                  << weight << "->";
    }
    // Print endline on the last vertex if same line is set
    if(sameLine) std::cout << "\n";
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

void MultiGraph::PrintEntireGraph() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    for(size_t i = 0; i < vertexList.size(); i++)
    {
        const GraphVertex& v = vertexList[i];
        std::cout << v.name << "\n";
        for(size_t j = 0; j < v.edges.size(); j++)
        {
            const GraphEdge& edge = v.edges[j];

            // List the all vertex names and weight
            std::cout << "    -"
                      << std::setfill('-')
                      << std::setw(4) << edge.weight[0]
                      << "-"
                      << std::setw(4) << edge.weight[1]
                      << "-> ";
            std::cout << vertexList[edge.endVertexIndex].name;
            std::cout << " (" << edge.name << ")" << "\n";
        }
    }
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

//=======================//
//          TODO         //
//=======================//
float MultiGraph::Lerp(float w0, float w1, float alpha)
{
    float result;
    result = w0*(1-alpha)+alpha*w1;
    return result;
}

void MultiGraph::InsertVertex(const std::string& vertexName)
{
    for(int i=0;i<vertexList.size();i++){
        if(vertexName==vertexList[i].name){
            throw DuplicateVertexException(vertexName);
        }
    }
    GraphVertex newVertex;
    newVertex.name = vertexName;
    vertexList.push_back(newVertex);
}

void MultiGraph::RemoveVertex(const std::string& vertexName)
{
    int removal=-1;
    for(int i=0;i<vertexList.size();i++){
        if(vertexName == vertexList[i].name){
            removal=i;
        }
    }
    if(removal == -1){
        throw VertexNotFoundException(vertexName);
    }
    
    vertexList.erase(vertexList.begin() + removal);
    for(int i=0 ; i<vertexList.size();i++){
        int j=0;
        std::vector<GraphEdge> currentEdges = vertexList[i].edges;
        while(j<currentEdges.size()){
            if(currentEdges[j].endVertexIndex==removal){
                 currentEdges.erase(currentEdges.begin() + j);
            }
            else{
                j++;
            }
        }
    }
    
    for(int i=0 ; i<vertexList.size();i++){
        int j=0;
        std::vector<GraphEdge> currentEdges = vertexList[i].edges;
        while(j<currentEdges.size()){
            if(currentEdges[j].endVertexIndex>removal){
                 currentEdges[j].endVertexIndex-=1;
            }
            else{
                j++;
            }
        }
    }
}

void MultiGraph::AddEdge(const std::string& edgeName,
                         const std::string& vertexFromName,
                         const std::string& vertexToName,
                         float weight0, float weight1)
{
    int start=-1,end=-1;
    for(int i=0;i<vertexList.size();i++){
        if(vertexList[i].name==vertexFromName){
            start=i;
        }
        if(vertexList[i].name==vertexToName){
            end=i;
        }
    }
    
    if(start == -1){
        throw VertexNotFoundException(vertexFromName);
    }
    if(end == -1){
        throw VertexNotFoundException(vertexToName);
    }
    
    
    for(int i=0;i<vertexList[start].edges.size();i++){
        if(vertexList[start].edges[i].name == edgeName && vertexList[start].edges[i].endVertexIndex == end){
            throw SameNamedEdgeException(edgeName,vertexFromName,vertexToName);
        }
    }
    
    GraphEdge newEdge;
    newEdge.name=edgeName;
    newEdge.endVertexIndex=end;
    newEdge.weight[0]=weight0;
    newEdge.weight[1]=weight1;
    vertexList[start].edges.push_back(newEdge);
}

void MultiGraph::RemoveEdge(const std::string& edgeName,
                            const std::string& vertexFromName,
                            const std::string& vertexToName)
{
    int start=-1,end=-1;
    for(int i=0;i<vertexList.size();i++){
        if(vertexList[i].name==vertexFromName){
            start=i;
        }
        if(vertexList[i].name==vertexToName){
            end=i;
        }
    }
    
    if(start == -1){
        throw VertexNotFoundException(vertexFromName);
    }
    if(end == -1){
        throw VertexNotFoundException(vertexToName);
    }
    
    bool found=false;
    for(int i=0;i<vertexList[start].edges.size();i++){
        if(vertexList[start].edges[i].name == edgeName){
            found=true;
            vertexList[start].edges.erase(vertexList[start].edges.begin() + i);
            break;
        }
    }
    if(!found){
        throw EdgeNotFoundException(vertexFromName,edgeName);
    }
}

bool MultiGraph::HeuristicShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                       const std::string& vertexNameFrom,
                                       const std::string& vertexNameTo,
                                       float heuristicWeight) const
{
    int INT_MAX = 2147483647;
    std::vector<int> weights(vertexList.size()+1,INT_MAX);
    std::vector<std::vector<int>> nearestPrevious(vertexList.size()+1,std::vector<int>());
    MinPairHeap<float,int> pq;
    
    int start=-1,end=-1;
    for(int i=0;i<vertexList.size();i++){
        if(vertexList[i].name==vertexNameFrom){
            start=i;
        }
        if(vertexList[i].name==vertexNameTo){
            end=i;
        }
    }
    
    if(start == -1){
        throw VertexNotFoundException(vertexNameFrom);
    }
    if(end == -1){
        throw VertexNotFoundException(vertexNameTo);
    }
    
    weights[start]=0.0;
    pq.push({0,start});
    
    while (!pq.empty()){
        int node=pq.top().value;
        float dist=pq.top().key;

        pq.pop();
        for (int i=0;i<vertexList[node].edges.size();i++){
            GraphEdge neighbor = vertexList[node].edges[i]; // neighbor == edge to a neighbor
            int neighborIndex = neighbor.endVertexIndex;
            float cost = Lerp(neighbor.weight[0], neighbor.weight[1], heuristicWeight);
            if (dist+cost<weights[neighborIndex]){
                std::vector<int> temp{node,i};
                nearestPrevious[neighborIndex].assign(temp.begin(), temp.end());
                weights[neighborIndex]=dist+cost;
                Pair<float,int> newPair{dist+cost,neighborIndex};
                pq.push(newPair);
            }
        }
    }
    if (weights[end] == INT_MAX) {
        return false;
    }
    else{
        int current=end;
        orderedVertexEdgeIndexList.insert(orderedVertexEdgeIndexList.begin(),current);
        while(current!=start){
            int previous = nearestPrevious[current][0];
            int usedEdge = nearestPrevious[current][1];
            orderedVertexEdgeIndexList.insert(orderedVertexEdgeIndexList.begin(),usedEdge);
            orderedVertexEdgeIndexList.insert(orderedVertexEdgeIndexList.begin(),previous);
            current=previous;
        }
    }
    return true;
}

bool MultiGraph::FilteredShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                      const std::string& vertexNameFrom,
                                      const std::string& vertexNameTo,
                                      float heuristicWeight,
                                      const std::vector<std::string>& edgeNames) const
{
    int INT_MAX = 2147483647;
    std::vector<int> weights(vertexList.size()+1,INT_MAX);
    std::vector<std::vector<int>> nearestPrevious(vertexList.size()+1,std::vector<int>());
    MinPairHeap<float,int> pq;
    
    int start=-1,end=-1;
    for(int i=0;i<vertexList.size();i++){
        if(vertexList[i].name==vertexNameFrom){
            start=i;
        }
        if(vertexList[i].name==vertexNameTo){
            end=i;
        }
    }
    
    if(start == -1){
        throw VertexNotFoundException(vertexNameFrom);
    }
    if(end == -1){
        throw VertexNotFoundException(vertexNameTo);
    }
    
    weights[start]=0.0;
    pq.push({0,start});
    
    while (!pq.empty()){
        int node=pq.top().value;
        float dist=pq.top().key;

        pq.pop();
        for (int i=0;i<vertexList[node].edges.size();i++){
            GraphEdge neighbor = vertexList[node].edges[i]; // neighbor == edge to a neighbor
            int neighborIndex = neighbor.endVertexIndex;
            bool found=false;
            for(int j=0;j<edgeNames.size();j++){
                if(edgeNames[j] == neighbor.name){
                    found=true;
                }
            }
            if(found){continue;}
            float cost = Lerp(neighbor.weight[0], neighbor.weight[1], heuristicWeight);
            if (dist+cost<weights[neighborIndex]){
                std::vector<int> temp{node,i};
                nearestPrevious[neighborIndex].assign(temp.begin(), temp.end());
                weights[neighborIndex]=dist+cost;
                Pair<float,int> newPair{dist+cost,neighborIndex};
                pq.push(newPair);
            }
        }
    }
    if (weights[end] == INT_MAX) {
        return false;
    }
    else{
        int current=end;
        orderedVertexEdgeIndexList.insert(orderedVertexEdgeIndexList.begin(),current);
        while(current!=start){
            int previous = nearestPrevious[current][0];
            int usedEdge = nearestPrevious[current][1];
            orderedVertexEdgeIndexList.insert(orderedVertexEdgeIndexList.begin(),usedEdge);
            orderedVertexEdgeIndexList.insert(orderedVertexEdgeIndexList.begin(),previous);
            current=previous;
        }
    }
    return true;
}

int MultiGraph::BiDirectionalEdgeCount() const
{
    int total=0;
    for(int i=0;i<vertexList.size();i++){
        for(int j=0;j<vertexList[i].edges.size();j++){
            int v1 = vertexList[i].edges[j].endVertexIndex;
            for(int t=0;t<vertexList[v1].edges.size();t++){
                if((vertexList[v1].edges[t].name == vertexList[i].edges[j].name) && (vertexList[v1].edges[t].endVertexIndex == i)){
                    total++;
                    break;
                }
            }
        }
    }
    return total/2;
}

int MultiGraph::MaxDepthViaEdgeName(const std::string& vertexName,
                                    const std::string& edgeName) const
{
    int count=0;
    int start=-1;
    int INT_MAX = 2147483647;
    std::vector<int> distances(vertexList.size(),INT_MAX);
    std::vector<bool> visited(vertexList.size(),false);
    MinPairHeap<int,int> heap;
    for(int i=0;i<vertexList.size();i++){
        if(vertexName == vertexList[i].name){
            start=i;
        }
    }
    if(start == -1){
        throw VertexNotFoundException(vertexName);
    }
    
    heap.push({0,start});
    while(!heap.empty()){
        int current = heap.top().value;
        int streak = heap.top().key;
        heap.pop();
        visited[current]=true;
        bool finished=true;
        
        for(auto neighbor : vertexList[current].edges){
            if(visited[neighbor.endVertexIndex]==false && neighbor.name == edgeName){
                heap.push({streak+1,neighbor.endVertexIndex});
                
                finished=false;
            }
        }
        if(finished && count<streak){
            count = streak;
        }
    }
    
    return count;
    

}

std::vector<float> MultiGraph::getInformation(const std::string vertexFromName,
                               const std::string vertexToName,
                               const std::string edgeName) 
{
    std::vector<float> information(4,-1);
    for(int i=0;i<vertexList.size();i++){
        if(vertexList[i].name == vertexFromName){
            information[0] = (float) i;
        }
        if(vertexList[i].name == vertexToName){
            information[1] = (float) i;
        }
    }
    
    if(information[0] == -1 || information[1]== -1){
        return information;
    }
    
    for(int i=0;i<vertexList[information[0]].edges.size();i++){
        if(vertexList[information[0]].edges[i].name == edgeName){
            information[2]=vertexList[information[0]].edges[i].weight[0];
            information[3]=vertexList[information[0]].edges[i].weight[1];
        }
    }
}

std::vector<int> MultiGraph::getIndices(const std::string vertexFromName,
                               const std::string vertexToName)
{
    std::vector<int> information(2,-1);
    for(int i=0;i<vertexList.size();i++){
        if(vertexList[i].name == vertexFromName){
            information[0] = i;
        }
        if(vertexList[i].name == vertexToName){
            information[1] = i;
        }
    }    
    return information;
}

int MultiGraph::getIndex(const std::string vertexFromName) const
{
    int index=-1;
    for(int i=0;i<vertexList.size();i++){
        if(vertexList[i].name == vertexFromName){
            index = i;
        }
    }    
    return index;
}

void MultiGraph::findComplement(std::vector<std::string>& edgeNames, const std::string edgeName, const std::string vertexFromName) const{
    std::vector<std::string> sisters;
    sisters.push_back(edgeName);
    std::vector<bool> visitedVertices(vertexList.size(),false);
    std::vector<int> possibleCounts(vertexList.size(),0);
    std::vector<int> myStack;
    
    int startInt = -1;
    for(int i=0;i<vertexList.size();i++){
        if(vertexList[i].name == vertexFromName){
            std::cout<<"BULDUM";
            startInt = i;
        }
    }
    
    myStack.push_back(startInt);
    
    while(!myStack.empty()){
        int current = myStack[myStack.size()-1]; 
        myStack.pop_back();
        visitedVertices[current] = true;
        for(auto neighbor : vertexList[current].edges){
            int neighborIndex = neighbor.endVertexIndex;
            if(neighbor.name == edgeName && visitedVertices[neighborIndex]==false){
                myStack.push_back(neighborIndex);
            }
        }
    }
    
    
    bool changed=false;
    do{
        changed=false;
        for(int i=0;i<vertexList.size();i++){ // counting the maximum non-visited neighbor part
            int count =0;
            
            for(int j=0;j<vertexList[i].edges.size();j++){ // traversing edges of that vertex
                int nei = vertexList[i].edges[j].endVertexIndex;
                if( visitedVertices[nei] == false){ // it is non-visited neighbor :)
                    count++;
                }   
            }
            possibleCounts[i] = count;
        }
        // POSSIBLE COUNTS SETTED, NOW WE WILL FIND THE MAXIMUM OF THESE 
        
        int maxi = 0;
        int maxiIndex=-1;
        for(int i=0;i<vertexList.size();i++){  
            if(possibleCounts[i] > maxi){
                maxi = possibleCounts[i];
                maxiIndex = i;
            }
        }
        // NOW THE STARTING AIRPORT IS i , FIND FIRST NON-UTILIZED EDGENAME FROM THIS VERTEX
        
        int currentVertex = maxiIndex;
        for(int i=0;i<vertexList[currentVertex].edges.size();i++){
            if(visitedVertices[vertexList[currentVertex].edges[i].endVertexIndex] == false){ // ve bunun sisterda olmamasını kontrol etmedim zaten olmayacak mantıken
                sisters.push_back(vertexList[currentVertex].edges[i].name);
            }
        }
        
        // NOW CURRENTEDGENAME VE STARTAIRPORT VAR // TRAVERSE NON-VISITED AIRPORTS
        
        myStack.clear();
        myStack.push_back(maxiIndex);
        
        while(!myStack.empty()){
            int current = myStack[myStack.size()-1]; 
            myStack.erase(myStack.end()-1);
            if(visitedVertices[current] == false){
                visitedVertices[current]=true;
                changed = true;
            }
            for(auto neighbor : vertexList[current].edges){
                int neighborIndex = neighbor.endVertexIndex;
                bool found=false;
                for(int i=0;i<sisters.size();i++){
                    if(neighbor.name == sisters[i]){
                        found=true;
                    }
                }
                if((neighbor.name == edgeName || found)&& visitedVertices[neighborIndex]==false){
                    myStack.push_back(neighborIndex);
                }
            }
        }
        
        /*if(changed){
            sisters.push_back(currentEdgeName);
        }*/
    
        
    }while(changed);
    
    for(int i=0;i<sisters.size();i++){
        if(sisters[i]!=edgeName){
            edgeNames.push_back(sisters[i]);
        }
    }
    
}


#endif // MULTI_GRAPH_H

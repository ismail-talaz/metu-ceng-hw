#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
//=======================//
// Implemented Functions //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::PRIMES[3] = {102523, 100907, 104659};

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintLine(int tableIndex) const
{
    const HashData& data = table[tableIndex];

    // Using printf here it is easier to format
    if(data.sentinel == SENTINEL_MARK)
    {
        printf("[%03d]         : SENTINEL\n", tableIndex);
    }
    else if(data.sentinel == EMPTY_MARK)
    {
        printf("[%03d]         : EMPTY\n", tableIndex);
    }
    else
    {
        printf("[%03d] - [%03d] : ", tableIndex, data.lruCounter);
        printf("(%-5s) ", data.isCostWeighted ? "True" : "False");
        size_t sz = data.intArray.size();
        for(size_t i = 0; i < sz; i++)
        {
            if(i % 2 == 0)
                printf("[%03d]", data.intArray[i]);
            else
                printf("/%03d/", data.intArray[i]);

            if(i != sz - 1)
                printf("-->");
        }
        printf("\n");
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintTable() const
{
    printf("____________________\n");
    printf("Elements %d\n", elementCount);
    printf("[IDX] - [LRU] | DATA\n");
    printf("____________________\n");
    for(int i = 0; i < MAX_SIZE; i++)
    {
        PrintLine(i);
    }
}

//=======================//
//          TODO         //
//=======================//
template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Hash(int startInt, int endInt, bool isCostWeighted)
{
    int result= PRIMES[0]*startInt+PRIMES[1]*endInt+PRIMES[2]*int(isCostWeighted);
    return result%MAX_SIZE;
}

template<int MAX_SIZE>
HashTable<MAX_SIZE>::HashTable()
{
        for(int i=0;i<MAX_SIZE;i++){
        table[i].lruCounter=0;
        table[i].sentinel=EMPTY_MARK;
        
    }
    elementCount=0;
}

template<int MAX_SIZE>
int HashTable<MAX_SIZE>::Insert(const std::vector<int>& intArray, bool isCostWeighted)
{
    if(intArray.size()==0){
        throw InvalidTableArgException();
    }
    
    int hashCode = Hash(intArray[0],intArray[intArray.size()-1],isCostWeighted);
    int factor = 0;
    
    while(true){
        int index = (hashCode+factor*factor)%MAX_SIZE;
        if(table[index].sentinel==EMPTY_MARK || table[index].sentinel==SENTINEL_MARK){
            break;
        }
        else if((table[index].intArray[0] == intArray[0]) && (table[index].intArray[table[index].intArray.size()-1] == intArray[intArray.size()-1])){
            table[index].lruCounter+=1;
            return table[index].lruCounter;
        }
        else{
            factor++;    
        }
        
    }

    if(float(elementCount)/MAX_SIZE >= 0.5){
        throw TableCapFullException(elementCount);
    }
    
    
    factor = 0;
    bool cont=true;
    while(cont){
        int index = (hashCode+factor*factor)%MAX_SIZE;
        if(table[index].sentinel==EMPTY_MARK || table[index].sentinel==SENTINEL_MARK){
            table[index].sentinel=OCCUPIED_MARK;
            table[index].isCostWeighted = isCostWeighted;
            table[index].startInt = intArray[0];
            table[index].endInt = intArray[intArray.size()-1];
            table[index].lruCounter=1;
            elementCount++;
            for(int i=0;i<intArray.size();i++){
                table[index].intArray.push_back(intArray[i]);
            }
            return 0;
        }
        else{
            factor++;    
        }
        
    }
}

template<int MAX_SIZE>
bool HashTable<MAX_SIZE>::Find(std::vector<int>& intArray,
                               int startInt, int endInt, bool isCostWeighted,
                               bool incLRU)
{
    
    int hashCode = Hash(startInt,endInt,isCostWeighted);
    int factor = 0;
    
    while(true){
        int index = (hashCode+factor*factor)%MAX_SIZE;
        if(table[index].sentinel==EMPTY_MARK || table[index].sentinel==SENTINEL_MARK){
            return false;
        }
        else if((table[index].startInt == startInt) && (table[index].endInt == endInt) && (table[index].isCostWeighted==isCostWeighted)){
            if(incLRU){table[index].lruCounter++;}
            for(int i=0;i<table[index].intArray.size();i++){
                intArray.push_back(table[index].intArray[i]);
            }
            return true;
        }
        else{
            factor++;    
        }
        
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::InvalidateTable()
{
    for(int i=0;i<MAX_SIZE;i++){
        table[i].sentinel=EMPTY_MARK;
        table[i].intArray.clear();
    }
    elementCount=0;
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::GetMostInserted(std::vector<int>& intArray) const
{
    intArray.clear();
    int max=0;
    int maxIndex=0;
    for(int i=0;i<MAX_SIZE;i++){
        if(table[i].lruCounter>max){
            max=table[i].lruCounter;
            maxIndex=i;
        }
    }
    for(int i=0;i<table[maxIndex].intArray.size();i++){
        intArray.push_back(table[maxIndex].intArray[i]);
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::Remove(std::vector<int>& intArray,
                                 int startInt, int endInt, bool isCostWeighted)
{
    int hashCode = Hash(startInt,endInt,isCostWeighted);
    int factor = 0;
    
    while(true){
        int index = (hashCode+factor*factor)%MAX_SIZE;
        if(table[index].sentinel==EMPTY_MARK || table[index].sentinel==SENTINEL_MARK){
            return;
        }
        else if((table[index].startInt == startInt) && (table[index].endInt == endInt) && (table[index].isCostWeighted==isCostWeighted)){
            for(int i=0;i<table[index].intArray.size();i++){
                intArray.push_back(table[index].intArray[i]);
            }
            table[index].intArray.clear();
            table[index].sentinel=SENTINEL_MARK;
            elementCount--;
            return;
        }
        else{
            factor++;    
        }
        
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::RemoveLRU(int lruElementCount)
{
    MinPairHeap<int,int> heap;
    
    for(int i=0;i<MAX_SIZE;i++){
        if(table[i].lruCounter!=0){
            heap.push({table[i].lruCounter,i});
        }
    }
    
    while(!heap.empty() && lruElementCount!=0){
        int current = heap.top().value;
        heap.pop();
        lruElementCount--;
        table[current].sentinel=SENTINEL_MARK;
        table[current].intArray.clear();
        elementCount--;
        
    }
}

template<int MAX_SIZE>
void HashTable<MAX_SIZE>::PrintSortedLRUEntries() const
{
    MaxPairHeap<int,int> heap;
    
    for(int i=0;i<MAX_SIZE;i++){
        if(table[i].lruCounter!=0){
            heap.push({table[i].lruCounter,i});
        }
    }
    
    while(!heap.empty()){
        int current = heap.top().value;
        heap.pop();
        PrintLine(current);
    }
}

#endif // HASH_TABLE_HPP
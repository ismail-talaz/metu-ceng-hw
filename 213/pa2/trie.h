#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;


template <class T>
class Trie {    
private:
    // Define TrieNode for the first phase (Trie)
    /* DO NOT CHANGE */
    struct TrieNode {
        static const int ALPHABET_SIZE = 128;
        TrieNode* children[ALPHABET_SIZE];
        char keyChar;
        bool isEndOfKey;
        T* data;

        TrieNode(char c) : keyChar(c), isEndOfKey(false), data(NULL) {

                for (int i = 0; i< ALPHABET_SIZE; i++) {
                children[i] = NULL;
                
            }
            
            }
    };

    TrieNode* root;

public: // Do not change.

    Trie(); 
    ~Trie();
    
    Trie& insert(const string& username); 
    void remove(std::string username);
    T* search(std::string username); 
    void findStartingWith(std::string prefix, std::vector<T*> &results); 
    void wildcardSearch(const std::string &wildcardKey, std::vector<T*> &results); 
    void print(); 

private: // you may add your own utility member functions here.
    void print(const std::string& primaryKey); // Do not change this line.
    void printTrie(TrieNode* node, const std::string& currentKey); // Do not change this line.
    
};

/* DO NOT CHANGE */
template <class T>
Trie<T>::Trie() : root(new TrieNode('\0')) {}

template <class T>
Trie<T>::~Trie() {
    /* IMPLEMENT THIS */
}

template <class T>
Trie<T>& Trie<T>::insert(const string& key) {
    TrieNode* curr=root;
    int length=key.length();
    for(int i=0;i<length;i++){
        char cChar=key[i];
        if ((curr->children[static_cast<int>(cChar)])!=NULL){
            curr=(curr->children[static_cast<int>(cChar)]);
        }
        else{
            TrieNode* newNode= new TrieNode(cChar);
            if (i==length-1){
                newNode->isEndOfKey=true;
                newNode->data=new T();
            }
            (curr->children[static_cast<int>(cChar)])=newNode;
            curr=(curr->children[static_cast<int>(cChar)]);
        }
    }
    return *this;
}

template <class T>
T* Trie<T>::search(std::string username) {
    TrieNode* curr=root;
    for(char charac:username){
        if(curr->children[static_cast<int>(charac)]==NULL){
            return NULL;
        }
        else{
            curr=curr->children[static_cast<int>(charac)];
        }
    }
    if (curr->isEndOfKey){
        return curr->data;
    }
    return NULL;

}

template <class T>
void Trie<T>::remove(std::string username) {
    if(!search(username)){return;}
    TrieNode* curr=root;
    for(char charac:username){
        curr=curr->children[static_cast<int>(charac)];
        }
    }
    curr->isEndOfKey=false;
}

template <class T>
void Trie<T>::findStartingWith(string prefix,vector<T*> &results) {
/* IMPLEMENT THIS */
    
}

template <class T>
void Trie<T>::wildcardSearch(const std::string &wildcardKey, std::vector<T*> &results) {
/* IMPLEMENT THIS */
}

/* DO NOT CHANGE */
template<class T>
void Trie<T>::print() {
    print("");
}
/* DO NOT CHANGE */
template <class T>
void Trie<T>::print(const std::string& primaryKey) {
    if (primaryKey.empty()) {
        // Print the entire tree.
        printTrie(root, "");
    } else {
        // Print specific keys.
        TrieNode* primaryNode = root;
        for (int i = 0; i < primaryKey.length(); i++) {
            
            int index = primaryKey[i];
            if (!primaryNode->children[index]) {
                std::cout << "{}" << std::endl; // Primary key not found, nothing to print.
                return ;
            }
            primaryNode = primaryNode->children[index];
        }

        }
    }
/* DO NOT CHANGE */
template <class T>
void Trie<T>::printTrie(TrieNode* node, const std::string& currentKey) {
    if (!node) {
        return;
    }
    if (node->isEndOfKey) {
        std::cout << currentKey <<std::endl;
        std::cout << "Watched Movies:" << std::endl;
        if(node->data) {
            node->data->printMovies();
        }
    }
    for (int i = 0; i < 128; ++i) {
        if (node->children[i]) {
            printTrie(node->children[i], currentKey + static_cast<char>(i));
        }
    }
}


#endif

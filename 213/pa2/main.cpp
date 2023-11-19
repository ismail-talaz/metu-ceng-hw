#include "bst.h"
#include "movie.h"
#include "trie.h"
#include "user.h"
#include "user_interface.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>


using namespace std;

int main(){


    BST<int> myBST;
    BST<int> meineBST2;

    BST<int> *result;

    myBST.insert("ASD",5);
    myBST.insert("ZAZAZA",25);
    myBST.insert("FAFAFAFA",30);


    meineBST2.insert("FAFAFAFA",25);
    meineBST2.insert("TTTTT",5);
    meineBST2.insert("ZAZAZA",22);
    meineBST2.insert("AS",5);



    result=myBST.intersection(&meineBST2);
    cout<<"Result:"<<endl;
    result->tree2vector(result->getRoot());
    



    return 0;
}

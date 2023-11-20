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


    /*BST<int> myBST;
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
    result->tree2vector(result->getRoot());*/

    /*string key1="eray";
    string key2="araasy";
    string key3="beray";
    string key4="yusuf";
    string key5="zuyokamk";
    string key6="sadadsa";
    string mov1="asdsaeray";
    string mov2="edsadasray";
    string mov3="vvveray";
    string mov4="cghb";
    string mov5="sadsada";
    string mov6="asdjasdkasdas";
    Movie movie1(mov1,3432,331);
    Movie movie2(mov2,342,31231231);
    Movie movie3(mov3,32,3131);
    Movie movie4(mov4,11,3232);
    Movie movie5(mov5,11,3232);
    Movie movie6(mov5,21312,211321);
    BST<Movie> a ,b;
    BST<Movie> *result;
    b.insert(key6,movie6);
    a.insert(key1,movie1);
    a.insert(key2,movie2);
    a.insert(key3,movie3);
    a.insert(key4,movie4);
    a.insert(key5,movie5);
    result=a.merge(&b);

    cout<<"era21321y";*/
    Movie movie1("Interstellar",1990,9.5);
    Trie<User> c;
    c.insert("eray");
    c.insert("berat");
    User* user2=c.search("eray");
    user2->addMovie("Interstellar",movie1);
    c.search("eray");
    c.remove("eray");
    c.print();
    
    return 0;
}

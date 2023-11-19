#ifndef BST_H
#define BST_H

#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
using namespace std;

// do not add any other library.
// modify parts as directed by assignment text and comments here.

template <class T>
class BST {
private:
    // Define TreeNode for the second phase (Binary Search Tree)
    /* DO NOT CHANGE */
    struct TreeNode {
        std::string key;
        T data;
        TreeNode* left;
        TreeNode* right;
        TreeNode(const std::string &k, const T& value) : key(k), data(value), left(NULL), right(NULL) {}
        
    };

    TreeNode* root;


public: // Do not change.

    BST();
    ~BST();
    TreeNode* getRoot() { return root; }
    bool isEmpty() { return root == NULL; }

    BST& insert(const std::string key, const T& value);
    bool search(std::string value) const;
    void remove(std::string value);
    BST<T>* merge(BST<T>* bst);
    BST<T>* intersection(BST<T>* bst);
    std::vector<TreeNode> tree2vector(TreeNode* root);
    void print();
    
private:// you may add your own utility member functions here.

    void print(TreeNode* node, std::string indent, bool last, bool isLeftChild); // Do not change.
    void tree2vector(TreeNode* node, vector<TreeNode>& result);
    
};

    // Constructor
    template <class T>
    BST<T>::BST() : root(NULL) {}

    // Destructor
    template <class T>
    BST<T>::~BST() {
    /* IMPLEMENT THIS */

    }

    // Insert function for BST.    
    template <class T>
    BST<T>& BST<T>::insert(const string key, const T& value) {
        TreeNode* curr=root;
        bool inserted=false;
        TreeNode* newNode=new TreeNode(key,value);
        while(!inserted){
            if (curr==NULL){
                root=newNode;
                inserted=true;
            }
            else if (key<curr->key){
                if(curr->left){
                    curr=curr->left;
                }
                else{
                    curr->left=newNode;
                    inserted=true;
                }

            }
            else{  /*key > curr->key*/
                if(curr->right){
                    curr=curr->right;
                }
                else{
                    curr->right=newNode;
                    inserted=true;
                }

            }
        }
        return *this;

    }
    
    // Search function for BST.
    template <class T>
    bool BST<T>::search(std::string value) const {
        TreeNode* curr=root;
        while(curr){
            if(value>curr->key){
                curr=curr->right;
            }
            else if (value<curr->key){
                curr=curr->left;
            }
            else{
                return true;
            }
        }
        return false;
    }


    
    // Remove a node from BST for given key. If key not found, do not change anything.
    template <class T>
    void BST<T>::remove(std::string key) {
        TreeNode* found=NULL;
        TreeNode* curr=root,*prev=NULL;
        pair<TreeNode*,string> ancPath;
        string last;
        while(curr){
            if(key>curr->key){
                prev=curr;
                curr=curr->right;
                last="RIGHT";
            }
            else if (key<curr->key){
                prev=curr;
                curr=curr->left;
                last="LEFT";
            }
            else{
                ancPath={prev,last};
                found=curr;
                break;
            }
        }

        if(!found){return;}

        if(!found->left && !found->right){ /*It's a leaf node.*/
            if (ancPath.second=="LEFT"){
                ancPath.first->left=NULL;
                delete found;
            }
            else{
                ancPath.first->right=NULL;
                delete found;
            }
        }
        else if(found->left && !found->right){
            if(ancPath.second=="LEFT"){
                ancPath.first->left=found->left;
                delete found;
            }
            else{
                ancPath.first->right=found->left;
                delete found;
            }
        }
        else if(!found->left && found->right){
            if(ancPath.second=="LEFT"){
                ancPath.first->left=found->right;
                delete found;
            }
            else{
                ancPath.first->right=found->right;
                delete found;
            }
        }
        else{
            TreeNode* minNode=found->right;
            while(minNode->left){
                minNode=minNode->left;
            }
            string tempKey=minNode->key;
            T tempValue=minNode->data;
            remove(tempKey);
            found->key=tempKey;
            found->data=tempValue;
        }

    }
    
    // A helper function for converting a BST into vector.
    template <class T>
    vector<typename BST<T>::TreeNode> BST<T>::tree2vector(TreeNode* root) {
        vector<TreeNode> result;
        tree2vector(root,result);

        for(auto i:result){
            std::cout<<i.key<<" ";
        }

        return result;
    }

    template <class T>
    void BST<T>::tree2vector(TreeNode* node, vector<TreeNode>& result){
        if (!node){return;}
        if(node->left){tree2vector(node->left,result);}
        TreeNode* newNode= new TreeNode(node->key,node->data);
        newNode->right=node->right;
        newNode->left=node->left;
        result.push_back(*newNode);
        if(node->right){tree2vector(node->right,result);}
    }
    
    // Merge two BST's and return merged BST.
    template <class T>
    BST<T>* BST<T>::merge(BST<T>* bst) {
        BST<T>* newBST = new BST<T>();
        vector<TreeNode> first=tree2vector(this->root);
        vector<TreeNode> second=tree2vector(bst->root);
        int size1=first.size();
        int size2=second.size();
        int i=0,j=0;
        while(true){
            if(i<size1 && j<size2 && first[i].data==second[j].data){
                newBST->insert(first[i].key,first[i].data);
                i++;
                j++;
            }
            else{
                if(i<size1){
                    newBST->insert(first[i].key,first[i].data);
                    i++;
                }
                if (j<size2){
                    newBST->insert(second[j].key,second[j].data);
                    j++;
                }
            }
            if(i>=size1 && j>=size2){
                break;
            }
        }
        return newBST;
    }
        
    // Intersect two BST's and return new BST.
    template <class T>
    BST<T>* BST<T>::intersection(BST<T>* bst) {
        BST<T>* newBST= new BST<T>();
        vector<TreeNode> first=this->tree2vector(this->getRoot());
        vector<TreeNode> second=bst->tree2vector(bst->getRoot());

        for (int i=0;i<first.size();i++){
            if(bst->search(first[i].key)){
                newBST->insert(first[i].key,first[i].data);
            }
        }
        return newBST;
    }
    
    /* DO NOT CHANGE */
    template <class T>
    void BST<T>::print() {
        print(root, "", true, false);
    
    }
    
    /* DO NOT CHANGE */
    template <class T>
    void BST<T>::print(TreeNode* node, string indent, bool last, bool isLeftChild) {
        if (node != NULL) {
            cout << node->key << endl;
            print(node->left, indent, false, true);
            print(node->right, indent, true, false);
        }
    
    }


#endif

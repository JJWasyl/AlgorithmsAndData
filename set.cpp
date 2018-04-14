#include <iostream>
#include <vector>

#include "AVL.hpp"

/**
 * Implementation of set using AVL Tree
 */

using namespace std;


template <class T> class Set;

template <class T>
class Set{

    avlTree<T> tree;
    size_t size = 0;
    
    public:
        
        class Iterator;
        friend class Iterator;
        T* insert(const T &key);
        bool erase(const T &key);
        void erase_all();
        const T* find(const T &key) const;
        T* find(const T &key);
        
        //typedef typename Node_Iterator<T> Iterator;
        
        //Constructors
        Set(){
            tree.root = NULL;
        }

        
        Set(const Set &source){
            tree.root = NULL;
            for(Iterator it = source.begin(); it != source.end(); it++){
                insert(*it);
            }
        }
        
        Set& operator =(const Set &source){
            if(&source == this) return *this;
            erase_all();
            for(Iterator it = source.begin(); it != source.end(); it++){
                insert(*it);
            }
            
            return *this;
        }
        
        Set(Set &&source){
            
        }
         
        Set& operator=(Set &&source);
        
        ~Set(){
            erase_all();
        }
        
        
    public:
        class Iterator{
            
            public:
                Iterator(avl_node<T> *anchor){
                    this->current = anchor;
                }
            
                bool operator==(const Iterator &it);
                bool operator!=(const Iterator &it){ return !(*this == it);}
                Iterator& operator++(){
                    if(current->left != NULL){
                        current = current->left;
                    } else if(current->right != NULL){
                        current = current->right;
                    }
                    return *this;
                }
                Iterator operator++(int){
                    Iterator temp (*this);
                    ++(*this);
                    return temp;
                }
                
                T& operator *();
                T* operator->();
                
            private:
                avl_node<T>* current;
        };
        
        Iterator begin() const { return Iterator(tree.root);}
        Iterator end(){ return Iterator(tree.findMin(tree.root));}
       
};

template <typename T>
T& Set<T>::Iterator::operator *(){ return (current->data);}

template <typename T>
T* Set<T>::Iterator::operator->(){ return &(current->data);}

template <typename T>
bool Set<T>::Iterator::operator==(const Set<T>::Iterator &it){
    return (this->current == it.current);
}


template <typename T>
T* Set<T>::insert(const T &key){
    if((tree.search(tree.root, key)) == NULL){
        avl_node<T>* val = tree.insert(tree.root, key);
        size++;
        return &(val->data);
    } else {
        cout << "Element already exists" << endl;
        return nullptr;
    }
}

template <typename T>
bool Set<T>::erase(const T &key){
    if((tree.search(tree.root, key)) != NULL){
        tree.remove(tree.root, key);
        return true;
    } else {
        return false;
    }
}

template <typename T>
void Set<T>::erase_all(){
    tree.makeEmpty(tree.root);
}

template <typename T>
T* Set<T>::find(const T& key){
    avl_node<T>* val = tree.search(tree.root, key);
    if(val != NULL){
        return &(val->data);
    } else {
        return nullptr;
    }
}

template <typename T>
const T* Set<T>::find(const T& key) const{
    avl_node<T>* val = tree.search(tree.root, key);
    if(val != NULL){
        return &(val->data);
    } else {
        return nullptr;
    }
}


int main(int argc, char * argv[]){

    Set<int> set;
    cout << "Construction complete" << endl;
    
    set.insert(4); set.insert(0);
    set.insert(2); set.insert(7);
    set.insert(0); set.insert(2);
    set.insert(6); set.insert(4);
    set.insert(12); set.insert(11);
    set.insert(8); set.insert(1); 
    set.insert(5); set.insert(0);
    set.insert(3); set.insert(2);
    set.insert(7); set.insert(1);
    set.insert(1); set.insert(1);
    set.insert(9); set.insert(7);
    set.insert(11); set.insert(1);
    cout << "Insertions complete" << endl;
    
    Set<int> set2 = set;
    cout << "Assignment complete" << endl;
    
    for(Set<int>::Iterator it = set.begin(); it != set.end(); it++){
        cout << *it << ' ';
    }
    cout << "Print complete" << endl;
    
return 0;
}


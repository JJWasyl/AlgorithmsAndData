#include <iostream>
#include <algorithm>

using namespace std;

//Node Class
template <typename T>
class AVLnode{
    public:
        T key;
        int balance;
        AVLnode *left, *right, *parent;
        AVLnode(T k, AVLnode *p){
            this->key = k;
            this->balance = 0;
            this->parent = p;
            this->left = NULL;
            this->right = NULL;
        }
        
        ~AVLnode(){
            delete left;
            delete right;
        }
};

//Tree class
template <typename T>
class AVLtree{
    public:        
        AVLtree(const AVLtree& source);
        //AVLtree& operator=(const AVLtree& source);
        AVLnode<T>* insert(T key);
        AVLnode<T>* search(AVLnode<T>* current, const T &arg) const;
        void deleteKey(const T key);
        void printBalance();
        void makeEmpty(AVLnode<T>* &start);
        void inorder(AVLnode<T>* root);
        void rebalance(AVLnode<T>*);
        int height(AVLnode<T>*);
        void setBalance(AVLnode<T>*);
        void printBalance(AVLnode<T>*);
        void clearNode(AVLnode<T>*);
        
        AVLnode<T> *root;
        
    private:        
        AVLnode<T>* rotateLeft(AVLnode<T>*);
        AVLnode<T>* rotateRight(AVLnode<T>*);
        AVLnode<T>* rotateLeftRight(AVLnode<T>*);
        AVLnode<T>* rotateRightLeft(AVLnode<T>*);
    
    public:    
    
        AVLtree(){
            this->root = NULL;
        }
        
        AVLtree& operator=(const AVLtree& source){
            if(&source == this) return *this;
            this.makeEmpty(this->root);
            Iterator source_itr;
            source_itr = source.begin();
            for(source_itr; source_itr != source.end(); source_itr++){
                insert(*source_itr);
            }
        }
        
        ~AVLtree(){
            delete root;
        }
        
        class Iterator{
            private:
                AVLnode<T>* current = nullptr;
                    
            public:
                Iterator(){};
                Iterator(AVLnode<T>* const begin){
                    current = begin;
                }               
                
                //Assignment constructor
                Iterator operator=(const Iterator& source){
                    if(this == &source)
                        return *this;
                        
                    this->current = source.current;
                    return *this; 
                }
                    
                bool operator==(const Iterator& i){
                    return current == i.current;
                }
                    
                bool operator!=(const Iterator& i){
                    return current != i.current;
                }
                    
                Iterator& operator++(){
                    if(current->right){
                        current = current->right;
                        while(current->left){
                            current = current->left;
                        }
                    } else {
                        while(current->parent && current == current->parent->right){
                            current = current->parent;
                        }
                        current = current->parent;
                    }
                }
                    
                Iterator operator++(int){
                    Iterator temp (*this);
                    ++(*this);
                    return temp;
                }
                    
                T& operator *();
                T* operator->();
        };
            
        const Iterator begin() const;
        const Iterator end() const;
};

/**
 * AVL class functions
 */
 
//AVL copy constructor
template <typename T>
AVLtree<T>::AVLtree(const AVLtree<T>& source){
    root = nullptr;
    Iterator source_itr, wall;
    source_itr = source.begin();
    for(source_itr; source_itr != source.end(); source_itr++){
        insert(*source_itr);
    }
}

template <typename T>
void AVLtree<T>::inorder(AVLnode<T>* root){
    if (root == nullptr)
        return;
    inorder (root->left);
    cout<<root->key<<" ";
    inorder (root->right);
}
 
//rebalance
template <typename T>
void AVLtree<T>::rebalance(AVLnode<T> *arg){
    setBalance(arg);
    
    if(arg->balance == -2){
        if(height(arg->left->left) >= height(arg->left->right))
            arg = rotateRight(arg);
        else
            arg = rotateLeftRight(arg);
    } else if(arg->balance == 2){
        if(height(arg->right->right) >= height(arg->right->left))
            arg = rotateLeft(arg);
        else
            arg = rotateRightLeft(arg);
    }
    
    if(arg->parent != NULL){
        rebalance(arg->parent);
    } else {
        this->root = arg;
    }
}

template <typename T>
void AVLtree<T>::makeEmpty(AVLnode<T>* &start){
    if(start != nullptr){
        makeEmpty(start->left);
        makeEmpty(start->right);
        free(start);
    }
    start == nullptr;
}

//left rotate
template <typename T>
AVLnode<T>* AVLtree<T>::rotateLeft(AVLnode<T> *arg){
    AVLnode<T> *temp = arg->right;
    temp->parent = arg->parent;
    arg->right = temp->left;
    
    if(arg->right != NULL)
        arg->right->parent = arg;
    
    temp->left = arg;
    arg->parent = temp;
    
    if(temp->parent != NULL){
        if(temp->parent->right == arg){
            temp->parent->right = temp;
        } else {
            temp->parent->left = temp;
        }
    }
    
    setBalance(arg);
    setBalance(temp);
    return temp;
}

//right rotate
template <typename T>
AVLnode<T>* AVLtree<T>::rotateRight(AVLnode<T> *arg){
    AVLnode<T> *temp = arg->left;
    temp->parent = arg->parent;
    arg->left = temp->right;
    
    if(arg->left != NULL)
        arg->left->parent = arg;
    
    temp->right = arg;
    arg->parent = temp;
    
    if(temp->parent != NULL){
        if(temp->parent->right == arg){
            temp->parent->right = temp;
        } else {
            temp->parent->left = temp;
        }
    }
    
    setBalance(arg);
    setBalance(temp);
    return temp;
}

//left and right rotate
template <typename T>
AVLnode<T>* AVLtree<T>::rotateLeftRight(AVLnode<T> *arg){
    arg->left = rotateLeft(arg->left);
    return rotateRight(arg);
}

//right and left rotate
template <typename T>
AVLnode<T>* AVLtree<T>::rotateRightLeft(AVLnode<T> *arg){
    arg->left = rotateRight(arg->left);
    return rotateLeft(arg);
}

//height
template <typename T>
int AVLtree<T>::height(AVLnode<T> *arg){
    if(arg == NULL)
        return -1;
    return 1 + std::max(height(arg->left), height(arg->right));
}

//setBalance
template <typename T>
void AVLtree<T>::setBalance(AVLnode<T> *arg){
    arg->balance = height(arg->right) - height(arg->left);
}

//Print balance factor
template <typename T>
void AVLtree<T>::printBalance(AVLnode<T> *arg){
    if(arg != NULL){
        printBalance(arg->left);
        cout << arg->balance << " ";
        printBalance(arg->right);
    }
}

//insert element
template <typename T>
AVLnode<T>* AVLtree<T>::insert(T key){
    if(root == NULL){
        root = new AVLnode<T>(key, NULL);
    } else {
        AVLnode<T>
            *temp = root,
            *parent;
        
        while(true){
            if(temp->key == key)
                return nullptr;
            
            parent = temp;
            
            bool goLeft = temp->key > key;
            temp = goLeft ? temp->left : temp->right;
            
            if(temp == NULL){
                if(goLeft){
                    parent->left = new AVLnode<T>(key, parent);
                } else {
                    parent->right = new AVLnode<T>(key, parent);
                }
                
                rebalance(parent);
                break;
            }
        }
    }
    
    return root;
}

//delete
template <typename T>
void AVLtree<T>::deleteKey(const T delKey){
    if(root == NULL)
        return;
    
    AVLnode<T>
        *temp = root,
        *parent = root,
        *delNode = NULL,
        *child = root;
        
    while(child != NULL){
        parent = temp;
        temp = child;
        child = delKey >= temp->key ? temp->right : temp->left;
        if(delKey == temp->key)
            delNode = temp;
    }
    
    if(delNode != NULL){
        delNode->key = temp->key;
        child = temp->left != NULL ? temp->left : temp->right;
        
        if(root->key == delKey){
            root = child;
        } else {
            if(parent->left == temp){
                parent->left = child;
            } else {
                parent->right = child;
            }
            
            rebalance(parent);
        }
    }
}

//search function
template <typename T>
AVLnode<T>* AVLtree<T>::search(AVLnode<T>* current, const T &arg) const{
    if(current == nullptr)
        return current;
    
    if(arg == current->key)
        return current;
    else if(arg < current->key)
        return search(current->left, arg);
    else
        return search(current->right, arg);
}

//print the balance
template <typename T>
void AVLtree<T>::printBalance(){
    printBalance(root);
    cout << endl;
}

//Begin and end for the iterator
template <typename T>
const typename AVLtree<T>::Iterator AVLtree<T>::begin() const{
    AVLnode<T> *temp = root;
    while(temp->left != nullptr){
        temp = temp->left;
    }
    return Iterator(temp);
}

template <typename T>
const typename AVLtree<T>::Iterator AVLtree<T>::end() const{
    return Iterator(nullptr);
}

//additional operators for the iterator
template <typename T>
T& AVLtree<T>::Iterator::operator *(){return current->key;}

template <typename T>
T* AVLtree<T>::Iterator::operator->(){return (&current->key);}
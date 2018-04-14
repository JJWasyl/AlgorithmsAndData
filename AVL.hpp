/**
 * Implementacja drzewa AVL
 */
#include<iostream>
#include<cstdio>
#include<sstream>
#include<algorithm>

#define pow2(n) (1 << (n))

using namespace std;

template <class T> class avl_node;
template <class T> class avlTree;

template <class T>
class avl_node{
    public:
        T data;
        class avl_node *left;
        class avl_node *right;
        
    ~avl_node(){
        delete left;
        delete right;
    }

};


template <class T>
class avlTree{

    public:
        avl_node<T> *root;
        int height(avl_node<T> *);
        int diff(avl_node<T> *);
        avl_node<T> *rr_rotation(avl_node<T> *);
        avl_node<T> *ll_rotation(avl_node<T> *);
        avl_node<T> *lr_rotation(avl_node<T> *);
        avl_node<T> *rl_rotation(avl_node<T> *);
        avl_node<T> *balance(avl_node<T> *);
        avl_node<T> *insert(avl_node<T> *, T);
        avl_node<T> *copyHelper(const avl_node<T> *);
        avl_node<T> *search(avl_node<T> *, T);
        T deleteMin(avl_node<T> *);
        avl_node<T> *findMin(avl_node<T> *);
        void makeEmpty(avl_node<T> *);
        void remove(avl_node<T> *, const T);
        void display(avl_node<T> *, int);
        void inorder(avl_node<T> *);
        void preorder(avl_node<T> *);
        void postorder(avl_node<T> *);
        void endTree(avl_node<T> *);
        
        avlTree(){
            root = NULL;
        }
        
        avlTree(const avlTree &toCopy){
            root = copyHelper(toCopy.root);
        }
        
        avlTree& operator=(const avlTree &toAssign){
            if(&toAssign == this){ return *this; }
            delete root;
            this->root = 0;
            if(toAssign.root){
                root = copyHelper(toAssign.root);
            }
            return *this;
        }
        
        //konstruktor przenoszacy dodac
        
        ~avlTree(){
            endTree(root);
        }

};


//Rekurencyjne kopiowanie wezlow do drzewa AVL
template <typename T>
avl_node<T> *copyHelper(const avl_node<T> *toCopy){
    if (toCopy == NULL)
	return NULL;
    avl_node<T> *copyNode = new avl_node<T>;
    copyNode->data = toCopy->data;
    copyNode->left = copyHelper(toCopy->left);
    copyNode->right = copyHelper(toCopy->right);
    return copyNode;
}


template <typename T>
void avlTree<T>::endTree(avl_node<T> *node){
    if(node){
        endTree(node->left);
        endTree(node->right);
        delete node;
    }
}

template <typename T>
int avlTree<T>::height(avl_node<T> *temp){
    int h = 0;
    if (temp != NULL){
        int l_height = height (temp->left);
        int r_height = height (temp->right);
        int max_height = max (l_height, r_height);
        h = max_height + 1;
    }
    
    return h;
}

template <typename T>
int avlTree<T>::diff(avl_node<T> *temp){
    int l_height = height (temp->left);
    int r_height = height (temp->right);
    int b_factor= l_height - r_height;
    return b_factor;
}


template <typename T>
avl_node<T> *avlTree<T>::rr_rotation(avl_node<T> *parent){
    avl_node<T> *temp;
    temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    return temp;
}

template <typename T>
avl_node<T> *avlTree<T>::ll_rotation(avl_node<T> *parent){
    avl_node<T> *temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    return temp;
}

template <typename T>
avl_node<T> *avlTree<T>::lr_rotation(avl_node<T> *parent){
    avl_node<T> *temp;
    temp = parent->left;
    parent->left = rr_rotation (temp);
    return ll_rotation (parent);
}

template <typename T>
avl_node<T> *avlTree<T>::rl_rotation(avl_node<T> *parent){
    avl_node<T> *temp;
    temp = parent->right;
    parent->right = ll_rotation (temp);
    return rr_rotation (parent);
}

template <typename T>
avl_node<T> *avlTree<T>::balance(avl_node<T> *temp){
    int bal_factor = diff(temp);
    if (bal_factor > 1){
        if (diff (temp->left) > 0){
            temp = ll_rotation (temp);
        }else{
            temp = lr_rotation (temp);
        }
    }else if(bal_factor < -1){
        if (diff (temp->right) > 0){
            temp = rl_rotation (temp);
        }else{
            temp = rr_rotation (temp);
        }
    }
    return temp;
}

template <typename T>
avl_node<T> *avlTree<T>::insert(avl_node<T> *root, T value){
    if (root == NULL){
        root = new avl_node<T>;
        root->data = value;
        root->left = NULL;
        root->right = NULL;
        return root;
    }else if (value < root->data){
        root->left = insert(root->left, value);
        root = balance (root);
    }else if (value >= root->data){
        root->right = insert(root->right, value);
        root = balance (root);
    }
    return root;
}

template <typename T>
void avlTree<T>::display(avl_node<T> *ptr, int level){
    int i;
    if (ptr!=NULL){
        display(ptr->right, level + 1);
        printf("\n");
        if (ptr == root)
        cout<<"Root -> ";
        for (i = 0; i < level && ptr != root; i++){
            cout<<"        ";
        }
        cout<<ptr->data;
        display(ptr->left, level + 1);
    }
}

template <typename T>
void avlTree<T>::inorder(avl_node<T> *tree){
    if (tree == NULL)
        return;
    inorder (tree->left);
    cout<<tree->data<<"  ";
    inorder (tree->right);
}

template <typename T>
void avlTree<T>::preorder(avl_node<T> *tree){
    if (tree == NULL)
        return;
    
    cout << tree->data << "  ";
    preorder (tree->left);
    preorder (tree->right);
}

template <typename T>
void avlTree<T>::postorder(avl_node<T> *tree){
    if (tree == NULL)
        return;
    postorder ( tree ->left );
    postorder ( tree ->right );
    cout << tree->data << "  ";
}

template <typename T>
avl_node<T>* avlTree<T>::search(avl_node<T> *tree, T key){
    if(tree == NULL){
        return NULL;
    } else {
        if(key < tree->data){
            search(tree->left, key);
        } else if(key > tree->data){
            search(tree->right, key);
        } else if(key == tree->data){
            return tree;
        }
        return NULL;
    }
}

template <typename T>
void avlTree<T>::makeEmpty(avl_node<T> *tree){
    avl_node<T> *d;
    if(tree != NULL){
        makeEmpty(tree->left);
        makeEmpty(tree->right);
        d = tree;
        free(d);
        tree = NULL;
    }
}

template <typename T>
void avlTree<T>::remove(avl_node<T> *tree, T key){
    avl_node<T> *d;
    if(tree == NULL){
        cout << "Element not found in AVL Tree" << endl;
    } else if(key < tree->data){
        remove(tree->left, key);
    } else if(key > tree->data){
        remove(tree->right, key);
    } else if((tree->left == NULL) && (tree->right == NULL)){
        d = tree;
        free(d);
        tree = NULL;
    } else if(tree->left = NULL){
        d = tree;
        tree = tree->left;
        free(d);
    } else {
        tree->data = deleteMin(tree->right);
    }
    
}

template <typename T>
T avlTree<T>::deleteMin(avl_node<T> *tree){
    T temp;
    if(tree->left == NULL){
        temp = tree->data;
        tree = tree->right;
        return temp;
    } else {
        temp = deleteMin(tree->left);
        return temp;
    }
}

template <typename T>
avl_node<T> *avlTree<T>::findMin(avl_node<T> *tree){
    if(tree->left != NULL){
        findMin(tree->left);
    } else {
        return tree;
    }
    return NULL;
}
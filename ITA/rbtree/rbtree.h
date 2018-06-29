#include <iostream>
#include <vector>
template<typename T>
struct Node_t{
    Node_t<T>* left;
    Node_t<T>* right;
    Node_t<T>* parent;
    T value;
    bool is_black;
    Node_t(T v = 0):left(NULL), right(NULL), parent(NULL), is_black(false), value(v){}
};
template<typename T>
class rbtree{
public:
    void insert(T v);//FIXME argument should be pointer to node
    void remove(Node_t<T>* node);
    void traverse_leaves(std::vector<T>& leaf_values, std::vector<int>& heights);
    void traverse(std::vector<T>& values, std::vector<int>& heights, std::vector<bool>& colors);
    Node_t<T>* lookup(T v, int* black_height);
    rbtree();
    ~rbtree();
private:
    void insert_fixup(Node_t<T>* node);
    void remove_fixup(Node_t<T>* node);
    void left_rotate(Node_t<T>* node);
    void right_rotate(Node_t<T>* node);
    void transplant(Node_t<T>* origNode, Node_t<T>* newNode);
    void delete_sub_tree(Node_t<T>* node);
    void inner_traverse_leaves(Node_t<T>* node, std::vector<T>& leaf_values,\
        std::vector<int>& heights, int counter);
    void inner_traverse(Node_t<T>* root, std::vector<T>& values, std::vector<int>& heights, std::vector<bool>& colors, int height);

    Node_t<T>* root;
    Node_t<T>* nil;
    
};

template<typename T>
void rbtree<T>::traverse_leaves(std::vector<T>& leaf_values, std::vector<int>& heights){
    inner_traverse_leaves(root, leaf_values, heights, 0);
}

template<typename T>
void rbtree<T>::inner_traverse_leaves(Node_t<T>* node, std::vector<T>& leaf_values, std::vector<int>& heights, int counter){
    if(node == nil){
        return;
    }
    if(node->left != nil){
        if(node->is_black)
            inner_traverse_leaves(node->left, leaf_values, heights, counter+1);
        else
            inner_traverse_leaves(node->left, leaf_values, heights, counter);
    }
    if(node->left == nil && node->right == nil){
        int height = node->is_black ? counter+1 : counter;
        leaf_values.push_back(node->value);
        heights.push_back(height);
    }
    if(node->right != nil){
        if(node->is_black)
            inner_traverse_leaves(node->right, leaf_values, heights, counter+1);
        else
            inner_traverse_leaves(node->right, leaf_values, heights, counter);
    }
}

template<typename T>
rbtree<T>::rbtree()
{
    nil = new Node_t<T>();
    nil->is_black = true;
    nil->left = nil;
    nil->right = nil;
    nil->parent = nil;

    root = nil;
}

template<typename T>
void rbtree<T>::delete_sub_tree(Node_t<T>* node){
    if(node == nil)
        return;
    if(node->left != nil){
        delete_sub_tree(node->left);
    }
    if(node->right != nil){
        delete_sub_tree(node->right);
    }
    delete node;
}

template<typename T>
rbtree<T>::~rbtree()
{
    //delete the tree
    delete_sub_tree(root);
    delete nil;
}

template<typename T>
void rbtree<T>::traverse(std::vector<T>& values, std::vector<int>& heights, 
    std::vector<bool>& colors)
{
    inner_traverse(root, values, heights, colors, 0);
}

template<typename T>
void rbtree<T>::inner_traverse(Node_t<T>* root, std::vector<T>& values, std::vector<int>& heights,
    std::vector<bool>& colors, int height)
{
    if(root == nil){
        return;
    }
    if(root->is_black)
        inner_traverse(root->left, values, heights, colors, height+1);
    else
        inner_traverse(root->left, values, heights, colors, height+1);

    values.push_back(root->value);
    heights.push_back(height);
    colors.push_back(root->is_black);

    if(root->is_black)
        inner_traverse(root->right, values, heights, colors, height+1);
    else
        inner_traverse(root->right, values, heights, colors, height+1);
}

template<typename T>
Node_t<T>* rbtree<T>::lookup(T v, int* black_height){
    auto node = root;
    *black_height = 0;
    while(node != nil && node->value != v){
        if(node->is_black) ++ *black_height;
        if(v < node->value){
            node = node->left;
        }else{
            node = node->right;
        }
    }
    if(node == nil){
        *black_height = 0;
        return NULL;
    }else{
        *black_height = node->is_black ? (1+*black_height) : *black_height;
        return node;
    }
}

template<typename T>
void rbtree<T>::left_rotate(Node_t<T>* node)
{
    auto orig_right = node->right;
    node->right = orig_right->left;
    if(node->right != nil){
        node->right->parent = node;
    }
    orig_right->parent = node->parent;
    if(node->parent == nil){
        root = orig_right;
    }else if(node == node->parent->left){
        node->parent->left = orig_right;
    }else{
        node->parent->right = orig_right;
    }
    orig_right->left = node;
    node->parent = orig_right;
}

template<typename T>
void rbtree<T>::right_rotate(Node_t<T>* node)
{
    auto orig_left = node->left;
    node->left = orig_left->right;
    if(node->left != nil){
        node->left->parent = node;
    }
    orig_left->parent = node->parent;
    if(node->parent == nil){
        root = orig_left;
    }else if(node == node->parent->left){
        node->parent->left = orig_left;
    }else{
        node->parent->right = orig_left;
    }
    orig_left->right = node;
    node->parent = orig_left;
}

template<typename T>
void rbtree<T>::insert(T v){
    Node_t<T>* new_node = new Node_t<T>(v);
    new_node->is_black = false;
    new_node->left = nil;
    new_node->right = nil;
    new_node->parent = nil;

    Node_t<T>* parent = nil, *child = root;
    while(child != nil){
        parent = child;
        if(v < child->value){
            child = child->left;
        }else{
            child = child->right;
        }
    }
    new_node->parent = parent;
    if(parent != nil){
        if(v < parent->value){
            parent->left = new_node;
        }else{
            parent->right = new_node;
        }
    }else{
        root = new_node;
    }
    //fix_up
    insert_fixup(new_node);
}

template<typename T>
static Node_t<T>* get_uncle(Node_t<T>* node){
    return node->parent->parent->left == node->parent ? \
        node->parent->parent->right : node->parent->parent->left;
}


template<typename T>
static bool is_left(Node_t<T>* node){
    return node->parent->left == node;
}

template<typename T>
void rbtree<T>::insert_fixup(Node_t<T>* node)
{
    while(node->parent->is_black == false){
        if(get_uncle(node)->is_black == false){
        //case 1
            std::cout<<"case 1:"<<"node value:"<<node->value<<std::endl<<std::flush;
            node->parent->is_black = true;
            get_uncle(node)->is_black = true;
            node = node->parent->parent;
            node->is_black = false;
            continue;
        }
        if(!is_left(node) && is_left(node->parent)){
        //case 2
            std::cout<<"case 2:"<<"node value:"<<node->value<<std::endl<<std::flush;
            left_rotate(node->parent);
            node = node->left;
        }else if(is_left(node) && !is_left(node->parent)){
            std::cout<<"case 2 mirror:"<<"node value:"<<node->value<<std::endl<<std::flush;
        //mirror case 2
            right_rotate(node->parent);
            node = node->right;
        }else if(is_left(node) && is_left(node->parent)){
        //case 3
            std::cout<<"case 3:"<<"node value:"<<node->value<<std::endl<<std::flush;
            right_rotate(node->parent->parent);
            node->parent->is_black = true;
            node->parent->right->is_black = false;
        }else{
        //mirror case 3
            std::cout<<"case 3 mirror:"<<"node value:"<<node->value<<std::endl<<std::flush;
            left_rotate(node->parent->parent);
            node->parent->is_black = true;
            node->parent->left->is_black = false;
        }
    }
    root->is_black = true;
}

template<typename T>
void rbtree<T>::transplant(Node_t<T>* origNode, Node_t<T>* newNode)
{
    if(origNode == root){
        root = newNode;
        return;
    }
    if(is_left(origNode)){
        origNode->parent->left = newNode;
    }else{
        origNode->parent->right = newNode;
    }
    if(newNode != nil){ // not necessary for rbtree since nil can be assigned any parent
        newNode->parent = origNode->parent;
    }
}

template<typename T>
void rbtree<T>::remove(Node_t<T>* node)
{
    bool need_to_fixup = node->is_black;
    auto fixup_node = nil;
    if(node->left == nil){
        fixup_node = node->right;
        transplant(node, node->right);
    }else if(node->right == nil){
        fixup_node = node->left;
        transplant(node, node->left);
    }else{
        auto rightChild = node->right;
        auto newSubRoot = rightChild;
        while(newSubRoot->left != nil){
            newSubRoot = newSubRoot->left;
        }
        need_to_fixup = newSubRoot->is_black;
        fixup_node = newSubRoot->right;
        if(newSubRoot != rightChild){
            transplant(newSubRoot, newSubRoot->right);
            newSubRoot->right = rightChild;
            rightChild->parent = newSubRoot;
        }
        transplant(node, newSubRoot);    
        newSubRoot->left = node->left;
        node->left->parent = newSubRoot;
        newSubRoot->is_black = node->is_black;
    }
    if(need_to_fixup){
        remove_fixup(fixup_node);
    }
}

enum TreeFixupState{
    TreeState_Case_1 = 0,
    TreeState_Case_2,
    TreeState_Case_3,
    TreeState_Case_4,
};

template<typename T>
inline Node_t<T>* get_brother(Node_t<T>* node){
    return is_left(node) ? node->parent->right : node->parent->left;
}

template<typename T>
static TreeFixupState get_tree_state(Node_t<T>* node){
    Node_t<T>* brother = get_brother(node);
    if(brother->is_black == false) 
        return TreeState_Case_1;
    else if(brother->left->is_black && brother->right->is_black)
        return TreeState_Case_2;
    else if((is_left(node)&& brother->right->is_black == false)
            || (is_left(node) == false && brother->left->is_black == false))
        return TreeState_Case_4;
    else 
        return TreeState_Case_3;
}

template<typename T>
void rbtree<T>::remove_fixup(Node_t<T>* node)
{
    while(node->is_black && node->parent != nil){
        TreeFixupState state = get_tree_state(node);
        Node_t<T>* brother = get_brother(node);
        switch(state){
        case TreeState_Case_1:
            if(is_left(node)){
                left_rotate(node->parent);
                node->parent->is_black = false;
                node->parent->parent->is_black = true;
            }
            break;
        case TreeState_Case_2:
            brother->is_black = false;
            node = node->parent;
            break;
        case TreeState_Case_3:
            if(is_left(brother)){
                left_rotate(brother);
            }else{
                right_rotate(brother);
            }
            brother->is_black = false;
            brother->parent->is_black = true;
            break;
        case TreeState_Case_4:
            if(is_left(node)){
                left_rotate(node->parent);
            }else{
                right_rotate(node->parent);
            }
            std::swap(node->parent->is_black, node->parent->parent->is_black);
            get_brother(node->parent)->is_black = false;
            break;
        default:
            return;
       }
    }
    node->is_black = true;
}

// MyMap.h

#ifndef MYMAP_H
#define MYMAP_H

template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap(){
        m_root = nullptr;
        m_size = 0;
    }
    
    ~MyMap(){
        freeTree(m_root);
    }
    
    void clear(){
        freeTree(m_root);
        m_root = nullptr; //set root to nullptr after node is deleted
    }
    
    int size() const{
        return m_size;
    }
    
    void associate(const KeyType& key, const ValueType& value){
        if(m_root == nullptr){          //if no nodes, just add new node to root
            m_root = new Node(key, value);
            m_size++;
            return;
        }
        Node* ptr = m_root;
        for(;;){    //repeatedly until function returns
            if(ptr->m_key == key){      //if key already exists, just update its value to avoid duplicates
                ptr->m_value = value;
                return;
            }
            if(key < ptr->m_key){       //if new key is less than key at node, visit the left node pointer
                if (ptr->left != nullptr)   //if left node pointer is not nullptr, go to the node
                    ptr = ptr->left;
                else{
                    ptr->left = new Node(key, value); //if left node is a nullptr, just set a new node to the empty space
                    m_size++;
                    return;
                }
            }else if(key > ptr->m_key){ //if new key is greater than key at node, visit the right node pointer
                if (ptr->right != nullptr) //if right node pointer is not nullptr, go to the node
                    ptr = ptr->right;
                else{
                    ptr->right = new Node(key, value);  //if right node is a nullptr, set a new node at empty space
                    m_size++;
                    return;
                }
            }
        }
    }

	  // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const{
        if(m_root == nullptr){  //if root is nullptr, the map is empty, return nullptr
            return nullptr;
        }
        Node* ptr = m_root;
        for(;;){    //repeatedly until function returns
            if(ptr->m_key == key)   //if current pointer points to node with desired key, return pointer to the value
                return &(ptr->m_value);
            if(key < ptr->m_key){   //if desired key is less than pointer key
                if (ptr->left != nullptr)   //if the pointer on the left is not nullptr, go to the pointer
                    ptr = ptr->left;
                else
                    return nullptr; //else, return nullptr, desired key is not found
                
            }else if(key > ptr->m_key){ //if desired key is greater than pointer key
                if (ptr->right != nullptr)  //if the pointer on the right is not nullptr, go to the pointer
                    ptr = ptr->right;
                else
                    return nullptr; //else, return nullptr, desired key is not found
            }
        }
    }

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    struct Node{
        Node(const KeyType &key, const ValueType &value):m_key(key), m_value(value){
            left = right = nullptr;
        }
        KeyType m_key;
        ValueType m_value;
        Node* left;
        Node* right;
    };
    
    Node* m_root;
    int m_size;
    
    void freeTree(Node* ptr){ //member function to free up tree, recursively calls for left and right subtree, then deletes root
        if(ptr == nullptr)
            return;
        freeTree(ptr->left);
        freeTree(ptr->right);
        delete ptr;
        m_size--;
    }
};
#endif





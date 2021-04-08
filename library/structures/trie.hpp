#ifndef _LIBRARY_STRUCTURES_TRIE_HPP_
#define _LIBRARY_STRUCTURES_TRIE_HPP_

#include <string>
#include <iostream>
#include <optional>
#include <map>
#include <vector>
#include <deque>

template<class T>
struct empty_item { 
  typedef T type; 
};

template<class T>
typename empty_item<T>::type item();

/// 'trie_node' structure defines the nodes of the trie.
template <typename T>
struct trie_node {
  // Constructs a node with value 'val'. A node by default is not a leaf node.  
  trie_node(T& val): val(val), is_leaf(false) {};
  // A trie node contains a value 'val', a map of the pointers of its children
  // nodes, and a boolean whick defines whether it is a leaf not or not.
  T val;
  std::map<T,trie_node<T>*> children;
  bool is_leaf;
};

/// 'trie' class implements the trie data structure structure. This class holds
/// a pointer to the root. [https://en.wikipedia.org/wiki/Trie]
/// This implementation supports the storage of any type of value, but it was
/// mainly build to set a word, as value in every node, so that it work as a
/// dictionary for the key-value storage.
template <class T>
class trie {
  public:
    using Map = std::map<T,trie_node<T>*>;

    // 'trie' constructor initializes the trie root with a dummy value 'val' 
    // and sets the 'is_leaf' boolean to false.
    trie(T& val) {
      root = new struct trie_node<T>(val);
    }

    // Inserts a new value in trie.
    template <typename V>
    bool insert(V& values) {
      trie_node<T>* curr = root;
      trie_node<T>* start = root;
      for(auto& val:values) {
        // if value doesn't exist, we create a new node and add its pointer to
        // its parents map.
        trie_node<T>* node = nullptr;
        typename Map::iterator it = curr->children.find(val);
        if (it == curr->children.end()) {
          node = new trie_node<T>(val);
          curr->children[val] = node;
        } else {
          node = it->second;
        }
        // continue traversing the trie until a value is not found.
        curr = node;
      }
      // last inserted node is marked as leaf.
      curr->is_leaf = true;
      // return success
      return true;
    }

    // Returns the values of the top-level key 'key'. If the key does not exist
    // it in the top level layer, it returns false.
    std::vector<T> get(T& key) {
      std::vector<T> v;
      auto curr = root->children;
      // if key doesn't exist return false
      if (curr.find(key) == curr.end()) {
        return v;
      }
      // cals print function to traverse the trie branch
      std::deque<T> dq;
      dq.push_back(key);
      v = get(curr[key]->children, dq, v);
      // return success
      return v;
    }

    // Calls 'get' method for a specific key. See line 58 for more details. 
    std::vector<T> query(T& val) {
      return get(val);
    }

    // Prints the value of any 'key', having its path. If the key does not
    // exist it returns false.
    T query(std::vector<T>& key_path) {
      // traverse the branch until end is reached
      auto curr = root->children;
      for(auto& key:key_path) {
        // if one of the keys in the path doesn't exist return false
        if(curr.find(key) == curr.end()) {
          return {};
        }
        curr = curr[key]->children;
      }
      typename Map::iterator it;
      for (it=curr.begin(); it!=curr.end(); ++it) {
        if (it->second->is_leaf) {
          return it->first;
        }
      }
      return {};
    }

    // Prints the whole trie.
    void print() {
      auto curr = root->children;
      print(curr);
    }

    bool validity_check(std::vector<T>& key_path) {
      auto curr = root->children;
      return validity_check(curr);
    }
    
    // Deletes a top level 'key'. Returns true if the deletion was successful,
    // otherwise it returns false.
    bool del(T& key) {
      auto& curr = root->children;
      if (curr.find(key) == curr.end()) {
        return false;
      }
      // check whether key exists in the top level layer
      // delete the whole branch
      del(curr[key]->children);
      // delete the entry of key in the root map
      delete curr[key];
      curr.erase(key);
      // return success
      return true;
    }

    // '~trie' destructor, deletes the whole trie by traversing all branches.
    ~trie() {
      auto& curr = root->children;
      typename Map::iterator it;
      // call 'del' method to delete the branch of each child node of root
      for(it=curr.begin(); it!=curr.end();) {
        del(it->second->children);
        delete it->second;
        it = curr.erase(it);
      }
      delete root;
    }
  
  private:
    // Private 'print' method that recursevly traverses the trie, to print the
    // values of its nodes.
    void print(Map& curr) {
      if (curr.empty()) {
        return;
      }
      // recursevly call the method for each child node of 'curr'
      typename Map::iterator it;
      for(it=curr.begin(); it!=curr.end(); ++it) {
        std::cout << it->first << " ";
        print(it->second->children);
        std::cout << std::endl;
      }
    }

    std::vector<T> get(Map& curr, std::deque<T>& dq, std::vector<T>& v) {
      if (curr.empty()) {
        for(auto& item : dq) {
          v.push_back(item);
        }
        v.push_back({});
      }
      // recursevly call the method for each child node of 'curr'
      typename Map::iterator it;
      for(it=curr.begin(); it!=curr.end(); ++it) {
        dq.push_back(it->first);
        v = get(it->second->children, dq, v);
        dq.pop_back();
      }
      return v;
    }

    // Private 'del' method that recursevly traverses the trie, to delete the
    // values of the nodes of a specific branch.
    void del(Map& curr) {
      if (curr.empty()) {
        return;
      }
      // recursevly call the method for each child node of 'curr'
      typename Map::iterator it;
      for (it=curr.begin(); it!=curr.end();) {
        del(it->second->children);
        delete it->second;
        it = curr.erase(it);
      }
    }

    bool validity_check(Map curr) {
      if (curr.empty()) {
        return true;
      }
      typename Map::iterator it;
      bool ok = true;
      for(it=curr.begin(); it!=curr.end(); ++it) {
        if (it->second->is_leaf) {
          return false;
        }
        ok = ok && validity_check(it->second->children);
      }
      return ok;
    }

  private:
    // 'root' is a pointer to the root node of the trie. This pointer helps to
    // easily handle insertions / deletions in trie.
    trie_node<T>* root;
};



#endif // _LIBRARY_STRUCTURES_TRIE_HPP_

#ifndef _LIBRARY_STRUCTURES_TRIE_HPP_
#define _LIBRARY_STRUCTURES_TRIE_HPP_

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <deque>   

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
      for(auto& val:values) {
        trie_node<T>* node = curr->children[val];
        // if value doesn't exist, we create a new node and add its pointer to
        // its parents map. 
        if (!node) {
          node = new trie_node<T>(val);
          curr->children[val] = node;
        }
        // continue traversing the trie until a value is not found.
        curr = node;
      }
      // last inserted node is marked as leaf.
      curr->is_leaf = true;
      // return success
      return true;
    }

    // Prints the value of the top-level key 'key'. If the key does not exist
    // it in the top level layer, it returns false.
    std::string get(T& key) {
      auto curr = root->children;
      // if key doesn't exist return false
      if (curr.find(key) == curr.end()) {
        return "ERROR";
      }
      // cals print function to traverse the trie branch
      std::string ret_val;
      std::deque<T> s;
      s.push_back(key);
      ret_val = get(curr[key]->children, s, ret_val);
      // return success
      return ret_val;
    }

    // Calls 'get' method for a specific key. See line 58 for more details. 
    std::string query(T& val) {
      return get(val);
    }

    // Prints the value of any 'key', having its path. If the key does not
    // exist it returns false.
    std::string query(std::vector<T>& key_path) {
      // traverse the branch until end is reached
      auto curr = root->children;
      for(auto& key:key_path) {
        // if one of the keys in the path doesn't exist return false
        if(curr.find(key) == curr.end()) {
          return "NOT FOUND";
        }
        curr = curr[key]->children;
      }
      typename Map::iterator it;
      for (it=curr.begin(); it!=curr.end(); ++it) {
        if (it->second->is_leaf) {
          return it->first;
        }
      }
      return "NOT FOUND";
    }

    // Prints the whole trie.
    void print() {
      auto curr = root->children;
      print(curr);
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

    std::string get(Map& curr, std::deque<T>& s, std::string ret_val) {
      if (curr.empty()) {
        for(auto& item : s) {
          ret_val += " : ";
          ret_val += item;
        }
        ret_val += "\n";
        return ret_val;
      }
      // recursevly call the method for each child node of 'curr'
      typename Map::iterator it;
      for(it=curr.begin(); it!=curr.end(); ++it) {
        s.push_back(it->first);
        ret_val = get(it->second->children, s, ret_val);
        s.pop_back();
      }
      return ret_val;
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

  private:
    // 'root' is a pointer to the root node of the trie. This pointer helps to
    // easily handle insertions / deletions in trie.
    trie_node<T>* root;
};



#endif // _LIBRARY_STRUCTURES_TRIE_HPP_

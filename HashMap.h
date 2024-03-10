//
//  HashMap.h
//  Project_4
//
//  Created by Maia Smolyanov on 3/8/24.
//

#ifndef HashMap_h
#define HashMap_h


#include <string>
#include <list>
#include <vector>

template <typename T> class HashMap
{
public:
    HashMap(double max_load = 0.75); // constructor
    ~HashMap(); // destructor; deletes all of the items in the hashmap
    int size() const; // return the number of associations in the hashmap
    // The insert method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the hashmap with that key/value pair. If there is
    // already an association with that key in the hashmap, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the hashmap must contain no duplicate keys.
    void insert(const std::string& key, const T& value);
    // Defines the bracket operator for HashMap, so you can use your map like this: // your_map["david"] = 2.99;
    // If the key does not exist in the hashmap, this will create a new entry in // the hashmap and map it to the default value of type T (0 for builtin types). // It returns a reference to the newly created value in the map.
    T& operator[](const std::string& key);
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be // used to examine that value within the map.
    const T* find(const std::string& key) const;
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be // used to examine that value or modify it directly within the map.
    T* find(const std::string& key) {
        const auto& hm = *this;
        return const_cast<T*>(hm.find(key));
    }
    
private:
    HashMap(int n_buckets, double max_load);
    
    
    struct KeyValPair {
        std::string m_key;
        T m_value;
    };
    
    std::vector<std::list<KeyValPair>> m_associations;
    int m_nItems;
    int m_nBuckets;
    int m_maxNItems;
    double m_maxLoad;
    std::list<KeyValPair*> traverseList;
    
};

template<typename T>
HashMap<T>::HashMap(double max_load)
:HashMap<T>(10, max_load){}

template<typename T>
HashMap<T>::HashMap(int n_buckets, double max_load) {
    m_nItems = 0;
    m_nBuckets = n_buckets;
    m_maxNItems = max_load * m_nBuckets;
    m_associations.assign(m_nBuckets, {});
    m_maxLoad = max_load;
}

template<typename T>
HashMap<T>::~HashMap() {
    //TODO: delete all association in hashmap
}

template<typename T>
int HashMap<T>::size() const {
    return m_nItems;
}

// The insert method associates one item (key) with another (value).
// If no association currently exists with that key, this method inserts
// a new association into the hashmap with that key/value pair. If there is
// already an association with that key in the hashmap, then the item
// associated with that key is replaced by the second parameter (value).
// Thus, the hashmap must contain no duplicate keys.
template<typename T>
void HashMap<T>::insert(const std::string& key, const T& value) {
    //hash the key;
    size_t hashNum = std::hash<std::string>()(key) % m_nBuckets;
    
    std::list<KeyValPair> &bucket = m_associations[hashNum];
    for (typename std::list<KeyValPair>::iterator i  = bucket.begin(); i != bucket.end(); ++i) {
        if (i->m_key == key) {
            //key exists so update it's value and return
            i->m_value = value;
            return;
        }
    }
    
    //key does not already exist:
    
    //construct KeyValuePair to add
    KeyValPair newPair;
    newPair.m_key = key;
    newPair.m_value = value;
    
    //add KeyValuePair to hashmap in appropriate bucket
    bucket.push_back(newPair);
    traverseList.push_back(&bucket.back());
    m_nItems++;
    
    // If capacity goes over, create new map and rehash everything.
    
    if (m_nItems > m_maxNItems) {
        
        HashMap<T> newHashMap(m_nBuckets * 2, m_maxLoad);
        for (typename std::list<KeyValPair*>::iterator i = traverseList.begin(); i != traverseList.end(); ++i) {
            newHashMap.insert((*i)->m_key, (*i)->m_value);
        }
        
        //swap the hashmaps internally
        
        m_nItems = newHashMap.m_nItems;
        m_nBuckets = newHashMap.m_nBuckets;
        m_maxNItems = newHashMap.m_maxNItems;
        m_associations.swap(newHashMap.m_associations);
        m_maxLoad = newHashMap.m_maxLoad;
        traverseList.swap(newHashMap.traverseList);
    }
}

// Defines the bracket operator for HashMap, so you can use your map like this: // your_map["david"] = 2.99;
// If the key does not exist in the hashmap, this will create a new entry in // the hashmap and map it to the default value of type T (0 for builtin types). // It returns a reference to the newly created value in the map.
template<typename T>
T& HashMap<T>::operator[](const std::string& key) {
    T* found = find(key);
    if (found != nullptr) {
        return *found;
    }
    insert(key, T());
    return *find(key);
}

// If no association exists with the given key, return nullptr; otherwise,
 // return a pointer to the value associated with that key. This pointer can be
 // used to examine that value or modify it directly within the map.
template<typename T>
const T* HashMap<T>::find(const std::string& key) const {
    //compute hash number
    size_t hashNum = std::hash<std::string>()(key) % m_nBuckets;
    
    const std::list<KeyValPair> &bucket = m_associations[hashNum];
    for (typename std::list<KeyValPair>::const_iterator i  = bucket.begin(); i != bucket.end(); ++i) {
        if (i->m_key == key) {
            return &(i->m_value);
        }
    }
    return nullptr;
    
}

#endif /* HashMap_h */

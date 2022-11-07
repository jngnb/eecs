#ifndef HASHTABLE_H
#define HASHTABLE_H

// Project Identifier: 2C4A3C53CD5AD45A7BEA3AE5356A3B1622C9D04B

// INSTRUCTIONS:
// fill out the methods in the class below.

// You may assume that the key and value types can be copied and have default
// constructors.

// You can also assume that the key type has (==) and (!=) methods.

// You may assume that Hasher is a functor type with a
// size_t operator()(const Key&) overload.

// The key/value types aren't guaranteed to support any other operations.

// Do not add, remove, or change any of the class's member variables.
// The num_deleted counter is *optional*. You might find it helpful, but it
// is not required to pass the lab assignment.

// Do not change the Bucket type.

// SUBMISSION INSTRUCTIONS:
// Submit this file, by itself, in a .tar.gz.
// Other files will be ignored.

#include <cstdint>
#include <functional> // where std::hash lives
#include <vector>
#include <cassert> // useful for debugging!

// A bucket's status tells you whether it's empty, occupied, 
// or contains a deleted element.
enum class Status : uint8_t {
    Empty,
    Occupied,
    Deleted
};

template<typename K, typename V, typename Hasher = std::hash<K>>
class HashTable {
    // used by the autograder; do not change/remove.
    friend class Verifier;
public:
    // A bucket has a status, a key, and a value.
    struct Bucket {
        // Do not modify Bucket.
        Status status = Status::Empty;
        K key;
        V val;
    };

    HashTable() {
        buckets.resize(6);
        
    }

    size_t size() const {
        return num_elements;
    }

    // returns a reference to the value in the bucket with the key, if it
    // already exists. Otherwise, insert it with a default value, and return
    // a reference to the resulting bucket.
    V& operator[](const K& key) {
        
        Hasher hash;
        int deletedIdx = -1;
        
        size_t hashed = hash(key);
        
        for (size_t i = 0; i < buckets.size(); ++i){
            size_t hashIdx = (hashed + i) % buckets.size();
            Bucket &currBk = buckets[hashIdx];
            
            if (currBk.status == Status::Occupied){
                if (currBk.key == key){
                    return currBk.val;
                }
            }
            
            else if (currBk.status == Status:: Deleted){
                if (deletedIdx == -1){
                    deletedIdx = (signed) hashIdx;
                }
            }
            else{
                if (num_elements >= (buckets.size() / 2)){
                    rehash_and_grow();
                    for (size_t i = 0; i < buckets.size(); ++i){
                        hashIdx = (hashed + i) % buckets.size();
                        if (buckets[hashIdx].status != Status::Occupied){
                            break;
                        }
                    }
                    deletedIdx = -1;
                }
                
                if (deletedIdx != -1){
                    Bucket &deletedBk = buckets[deletedIdx];
                    deletedBk.status = Status::Occupied;
                    deletedBk.key = key;
                    deletedBk.val = V();
                    
                    num_elements++;
                    return deletedBk.val;
                }
                else{
                    Bucket &newBk = buckets[hashIdx];
                    newBk.status = Status::Occupied;
                    newBk.key = key;
                    newBk.val = V();
                    
                    num_elements++;
                    return newBk.val;
                }
            }
        }
        
        // if everything deleted and for loop terminates,
        // still insert at first deleted index
        Bucket &deletedBk = buckets[deletedIdx];
        deletedBk.status = Status::Occupied;
        deletedBk.key = key;
        deletedBk.val = V();
        
        num_elements++;
        return deletedBk.val;
    }

    // insert returns whether inserted successfully
    // (if the key already exists in the table, do nothing and return false).
    bool insert(const K& key, const V& val) {
        
        Hasher hash;
        int deletedIdx = -1;
        size_t hashed = hash(key);
        
        for (size_t i = 0; i < buckets.size(); ++i){
            size_t hashIdx = (hashed + i) % buckets.size();
            Bucket &currBk = buckets[hashIdx];
            
            if (currBk.status == Status::Occupied){
                if (currBk.key == key){
                    return false;
                }
            }
            else if (currBk.status == Status:: Deleted){
                if (deletedIdx == -1){
                    deletedIdx = (signed) hashIdx;
                }
            }
            else{
                
                if (num_elements >= (buckets.size() / 2)){
                    rehash_and_grow();
                    for (size_t i = 0; i < buckets.size(); ++i){
                        hashIdx = (hashed + i) % buckets.size();
                        if (buckets[hashIdx].status != Status::Occupied){
                            break;
                        }
                    }
                    deletedIdx = -1;
                }
                
                if (deletedIdx != -1){
                    Bucket &deletedBk = buckets[deletedIdx];
                    deletedBk.status = Status::Occupied;
                    deletedBk.key = key;
                    deletedBk.val = val;
                    
                    num_elements++;
                    return true;
                }
                else{
                    Bucket &newBk = buckets[hashIdx];
                    newBk.status = Status::Occupied;
                    newBk.key = key;
                    newBk.val = val;
                    
                    num_elements++;
                    return true;
                }
            }
        }
        
        // if everything deleted and for loop terminates,
        // still insert at first deleted index
        Bucket &deletedBk = buckets[deletedIdx];
        deletedBk.status = Status::Occupied;
        deletedBk.key = key;
        deletedBk.val = val;
        
        num_elements++;
        return true;
        
    }
    
    // erase returns the number of items remove (0 or 1)
    size_t erase(const K& key) {
        Hasher hash;
        num_deleted = 0;
        
        size_t hashed = hash(key);
        
        for (size_t i = 0; i < buckets.size(); ++i){
            size_t hashIdx = (hashed + i) % buckets.size();
            Bucket &currBk = buckets[hashIdx];
            
            if (currBk.status == Status::Occupied &&
                currBk.key == key){
                currBk.status = Status::Deleted;
                num_deleted++;
                num_elements--;
                
                return num_deleted;
            }
            else if (currBk.status == Status::Empty){
                return num_deleted;
            }
        }
        return num_deleted;
        
    }

private:
    size_t num_elements = 0;
    size_t num_deleted = 0; // OPTIONAL: you don't need to use num_deleted to pass
    std::vector<Bucket> buckets;

    void rehash_and_grow() {
        
        std::vector<Bucket> oldBuckets = buckets;
        
        size_t oldSize = buckets.size();
        size_t newSize = oldSize * 2;
        
        buckets.clear();
        buckets.resize(newSize);
        
        for (size_t i = 0; i < oldSize; ++i){
            Bucket tempBk = oldBuckets[i];
            if (tempBk.status == Status::Occupied){
                num_elements--;
                insert(tempBk.key, tempBk.val);
            }
        }
    }

    // You can add methods here if you like.
    // TODO
};

#endif // HASHTABLE_H

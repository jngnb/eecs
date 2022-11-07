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
        buckets.resize(1);
    }

    size_t size() const {
        return num_elements;
    }

    // returns a reference to the value in the bucket with the key, if it
    // already exists. Otherwise, insert it with a default value, and return
    // a reference to the resulting bucket.
    V& operator[](const K& key) {
        Hasher hash;
        
        size_t x = hash(key) % buckets.size();
        //Bucket bk = buckets[x];
        for (size_t i = 0; i < buckets.size(); ++i){
            Bucket &bk = buckets[x];
            if (bk.status == Status::Occupied && bk.key == key){
                return bk.val;
            }
            else if (bk.status != Status::Occupied){
                break;
            }
            
            if (x == buckets.size() - 1){
                x = 0;
            }
            else{
                x++;
            }
        }
        
        if (num_elements == buckets.size()){
            rehash_and_grow();
            x = hash(key) % buckets.size();
            
            while(buckets[x].status == Status::Occupied){
                x++;
            }
        }
        
        Bucket &bk_ref = buckets[x];
        bk_ref.status = Status::Occupied;
        bk_ref.key = key;
        bk_ref.val = V();
        num_elements++;
        
        
        return bk_ref.val;
        
    }

    // insert returns whether inserted successfully
    // (if the key already exists in the table, do nothing and return false).
    bool insert(const K& key, const V& val) {
        Hasher hash;
        
        size_t x = hash(key) % buckets.size();
        
        for (size_t i = 0; i < buckets.size(); ++i){
            Bucket &bk = buckets[x];
            if (bk.status == Status::Occupied && bk.key == key){
                return false;
            }
            else if (bk.status != Status::Occupied){
                break;
            }
            
            if (x == buckets.size() - 1){
                x = 0;
            }
            else{
                x++;
            }
        }
        
        if (num_elements == buckets.size()){
            rehash_and_grow();
            x = hash(key) % buckets.size();
            
            while(buckets[x].status == Status::Occupied){
                x++;
            }
        }
        
        Bucket &bk_ref = buckets[x];
        bk_ref.status = Status::Occupied;
        bk_ref.key = key;
        bk_ref.val = val;
        num_elements++;
        
        return true;
    
    }
    // erase returns the number of items remove (0 or 1)
    size_t erase(const K& key) {
        num_deleted = 0;
        Hasher hash;
        
        size_t x = hash(key) % buckets.size();
        for (size_t i = 0; i < buckets.size(); ++i){
            Bucket &bk = buckets[x];
            
            if (bk.status == Status::Occupied && bk.key == key){
                bk.status = Status::Deleted;
                num_deleted++;
                num_elements--;
                break;
            }
            
            if (x == buckets.size() - 1){
                x = 0;
            }
            else{
                x++;
            }
        }
        return num_deleted;
        // TODO
    }

private:
    size_t num_elements = 0;
    size_t num_deleted = 0; // OPTIONAL: you don't need to use num_deleted to pass
    std::vector<Bucket> buckets;

    void rehash_and_grow() {
        
        Hasher hash;
        size_t oldSize = num_elements;
        size_t newSize = oldSize * 2 + 1;
        
        std::vector<Bucket> newBuckets;
        newBuckets.resize(newSize);
        
        for (size_t i = 0; i < oldSize; ++i){
            
            size_t x = hash(buckets[i].key) % newSize;
            if (newBuckets[x].status != Status::Occupied){
                newBuckets[x] = buckets[i];
            }
            else{
                while (newBuckets[x].status == Status::Occupied){
                    x++;
                }
                newBuckets[x] = buckets[i];
            }
            
        }
        buckets = newBuckets;

    }

    // You can add methods here if you like.
    // TODO
};

#endif // HASHTABLE_H

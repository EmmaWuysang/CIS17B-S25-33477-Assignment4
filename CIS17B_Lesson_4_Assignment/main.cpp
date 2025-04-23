/* 
 * File:   main.cpp
 * Author: Emma Wuysang
 * Creator Notes: Works as required. V1. Next version, I would work on possible a function for the try catch so that isn't all that repetitive, etc.
 * Created on April 22, 2025 5:25 PM
 * Purpose: Develop a Simple Storage Management System  using Ordered and Unordered Collections
 */


// libraries
#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>
using namespace std;

// functions for run exceptions
class DuplicateItemException : public runtime_error {
public:
    DuplicateItemException(const string& msg) : runtime_error(msg) {}
};

class ItemNotFoundException : public runtime_error {
public:
    ItemNotFoundException(const string& msg) : runtime_error(msg) {}
};

// class for items and their information
class StoredItem {
private: // characteristics of said items
    string id;
    string description;
    string location;

public: // functions
    StoredItem(string id, string desc, string loc) // constructor
        : id(id), description(desc), location(loc) {}
    
    string getId() const { return id; }
    string getDescription() const { return description; }
    string getLocation() const { return location; }
};

// class for managing storage
class StorageManager {
private: // pointers to the maps items
    unordered_map<string, shared_ptr<StoredItem>> itemById;
    map<string, shared_ptr<StoredItem>> itemByDescription;

public: // functions
    void addItem(const shared_ptr<StoredItem>& item) {
        // added item to both maps, throw'd if ID already exists
        if (itemById.find(item->getId()) != itemById.end()){ // if cannot find the item's id till the end
            throw DuplicateItemException("Error: Item with ID " + item->getId() + " already exists!");
        }
        // if there is not a duplicate, the item is added
        itemById[item->getId()] = item;
        itemByDescription[item->getDescription()] = item;
    }

    shared_ptr<StoredItem> findById(const string& id) const {
        // returned item if found / threw ItemNotFoundException
        auto searchKey = itemById.find(id);
        if (searchKey == itemById.end()){ // if cannot find the item's id till the end
            throw ItemNotFoundException("Error: Item with ID " + id + " not found!");
        }
        return searchKey->second; // instead returns the value of the smart pointer
    }

    void removeItem(const string& id) {
        // removed from both maps, throw'd if not found
        auto searchKey = itemById.find(id);
        if (searchKey == itemById.end()){ // if cannot find the item's id till the end
            throw ItemNotFoundException("Error: Item with ID " + id + " not found!");
        }
        itemByDescription.erase((searchKey->second)->getDescription()); //removing the item from both of the maps
        itemById.erase(id);
        
    }

    void listItemsByDescription() const {
        // iterate over itemByDescription and print info
        for(auto& item:itemByDescription){
            cout<<"- "<<item.second->getDescription()<< ": " << item.second->getLocation() << endl;
        }
    }
};

void testDuplicateAddition();
void testItemNotFound();

// where program starts
int main() {
    // created StorageManager and tested for functionality
    StorageManager systemCongo; // first and only instance for storagemanager
    // all the different testing items 
    auto itemOne = make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 3, Shelf 1");
    auto itemTwo = make_shared<StoredItem>("ITEM002", "Fan Motor", "Aisle 2, Shelf 5");
    auto itemThree = make_shared<StoredItem>("ITEM003", "Artificial Leaves", "Aisle 4, Shelf 3");
    
    // testing for adding items
        // adding item 1
    cout<<"Adding item: " << itemOne->getId() << " - " << itemOne->getDescription()<<endl;
    systemCongo.addItem(itemOne);
        // adding item 2
    cout<<"Adding item: " << itemTwo->getId() << " - " << itemTwo->getDescription()<<endl;
    systemCongo.addItem(itemTwo);
    // testing for duplicates
    cout<<"Attempting to add " << itemOne->getId() << " again...\n";
    try{
        systemCongo.addItem(itemOne);
    } catch(const DuplicateItemException& error){
        cout << error.what() <<endl;
    }
    
    // finding item by Id and returning desc of it
    cout <<"Retrieving " << itemTwo->getId() << "..." << endl;
    try{
        auto itemSearch = systemCongo.findById(itemTwo->getId());
        cout << "Found: " << itemSearch->getDescription() << " at " << itemSearch->getLocation() << endl;
    } catch(const ItemNotFoundException& error){
        cout << error.what() <<endl;
    }
    
    // trying to remove an item not in the storage
    cout << "Removing ITEM003..." << endl;
    try{
        systemCongo.removeItem("ITEM003"); // will throw up an error since we never added 003
    } catch(const ItemNotFoundException& error){
        cout << error.what() <<endl;
    }
    
    // listing the items along with their description
    cout << "Items in Description Order:\n";
    systemCongo.listItemsByDescription();
    
    // exit the program!
    return 0;
}

// end of program and should follow sample output listed in program desc.
/**
 * vectordb.cpp
 * C++ source file that should contain implementation for member functions
 * - rows()
 * - get()
 * - add()
 * - update()
 * - remove()
 *
 * You need to modify this file to implement the above-mentioned member functions
 * as specified in the hand-out (Task 3)
 */

#include <iostream>
#include "vectordb.hpp"
#include <vector>

namespace nwen {
    VectorDbTable::VectorDbTable() {}//constructor
    VectorDbTable::~VectorDbTable() {}//destructor

    int VectorDbTable::rows() {
        return table.size();
    }

    movie* VectorDbTable::get(int index) {
        //check it aint 0 and not beyond bounds
        if (index >= 0 && index < table.size()) {
            return &table[index];
        } else {
            return nullptr;
        }
    }

    bool VectorDbTable::add( movie& m) {
        // Check if the movie record with the same id already exists
        for (const auto& record : table) {
            if (record.id == m.id) {
                return false; // Record with the same id already exists, insertion fails
            }
        }

        // Insert the record into the table
        table.push_back(m);
        return true; // Successfully inserted the record
    }

    bool VectorDbTable::update(unsigned long id, movie& m) {
        for (auto& record : table) {
            if (record.id == id) {
                record = m; // Update the record with the provided data
                return true; // Successfully updated the record
            }
        }
        return false; // Record with the provided id not found, update fails
    }

    bool VectorDbTable::remove(unsigned long id) {
        for (auto it = table.begin(); it != table.end(); ++it) {
            if (it->id == id) {
                table.erase(it); // Remove the record from the table
                return true; // Successfully removed the record
            }
        }
        return false; // Record with the provided id not found, removal fails
    }
}

/**
 * abstractdb.cpp
 * C++ source file that should contain implementation for member functions
 * - loadCSV()
 * - saveCSV()
 * 
 * You need to modify this file to implement the above-mentioned member functions
 * as specified in the hand-out (Tasks 4 and 5)
 */ 
#include "abstractdb.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

namespace nwen {
    bool AbstractDbTable::saveCSV(std::string filename) {
        // Open the file for writing
        std::ofstream file(filename);
        if (!file.is_open()) {
            cerr << "File opening failed" << endl;
            return false; // Unable to open the file for writing
        }

        // Write every record from the table into the file
        int num = this->rows();
        for (int i = 0; i < num; i++) {
            movie *record = this->get(i);
            file << record->id << ",\"" << record->title << "\"," << record->year << ",\"" << record->director << "\"\n";
            if (!file.good()) {
                file.close();
                return false; // Error occurred while writing to the file
            }
        }

        // Close the file
        file.close();
        return true; // File write successful
    }

    bool AbstractDbTable::loadCSV(std::string filename) {
        // Open the file for reading
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false; // Unable to open the file for reading
        }

        // Read in all lines from the file and add them to the table
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string field;
            std::vector<std::string> recordFields;

            // Parse the line by comma-separated fields
            while (std::getline(ss, field, ',')) {
                recordFields.push_back(field);
            }

            // Check if the line has the expected number of fields
            if (recordFields.size() != 4) {
                file.close();
                return false; // Line does not follow the expected format
            }

            // Create a movie record from the parsed fields and add it to the table
            movie record;
            try {
                record.id = std::stoul(recordFields[0]);
                std::strncpy(record.title, recordFields[1].c_str(), sizeof(record.title) - 1);
                record.title[sizeof(record.title) - 1] = '\0'; // Ensure null-termination
                record.year = std::stoi(recordFields[2]);
                std::strncpy(record.director, recordFields[3].c_str(), sizeof(record.director) - 1);
                record.director[sizeof(record.director) - 1] = '\0'; // Ensure null-termination

                add(record);
            } catch (const std::exception& e) {
                file.close();
                return false; // Error occurred while parsing the line
            }
        }

        // Close the file
        file.close();
        return true; // File read successful
    }
}

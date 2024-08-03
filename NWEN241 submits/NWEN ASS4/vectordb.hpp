/**
 * vectordb.hpp
 * C++ header file that should contain declaration for
 * - VectorDbTable class
 * 
 * You need to modify this file to declare VectorDbTable class 
 * as specified in the hand-out (Task 2)
 */ 

#ifndef VECTORDB_HPP
#define VECTORDB_HPP

#include "abstractdb.hpp"
#include <vector>

namespace nwen {
    class VectorDbTable : public AbstractDbTable {
    private:
        std::vector<movie> table; // Vector to store the movie records

    public:
        VectorDbTable();//Default constructor
        ~VectorDbTable();//Destructor

        int rows();//return # of rows
        movie* get(int i);//return pointer to movie at row i
        bool add(movie &m);//add movie to table
        bool update(unsigned long id, movie &m);//updates movie at id
        bool remove(unsigned long id);//removes whatever is at id
    };
}

#endif // VECTORDB_HPP

#pragma once 

#include "linkedList.hpp" 

template <typename T>
class Stack 
{
    private: 
        List<T> list; 
        size_t size; 

    public: 
        Stack() 
        {
            size = 0; 
        } 

        void pushMatrix(T newData) 
        {
            list.insert_end(newData); 
        } 

        T popMatrix() 
        {
            T endData;  
            list.pop_end(&endData); 
            return endData; 
        } 

        ~Stack() 
        {
        } 
}; 


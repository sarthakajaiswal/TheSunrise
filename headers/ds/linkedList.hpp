#pragma once 

#include <cstdlib> 
#include <assert.h> 

#include "vmath.h" 

typedef enum status 
{
    SUCCESS = 1, 
    LIST_EMPTY = -1, 
}status_t;  

template <typename T> 
class List 
{
    private: 
        class node 
        {
            friend class List; 

            T data; 
            node* prev; 
            node* next; 
        };        

        node* p_head_node; 

        node* get_new_node(T new_data) 
        {
            node* p_new_node = NULL; 

            p_new_node = new node; 
            p_new_node->data = new_data; 
            p_new_node->prev = NULL; 
            p_new_node->next = NULL; 
            return (p_new_node); 
        } 

        void generic_insert(node* p_beg, node* p_mid, node* p_end) 
        {
            p_mid->prev = p_beg; 
            p_mid->next = p_end; 
            p_beg->next = p_mid; 
            p_end->prev = p_mid; 
        } 

        void generic_delete(node* p_delete_node) 
        {
            p_delete_node->next->prev = p_delete_node->prev; 
            p_delete_node->prev->next = p_delete_node->next; 
            free(p_delete_node); 
            p_delete_node = NULL; 
        } 
    
    public: 
        List() 
        {
            p_head_node = NULL; 

            p_head_node = get_new_node(vmath::mat4::identity()); 
            p_head_node->prev = p_head_node; 
            p_head_node->next = p_head_node; 
        } 

        // data insertion 
        status_t insert_end(T new_data) 
        {
            generic_insert(p_head_node->prev, get_new_node(new_data), p_head_node); 
            return (SUCCESS); 
        } 

        // data removal 
        status_t pop_end(T* p_popped_data) 
        {
            if(is_empty() == true) 
                return (LIST_EMPTY); 

            *p_popped_data = p_head_node->prev->data; 
            return (SUCCESS); 
        } 

        bool is_empty() const 
        {
            if(p_head_node->prev == p_head_node && p_head_node->next == p_head_node) 
                return (TRUE); 
            return (FALSE); 
        } 

        // destruction 
        ~List() 
        {
            node* p_run = NULL; 
            node* p_run_next = NULL;

            p_run = p_head_node->next; 
            while(p_run != p_head_node) 
            {
                p_run_next = p_run->next; 
                free(p_run); 
                p_run = p_run_next; 
            } 

            free(p_head_node); 
            p_head_node = NULL; 
        } 
}; 


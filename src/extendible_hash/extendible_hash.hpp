#include <iostream>
#include <fstream>
#include "register.h"

#pragma once

using namespace std;

template <class T, class bucket>
class extendible_hash{

    private:
        /* header, datos ordenados segun hash */
	    fstream index_file;
        fstream data_file;
        /* profundidad global */
        int D;

        /* evaluates if index is empty (without keys) */
        bool index_empty();
        /* starts index with key, initialize its buckets with no elements */
        index_hash start_index(int key, int d);

        int index_size();
        int data_size();
        
        index_hash search_index(int key, int d);
        index_hash iterate_key(int key);

        bucket read_bucket(int position);
        index_hash read_index(int position);
        void write_bucket(bucket& ihb, int bucket_position);

        void resize(index_hash& ih, bucket& ihb, index_hash& ih_new, bucket& ihb_new, int d);
        void overflow_handle(index_hash& ih, bucket& ihb, T& registro);
        void add_to_bucket(index_hash& ih, bucket& ihb, T registro);
        void increment_depth(int key);
        
    public:
        
        extendible_hash(int D, string index_filename, string data_filename);
        ~extendible_hash();
        
        void insert_register(T registro);
        bool search_register(int registro_key, T& registro);
        bool delete_register(int registro_key);
        vector <T> select_all();
        void print_all();
        /* hash function */
        int f(int key);
};



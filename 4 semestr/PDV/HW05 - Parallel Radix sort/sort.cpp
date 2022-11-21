#include "sort.h"
#include <iostream>
#include <atomic>

using namespace std;


void my_rad_sort(vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
               unsigned long alphabet_size, unsigned long string_lengths) {

    unsigned long num_of_strings = vector_to_sort.size();
    unsigned long size_of_string = vector_to_sort.at(0)->size();

    vector< vector<string *> > buckets(alphabet_size, vector<string*>(0));
    vector<unsigned long> point(alphabet_size, 0);


    for(unsigned long i = 0; i < num_of_strings; i++){
        unsigned int index = mappingFunction(vector_to_sort[i]->at(size_of_string - string_lengths));
        buckets[index].push_back(vector_to_sort[i]);
    }

    if (string_lengths > 1){
#pragma omp parallel for schedule(dynamic)
        for(unsigned int i = 0; i < alphabet_size; i++){
            if(buckets[i].size() > 1){
                my_rad_sort(buckets[i], mappingFunction, alphabet_size, string_lengths-1);
            }
        }
    }

    for(unsigned int i = 1; i < alphabet_size; i++) {
        point[i] = point[i - 1] + buckets[i - 1].size();
    }



#pragma omp parallel for
    for(unsigned int a = 0; a < alphabet_size; a++) {
        unsigned int pos = point[a];
        unsigned int bucket_size = buckets[a].size();
        for(unsigned int b = 0; b < bucket_size; b++){
            vector_to_sort[pos + b] = buckets[a][b];
        }
    }
}

void radix_par(vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
               unsigned long alphabet_size, unsigned long string_lengths) {
    my_rad_sort(vector_to_sort, mappingFunction, alphabet_size, string_lengths);
}

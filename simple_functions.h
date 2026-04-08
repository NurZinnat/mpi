#ifndef SIMPLE_FUNCTIONS_H
#define SIMPLE_FUNCTIONS_H
#include <stdlib.h>

// предполгается, что масисвы не меньшей длины, чем size
void array_copy (size_t size, double *consumer, double *producer);
double max_from_array (size_t size, double *arr);
void print_array (size_t size, double *arr);
void array_add_to_first (size_t size, double *res_arr, double *arr);
void arr_set_val (size_t arr_size, double *arr, double val);
#endif // SIMPLE_FUNCTIONS_H
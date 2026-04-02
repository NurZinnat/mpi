#ifndef SIMPLE_FUNCTIONS_H
#define SIMPLE_FUNCTIONS_H
#include <stdlib.h>

// предполгается, что масисвы не меньшей длины, чем size
void array_copy (size_t size, double *consumer, double *producer);
double max_from_array (size_t size, double *arr);
void print_array (size_t size, double *arr);
#endif // SIMPLE_FUNCTIONS_H
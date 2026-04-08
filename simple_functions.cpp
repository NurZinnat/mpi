#include "simple_functions.h"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

void
array_copy (size_t size, double *consumer, double *producer)
{
  for (size_t i = 0; i < size; i++)
    consumer[i] = producer[i];
}

double
max_from_array (size_t size, double *arr)
{
  double res = arr[0];
  for (size_t i = 1; i < size; i++)
    res = std::max (res, arr[i]);
  return res;
}

void
print_array (size_t size, double *arr)
{
  for (size_t i = 0; i < size; i++)
    printf ("%10.3e ", arr[i]);
  printf ("\n");
}

void
array_add_to_first (size_t size, double *res_arr, double *arr)
{
  for (size_t i = 0; i < size; i++)
    res_arr[i] += arr[i];
}

void
arr_set_val (size_t arr_size, double *arr, double val)
{
  for (size_t i = 0; i < arr_size; i++)
    arr[i] = val;
}
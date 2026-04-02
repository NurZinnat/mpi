#include "reflection_vectors.h"
#include "simple_functions.h"

void
reflection_vectors_storage::set_reflection_storage (double *val)
{
  arr = val;
}

void
reflection_vectors_storage::push (size_t val_size, double *val)
{
  array_copy (val_size, arr + arr_size, val);
  arr_size += val_size;
}

void
reflection_vectors_storage::pop (size_t val_size, double *val)
{
  array_copy (val_size, val, arr + arr_size);
  arr_size -= val_size;
}

void
reflection_vectors_storage::push (reflection_vectors &rv)
{
  double *val = rv.get_arr ();
  size_t val_size = rv.get_rv_size ();
  array_copy (val_size, arr + arr_size, val);
  arr_size += val_size;
}

void
reflection_vectors_storage::pop (reflection_vectors &rv)
{
  double *val = rv.get_arr ();
  size_t val_size = rv.get_rv_size ();
  array_copy (val_size, val, arr + arr_size);
  arr_size -= val_size;
}
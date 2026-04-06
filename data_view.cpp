#include "data_view.h"

double *
data_view::get_arr () const
{
  return arr;
}

size_t
data_view::get_arr_size () const
{
  return arr_size;
}

void
data_view::set_arr (double *val)
{
  arr = val;
}

void
data_view::set_arr_size (size_t val)
{
  arr_size = val;
}

double *
data_view::get_shift_arr () const
{
  return arr + arr_size;
}

void
data_view::shift_minus (size_t shift)
{
  arr_size -= shift;
}

void
data_view::shift_plus (size_t shift)
{
  arr_size += shift;
}
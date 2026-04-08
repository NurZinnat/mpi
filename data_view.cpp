#include "data_view.h"
#include "simple_functions.h"
#include <stdio.h>

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

data_view &
data_view::operator= (const data_view &x)
{
  double *x_arr = x.get_arr ();
  size_t x_arr_size = x.get_arr_size ();
  array_copy (x_arr_size, arr, x_arr);
  return *this;
}

data_view &
data_view::operator+= (const data_view &x)
{
  double *x_arr = x.get_arr ();
  size_t x_arr_size = x.get_arr_size ();
  if (arr_size < x_arr_size)
    {
      printf ("data_view::operator=(const data_view &x)::операция не будет "
              "выполнена\n");
      return *this;
    }
  array_add_to_first (x_arr_size, arr, x_arr);
  return *this;
}
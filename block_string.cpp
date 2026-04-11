#include "block_string.h"
#include "simple_functions.h"

size_t
block_string::get_r_num () const
{
  return r_num;
}

size_t
block_string::get_str_size () const
{
  return str_size;
}

void
block_string::set_r_num (size_t value)
{
  r_num = value;
}

void
block_string::set_str_size (size_t value)
{
  str_size = value;
}

void
block_string::get_block (block_view &b, size_t c_b_index)
{
  size_t b_size = get_b_size ();
  b.set_arr (get_arr () + r_num * b_size * c_b_index);
  b.set_c_num (r_num);
  if (c_b_index == str_size - 1)
    {
      b.set_r_num (ost);
      b.set_arr_size (ost * r_num);
    }
  else
    {
      b.set_r_num (b_size);
      b.set_arr_size (b_size * r_num);
    }
}

void
block_string::set_block (block_view &b, size_t c_b_index)
{

  size_t b_size = get_b_size ();
  double *arr = get_arr () + r_num * b_size * c_b_index;
  double *b_arr = b.get_arr ();
  array_copy (b.get_arr_size (), arr, b_arr);
}

void
block_string::set_params (double *arr, size_t _str_size, size_t _r_num,
                          size_t _ost)
{
  str_size = _str_size;
  if (str_size == 0)
    {
      set_arr_size (0);
      return;
    }
  r_num = _r_num;
  ost = _ost;
  size_t b_size = get_b_size ();
  set_arr (arr);
  int arr_size = r_num * b_size * (str_size - 1) + r_num * ost;
  if (arr_size < 0)
    {
      printf (
          "arr_size = %d, r_num = %ld, b_size = %ld, str_size = %ld, ost = %ld\n",
          arr_size, r_num, b_size, str_size, ost);
    }
  set_arr_size (arr_size);
}

void
block_string::print_params ()
{
  printf ("block_string_params::\n");
  printf ("r_num = %ld, string_size = %ld, ost = %ld\n", r_num, str_size, ost);
}
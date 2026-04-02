#include "data.h"

execution_status
data::data_memory_allocate (size_t _arr_size)
{
  if (arr)
    {
      printf ("data::memory_allocate:: повторное выделение памяти\n");
      return execution_status::runtime_error;
    }
  arr = std::make_unique<double[]> (_arr_size);
  if (!arr)
    {
      return execution_status::memory_allocate;
    }
  arr_size = _arr_size;
  return execution_status::success;
}

execution_status
data::reinit (size_t n, const data &x)
{
  if (n > arr_size)
    {
      printf (
          "data::reinit: размер блока памяти недостаточен: %ld < %ld\n",
          arr_size, n);
      return execution_status::runtime_error;
    }
  double *x_arr = x.get_arr ();
  for (size_t i = 0; i < n; i++)
    arr[i] = x_arr[i];
  return execution_status::success;
}

double *
data::get_arr () const
{
  return arr.get ();
}

size_t
data::get_arr_size () const
{
  return arr_size;
}





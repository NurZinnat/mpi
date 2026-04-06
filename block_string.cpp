#include "block_string.h"

// Геттеры для ВСЕХ полей
size_t
block_string::get_local_b_r_index () const
{
  return local_b_r_index;
}

size_t
block_string::get_global_b_r_index () const
{
  return global_b_r_index;
}

size_t
block_string::get_start_r_index () const
{
  return start_r_index;
}

size_t
block_string::get_r_size () const
{
  return r_size;
}

size_t
block_string::get_shift () const
{
  return shift;
}

// Сеттеры для ВСЕХ полей
void
block_string::set_local_b_r_index (size_t value)
{
  local_b_r_index = value;
}

void
block_string::set_global_b_r_index (size_t value)
{
  global_b_r_index = value;
}

void
block_string::set_start_r_index (size_t value)
{
  start_r_index = value;
}

void
block_string::set_r_size (size_t value)
{
  r_size = value;
}

void
block_string::set_shift (size_t value)
{
  shift = value;
}

// Остальные методы
void
block_string::init_block_string (size_t m_size, size_t b_size, double *arr,
                                 size_t _local_index, size_t _global_index)
{
  init_m_sizes (m_size, b_size);
  set_arr (arr);
  set_arr_size (m_size * b_size);
  local_b_r_index = _local_index;
  global_b_r_index = _global_index;
  r_size = (global_b_r_index == (get_k () - 1) && get_r () != 0)
               ? get_r ()
               : get_b_size ();
  shift = m_size * b_size * local_b_r_index;
  bxb_size = r_size * b_size;
  ost_bxb_size = r_size * get_r ();
}

void
block_string::get_block (block_view &b, size_t c_b_index)
{
  size_t k = get_k ();
  size_t b_size = get_b_size ();
  b.set_arr (get_arr () + bxb_size * c_b_index);
  b.set_c_num (r_size);
  b.set_r_num (c_b_index == k - 1 ? get_r () : b_size);
  if (c_b_index == k - 1)
    b.set_arr_size (ost_bxb_size);
  else
    b.set_arr_size (bxb_size);
}

double

block_string::formula (size_t s, size_t i, size_t j)
{
  double res{};
  size_t m_size = get_m_size ();
  switch (s)
    {
    case 1:
      res = m_size - std::max (i, j);
      break;
    case 2:
      res = std::max (i, j) + 1;
      break;
    case 3:
      res = fabs (int (i) - int (j));
      break;
    case 4:
      res = double (1) / double (i + j + 1);
      break;
    default:
      break;
    }
  // if (part_index == 0)
  // printf ("res = %lf\n", res);
  return res;
}

void
block_string::init_by_formula (size_t s)
{
  size_t b_size = get_b_size ();
  size_t k = get_k ();
  double *arr = get_arr ();
  size_t arr_index = 0;
  size_t r_start = 0;
  size_t r_end = r_start + get_bost_size (global_b_r_index);
  r_start = b_size * global_b_r_index;
  r_end = r_start + get_bost_size (global_b_r_index);
  size_t c_start = 0;
  for (size_t global_b_c_index = 0; global_b_c_index < k; global_b_c_index++)
    {
      size_t c_end = c_start + get_bost_size (global_b_c_index);
      for (size_t c_index = c_start; c_index < c_end; c_index++)
        for (size_t r_index = r_start; r_index < r_end; r_index++)
          {
            arr[arr_index++] = formula (s, r_index, c_index);
          }
      c_start += b_size;
    }
}

execution_status
block_string::read_b_row_from_file_to_arr (FILE *fp)
{
  size_t m_size = get_m_size ();
  size_t b_size = get_b_size ();
  size_t k = get_k ();
  size_t rb_size = get_bost_size (global_b_r_index);
  size_t step = rb_size * b_size;
  double *arr = get_arr ();
  for (size_t i = 0; i < rb_size; i++)
    {
      double *sub_arr = arr;
      for (size_t j = 0; j < k; j++)
        {
          size_t cb_size = get_bost_size (j);
          for (size_t u = 0; u < cb_size; u++)
            if (fscanf (fp, "%lf", &sub_arr[u * rb_size]) != 1)
              return execution_status::read;
          sub_arr += step;
        }
      arr++;
    }
  return execution_status::success;
}
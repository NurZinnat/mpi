#include "matrix_part.h"
#include "simple_functions.h"

double *
matrix_part::get_b_row_pointer (size_t b_row_index)
{
  size_t m_size = get_m_size ();
  size_t b_size = get_b_size ();
  return get_arr () + m_size * b_size * b_row_index;
}

size_t
matrix_part::get_part_size ()
{
  return part_size;
}

execution_status
matrix_part::init_matrix_part (size_t m_size, size_t b_size, size_t _index,
                               size_t _p)
{
  init_m_sizes (m_size, b_size);
  part_index = _index;
  p = _p;
  size_t k = get_k ();
  size_t r = get_r ();
  part_size = k / p + (k % p && part_index <= (k - 1) % p ? 1 : 0);
  // printf ("k = %ld p = %ld part_size = %ld\n", k, p, part_size);
  size_t arr_size
      = m_size * b_size * (part_size - 1)
        + (r != 0 && (k - 1) % p == part_index ? m_size * r : m_size * b_size);
  // printf ("arr_size = %ld\n", arr_size);
  ost_flag = part_index + p * (part_size - 1) == k - 1 && r != 0;
  return data_memory_allocate (arr_size);
}

size_t
matrix_part::get_local_bost_size (size_t index)
{
  if (index == part_size - 1 && ost_flag)
    return get_r ();
  else
    return get_b_size ();
}
void
matrix_part::copy_b_str (size_t str_index, double *arr)
{
  double *part_arr = get_arr ();
  size_t b_size = get_b_size ();
  size_t m_size = get_m_size ();
  size_t k = get_k ();
  size_t r = get_r ();
  part_arr += m_size * b_size * str_index;
  size_t size = m_size * b_size;
  if (str_index * p + part_index == k - 1)
    size = m_size * r;
  array_copy (size, part_arr, arr);
}




execution_status
matrix_part::get_row_part (size_t row_index, size_t row_size, double *row)
{
  double *arr = get_arr ();
  size_t b_size = get_b_size ();
  size_t m_size = get_m_size ();
  size_t global_b_row_index = row_index / b_size;
  if (global_b_row_index % p != part_index)
    return execution_status::runtime_error;
  size_t local_b_row_index = global_b_row_index / p;
  size_t rb_size = get_bost_size (global_b_row_index);
  size_t step = b_size * rb_size;
  size_t row_index_in_block = (row_index % b_size);
  arr += b_size * m_size * local_b_row_index + row_index_in_block;
  size_t k_stop = row_size / b_size + 1;
  for (size_t i = 0; i < k_stop; i++)
    {
      size_t cb_size = get_bost_size (i);
      for (size_t u = 0; u < cb_size; u++)
        row[u] = arr[rb_size * u];
      row += b_size;
      arr += step;
    }
  return execution_status::success;
}

execution_status
matrix_part::get_col_part (size_t col_index, size_t b_index, double *col)
{
  double *arr = get_arr ();
  size_t b_size = get_b_size ();
  size_t m_size = get_m_size ();
  size_t global_b_col_index = col_index / b_size;
  if (b_index % p != part_index)
    return execution_status::runtime_error;
  size_t local_b_row_index = b_index / p;
  size_t rb_size = get_bost_size (b_index);
  size_t col_index_in_blcok = col_index % b_size;
  arr += m_size * b_size * local_b_row_index
         + b_size * rb_size * global_b_col_index
         + col_index_in_blcok * rb_size;
  array_copy (rb_size, col, arr);
  return execution_status::success;
}

size_t
matrix_part::calculate_norm_part (size_t global_b_col_index, double *norm_part)
{
  double *arr = get_arr ();
  size_t b_size = get_b_size ();
  size_t m_size = get_m_size ();
  size_t r = get_r ();
  size_t k = get_k ();
  arr += b_size * b_size * global_b_col_index;
  size_t cb_size = get_bost_size (global_b_col_index);
  size_t step = m_size * b_size;
  for (size_t i = 0; i < cb_size; i++)
    norm_part[i] = 0;
  for (size_t i = 0; i < part_size - 1; i++)
    {
      double *sub_arr = arr;
      for (size_t j = 0; j < cb_size; j++)
        {
          for (size_t u = 0; u < b_size; u++)
            {
              norm_part[j] += fabs (sub_arr[u]);
            }
          sub_arr += b_size;
        }
      arr += step;
    }
  size_t global_b_row_index = p * (part_size - 1) + part_index;
  if (global_b_row_index == k - 1 && r)
    arr -= (b_size - r) * b_size * global_b_col_index;
  size_t rb_size = get_bost_size (global_b_row_index);
  double *sub_arr = arr;
  for (size_t j = 0; j < cb_size; j++)
    {
      for (size_t u = 0; u < rb_size; u++)
        {
          norm_part[j] += fabs (sub_arr[u]);
        }
      sub_arr += cb_size;
    }
  arr += step;
  return cb_size;
}

void
matrix_part::set_norm (double val)
{
  norm = val;
}

void
matrix_part::get_block_view (block_view &b, size_t local_r_index,
                             size_t c_index)
{
  size_t m_size = get_m_size ();
  size_t b_size = get_b_size ();
  double *arr = get_arr ();
  arr += m_size * b_size * local_r_index;
  size_t rb_size = get_local_bost_size (local_r_index);
  arr += rb_size * b_size * c_index;
  b.set_arr (arr);
  b.set_r_num (rb_size);
  b.set_c_num (get_bost_size (c_index));
  b.set_m_type (matrix_type::ordinary);
}

void
matrix_part::set_block (block &b, size_t local_r_index, size_t c_index)
{
  double *arr = get_arr () + get_ordinary_b_str_size () * local_r_index;
  size_t b_size = get_b_size ();
  size_t rb_size = get_local_bost_size (local_r_index);
  arr += rb_size * b_size * c_index;
  size_t cb_size = get_bost_size (c_index);
  double *b_arr = b.get_arr ();
  b.set_r_num (rb_size);
  b.set_c_num (cb_size);
  size_t size = rb_size * cb_size;
  array_copy (size, arr, b_arr);
}

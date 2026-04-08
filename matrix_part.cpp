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

  return data_memory_allocate (arr_size);
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

double
matrix_part::formula (size_t s, size_t i, size_t j)
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
matrix_part::init_by_formula (size_t s)
{
  // printf ("kaka\n");
  size_t b_size = get_b_size ();
  size_t k = get_k ();
  double *arr = get_arr ();
  size_t arr_index = 0;
  size_t r_start = 0;
  // size_t arr_size = get_arr_size ();
  // printf ("part_size = %ld arr_size = %ld\n", part_size, arr_size);
  for (size_t local_b_row_index = 0; local_b_row_index < part_size;
       local_b_row_index++)
    {
      size_t global_b_row_index = part_index + local_b_row_index * p;
      size_t r_end = r_start + get_bost_size (global_b_row_index);
      r_start = b_size * global_b_row_index;
      r_end = r_start + get_bost_size (global_b_row_index);
      size_t c_start = 0;
      for (size_t global_b_col_index = 0; global_b_col_index < k;
           global_b_col_index++)
        {
          size_t c_end = c_start + get_bost_size (global_b_col_index);
          for (size_t c_index = c_start; c_index < c_end; c_index++)
            for (size_t r_index = r_start; r_index < r_end; r_index++)
              {
                arr[arr_index++] = formula (s, r_index, c_index);
              }
          c_start += b_size;
        }
    }
}

execution_status
matrix_part::read_b_row_from_file_to_arr (size_t global_b_row_index,
                                          size_t &size, double *arr, FILE *fp)
{
  size_t m_size = get_m_size ();
  size_t b_size = get_b_size ();
  size_t k = get_k ();
  size_t rb_size = get_bost_size (global_b_row_index);
  size = rb_size * m_size;
  for (size_t i = 0; i < rb_size; i++)
    {
      size_t c_step = rb_size * b_size;
      size_t b_start = 0;
      for (size_t j = 0; j < k; j++)
        {
          size_t cb_size = get_bost_size (j);
          size_t start = b_start + cb_size * i;
          size_t end = start + cb_size;
          for (size_t u = start; u < end; u++)
            if (fscanf (fp, "%lf", &arr[u]) != 1)
              {
                return execution_status::read;
              }
          b_start += c_step;
        }
    }
  return execution_status::success;
}

execution_status
matrix_part::get_row_part (size_t row_index, size_t row_size, double *row)
{
  double *arr = get_arr ();
  size_t b_size = get_b_size ();
  size_t m_size = get_m_size ();
  size_t r = get_r ();
  size_t k = get_k ();
  size_t global_b_row_index = row_index / b_size;
  if (global_b_row_index % p != part_index)
    return execution_status::runtime_error;
  size_t local_b_row_index = global_b_row_index / p;
  size_t rb_size = get_bost_size (global_b_row_index);
  size_t step = b_size * rb_size;
  size_t row_index_in_block = (row_index % b_size);
  arr += b_size * m_size * local_b_row_index + row_index_in_block * b_size;
  size_t k_stop = row_size / b_size + 1;
  // printf (
  //     "\n\n\n\n\n\n\n\nk_stop = %ld row_index = %ld global_b_row_index = %ld
  //     " "local_b_row_index = %ld\n\n", k_stop, row_index,
  //     global_b_row_index, local_b_row_index);
  for (size_t i = 0; i < k_stop - 1; i++)
    {

      array_copy (b_size, row, arr);
      row += b_size;
      arr += step;
    }
  if (k_stop == k)
    arr -= row_index_in_block * (b_size - r);
  array_copy (b_size, row, arr);
  row += b_size;
  arr += step;

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
  size_t cb_size = get_bost_size (global_b_col_index);
  size_t rb_size = get_bost_size (b_index);

  arr += m_size * b_size * local_b_row_index
         + b_size * rb_size * global_b_col_index + col_index % b_size;
  for (size_t i = 0; i < rb_size; i++)
    col[i] = arr[i * cb_size];
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
  for (size_t i = 0; i < part_size - 1; i++)
    {
      double *sub_arr = arr;
      for (size_t j = 0; j < b_size; j++)
        {
          for (size_t u = 0; u < cb_size; u++)
            {
              norm_part[u] += fabs (sub_arr[u]);
            }
          sub_arr += cb_size;
        }
      arr += step;
    }
  size_t global_b_row_index = p * (part_size - 1) + part_index;
  if (global_b_row_index == k - 1 && r)
    {
      arr -= (b_size - r) * b_size * global_b_col_index;
    }
  size_t rb_size = get_bost_size (global_b_row_index);
  double *sub_arr = arr;
  for (size_t j = 0; j < rb_size; j++)
    {
      for (size_t u = 0; u < cb_size; u++)
        {
          norm_part[u] += fabs (sub_arr[u]);
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

size_t
matrix_part::get_local_bost_size (size_t index)
{
  size_t global_index = part_index + index * p;
  return get_bost_size (global_index);
}
void
matrix_part::get_block (block_view &b, size_t local_r_index, size_t c_index)
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
matrix_part::set_block (block_view &b, size_t local_r_index, size_t c_index)
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

void
matrix_part::get_b_str (block_string &str, size_t local_index, size_t start,
                        size_t end)
{
  size_t b_size = get_b_size ();
  size_t m_size = get_m_size ();
  size_t r_num = get_local_bost_size (local_index);
  double *arr = get_arr () + b_size * m_size;
  str.set_params (start, end, r_num, arr);
}
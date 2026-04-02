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
  part_size = k / p + (part_index <= (k - 1) % p ? 1 : 0);
  // printf ("k = %ld p = %ld part_size = %ld\n", k, p, part_size);

  execution_status status = data_memory_allocate (
      m_size * b_size * (part_size - 1) + ((p * part_size + part_index)
              == k - 1
          ? m_size * get_r ()
          : m_size * b_size));
  if (status != execution_status::success)
    return status;

  return execution_status::success;
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
  // printf ("part_size = %ld\n", part_size);
  size_t arr_size = get_arr_size ();
  printf ("arr_size = %ld\n", arr_size);

  for (size_t local_b_row_index = 0; local_b_row_index < part_size;
       local_b_row_index++)
    {
      // printf ("flasg\n");
      if (local_b_row_index == 3)
        return;
      size_t global_b_row_index = part_index + local_b_row_index * p;
      size_t r_end = r_start + get_bost_size (global_b_row_index);
      r_start = b_size * global_b_row_index;
      r_end = r_start + get_bost_size (global_b_row_index);
      size_t c_start = 0;
      for (size_t global_b_col_index = 0; global_b_col_index < k;
           global_b_col_index++)
        {
          size_t c_end = c_start + get_bost_size (global_b_col_index);
          for (size_t r_index = r_start; r_index < r_end; r_index++)
            for (size_t c_index = c_start; c_index < c_end; c_index++)
              {
                arr[arr_index++] = formula (s, r_index, c_index);
                if (arr_index == arr_size)
                  {
                    printf ("local_b_row_index = %ld, global_b_col_index = "
                            "%ld, r_index = %ld, c_index = %ld\n",
                            local_b_row_index, global_b_col_index, r_index,
                            c_index);
                  }
                // printf ("suka\n");
              }
          c_start += b_size;
        }
    }
  // printf ("end_kaka\n");
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
                fclose (fp);
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
  size_t global_b_row_index = row_index / b_size;
  if (global_b_row_index % p != part_index)
    return execution_status::runtime_error;
  size_t local_b_row_index = global_b_row_index / p;
  size_t step = b_size * get_bost_size (global_b_row_index);
  arr += b_size * m_size * local_b_row_index + (row_index % b_size) * b_size;
  size_t k_stop = (row_size / b_size + 1);
  for (size_t i = 0; i < k_stop; i++)
    {
      size_t cb_size = get_bost_size (i);
      array_copy (cb_size, row, arr);
      row += cb_size;
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
  for (size_t i = 0; i < k - 1; i++)
    {
      size_t rb_size = get_bost_size (i);
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
    }
  arr -= (b_size - r) * b_size * global_b_col_index;
  size_t rb_size = get_bost_size (k - 1);
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
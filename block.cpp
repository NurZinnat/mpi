#include "block.h"

// gets::

size_t
block::get_r_num () const
{
  return r_num;
}

size_t
block::get_c_num () const
{
  return c_num;
}
matrix_type
block::get_m_type ()
{
  return m_type;
}
// gets_end

// sets::

void
block::set_r_num (size_t _r_num)
{
  r_num = _r_num;
}

void
block::set_c_num (size_t _c_num)
{
  c_num = _c_num;
}
void
block::set_m_type (matrix_type _m_type)
{
  m_type = _m_type;
}
// sets_end

void
block::print ()
{
  printf ("\n\nblock_print:\n");
  printf ("r_num = %ld\nc_num = %ld\n", r_num, c_num);
  double *arr = get_arr ();
  for (size_t i = 0; i < r_num; i++)
    {
      size_t index = c_num * i;
      for (size_t j = 0; j < c_num; j++)
        printf (" %10.3e", arr[index++]);
      printf ("\n");
    }
  printf ("\n\n\n");
}

block &
block::operator= (const block &x)
{
  size_t x_b_size = x.get_b_size ();
  size_t x_r_num = x.get_r_num ();
  size_t x_c_num = x.get_c_num ();
  size_t b_size = get_b_size ();
  if (b_size != x_b_size)
    {
      printf ("block::operator= (const block &x): разные размеры блоков: %ld "
              "and %ld - Операция не будет выполнена\n",
              b_size, x_b_size);
      return *this;
    }

  if (reinit (x_r_num * x_c_num, (const data &)x) == execution_status::success)
    {
      r_num = x_r_num;
      c_num = x_c_num;
    }
  return *this;
}

block &
block::operator+= (const block &x)
{
  size_t x_b_size = x.get_b_size ();
  size_t x_r_num = x.get_r_num ();
  size_t x_c_num = x.get_c_num ();
  size_t b_size = get_b_size ();
  if (b_size != x_b_size || r_num != x_r_num || c_num != x_c_num)
    {
      printf ("block::operator+= (const block &x): разные размеры блоков: "
              "b_size = %ld r_num = %ld c_num = %ld\n "
              "x_b_size = %ld x_r_num = %ld x_c_num = %ld\n - Операция не "
              "будет выполнена\n",
              b_size, r_num, c_num, x_b_size, x_r_num, x_c_num);
      return *this;
    }

  double *arr = get_arr ();
  double *x_arr = x.get_arr ();
  size_t size = r_num * c_num;
  for (size_t i = 0; i < size; i++)
    arr[i] += x_arr[i];
  return *this;
}

execution_status
block::init_block (size_t _b_size, double _eps)
{
  r_num = c_num = _b_size;
  set_b_size (_b_size);
  set_eps (_eps);
  execution_status status
      = static_cast<data *> (this)->data_memory_allocate (_b_size * _b_size);
  if (status != execution_status::success)
    return status;
  zero_padding ();
  return execution_status::success;
}
bool
block::square ()
{
  if (r_num == c_num)
    return true;
  return false;
}

execution_status
block::inverse_triangular_upper (block &x)
{
  double eps = get_eps ();
  if (&x == this)
    {
      printf ("block::inverse_upper_triangular:: исходный и результирующий "
              "блоки совпадают\n");
      return execution_status::inval_arg;
    }
  if (get_arr_size () < x.get_arr_size ())
    {
      printf ("block::inverse_upper_triangular:: результирующий блок меньше "
              "по размеру. Требуется перевыдление памяти\n");
      return execution_status::runtime_error;
    }
  if (x.get_m_type () != matrix_type::triangular_upper)
    {
      printf ("block::inverse_upper_triangular: обращаемая матрица не "
              "верхнетреугольная\n");
      return execution_status::inval_arg;
    }
  set_r_num (x.get_r_num ());
  set_c_num (x.get_c_num ());
  size_t r_n = get_r_num (), c_n = get_c_num ();
  double *arr = get_arr (), *x_arr = x.get_arr ();

  // Инициализируем всю матрицу нулями
  for (size_t i = 0; i < r_n; i++)
    for (size_t j = 0; j < c_n; j++)
      arr[c_n * i + j] = 0;

  // Обработка последней строки
  double divider = x_arr[c_n * (r_n - 1) + c_n - 1];
  if (fabs (divider) < eps)
    return execution_status::small_divider;

  arr[c_n * (r_n - 1) + c_n - 1] = 1.0 / divider;

  // Обработка остальных строк снизу вверх
  for (int i = r_n - 2; i >= 0; i--)
    {
      divider = x_arr[c_n * i + i];
      if (fabs (divider) < eps)
        {
          return execution_status::small_divider;
        }

      // Диагональный элемент
      arr[c_n * i + i] = 1.0 / divider;

      // Наддиагональные элементы
      for (size_t j = i + 1; j < c_n; j++)
        {
          double sum = 0;
          for (size_t u = i + 1; u <= j; u++)
            sum += x_arr[c_n * i + u] * arr[c_n * u + j];
          arr[c_n * i + j] = -sum / divider;
        }
    }

  set_m_type (matrix_type::triangular_upper);
  return execution_status::success;
}

block &
block::operator*= (double val)
{
  size_t size = r_num * c_num;
  double *arr = get_arr ();
  for (size_t i = 0; i < size; i++)
    arr[i] *= val;
  return *this;
}
void
block::zero_padding ()
{
  double *arr = get_arr ();
  size_t size = get_b_size ();
  size = size * size;
  for (size_t i = 0; i < size; i++)
    arr[i] = 0.0;
}
#include "block_view.h"
#include "simple_functions.h"

void
block_view::print ()
{
  printf ("\n\nblock_view_print:\n");
  printf ("r_num = %ld\nc_num = %ld\n", get_r_num (), get_c_num ());
  double *arr = get_arr ();
  for (size_t i = 0; i < get_r_num (); i++)
    {
      size_t index = get_c_num () * i;
      for (size_t j = 0; j < get_c_num (); j++)
        printf (" %10.3e", arr[index++]);
      printf ("\n");
    }
  printf ("\n\n\n");
}

void
block_view::print_debug ()
{
  printf ("\n\nblock_view_print:\n");
  size_t r_num = get_r_num ();
  size_t c_num = get_c_num ();
  printf ("r_num = %ld\nc_num = %ld\n", r_num, c_num);
  double *arr = get_arr ();
  print_array (c_num * r_num, arr);
  printf ("\n\n\n");
}

void
block_view::init_block_view (size_t m_size, size_t b_size, double eps)
{
  init_m_sizes (m_size, b_size);
  set_r_num (b_size);
  set_c_num (b_size);
  set_eps (eps);
}

bool
block_view::square ()
{
  return get_r_num () == get_c_num ();
}

block_view &
block_view::operator= (const block_view &x)
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
  data_view::operator= (static_cast<const data_view &> (x));
  set_r_num (x_r_num);
  set_c_num (x_c_num);
  set_m_type (x.get_m_type ());
  return *this;
}

block_view &
block_view::operator+= (const block_view &x)
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

  data_view::operator+= (static_cast<const data_view &> (x));
  return *this;
}

execution_status
block_view::inverse_triangular_upper (block_view &x)
{
  double eps = get_eps ();
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

execution_status
block_view::inverse_triangular_lower (block_view &x)
{
  double eps = get_eps ();
  if (get_arr_size () < x.get_arr_size ())
    {
      printf ("block::inverse_lower_triangular:: результирующий блок меньше "
              "по размеру. Требуется перевыделение памяти\n");
      return execution_status::runtime_error;
    }
  if (x.get_m_type () != matrix_type::triangular_lower)
    {
      printf ("block::inverse_lower_triangular: обращаемая матрица не "
              "нижнетреугольная\n");
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

  // Обработка первой строки (диагональный элемент [0][0])
  double divider = x_arr[0]; // x_arr[c_n * 0 + 0]
  if (fabs (divider) < eps)
    return execution_status::small_divider;

  arr[0] = 1.0 / divider;

  // Обработка остальных строк сверху вниз
  for (size_t i = 1; i < r_n; i++)
    {
      // Диагональный элемент
      divider = x_arr[c_n * i + i];
      if (fabs (divider) < eps)
        {
          return execution_status::small_divider;
        }

      arr[c_n * i + i] = 1.0 / divider;

      // Поддиагональные элементы (в нижнетреугольной матрице это элементы слева от диагонали)
      // Для L^{-1}: L_ii * X_ij + sum(L_ik * X_kj) = 0, где k < i и k <= j
      // Так как результат тоже нижнетреугольный, j меняется от 0 до i-1
      for (size_t j = 0; j < i; j++)
        {
          double sum = 0;
          // Суммируем произведение элементов строки i исходной матрицы (от 0 до j)
          // на соответствующие элементы уже вычисленного столбца j результата (от 0 до j)
          // Формула: X_ij = - (sum_{k=j}^{i-1} L_ik * X_kj) / L_ii
          // Но так как L_ik = 0 при k > i, а X_kj = 0 при k < j (нижнетреугольность результата),
          // сумма идет от k = j до i - 1.
          
          for (size_t k = j; k < i; k++)
            {
              sum += x_arr[c_n * i + k] * arr[c_n * k + j];
            }
          
          arr[c_n * i + j] = -sum / divider;
        }
    }

  set_m_type (matrix_type::triangular_lower);
  return execution_status::success;
}

block_view &
block_view::operator*= (double val)
{
  size_t size = r_num * c_num;
  double *arr = get_arr ();
  for (size_t i = 0; i < size; i++)
    arr[i] *= val;
  return *this;
}

void
block_view::zero_padding ()
{
  double *arr = get_arr ();
  size_t b_size = get_b_size ();
  size_t size = b_size * b_size;
  for (size_t i = 0; i < size; i++)
    arr[i] = 0.0;
}

void
block_view::sum_abs_columns (double *res_arr)
{
  size_t r_num = get_r_num ();
  size_t c_num = get_c_num ();
  double *arr = get_arr ();
  for (size_t i = 0; i < r_num; i++)
    {
      double *sub_arr = arr + c_num * i;
      for (size_t j = 0; j < c_num; j++)
        res_arr[i] += std::fabs (sub_arr[j]);
    }
}


void
block_view::print_real ()
{
  size_t r_num = get_r_num ();
  size_t c_num = get_c_num ();
  double *arr = get_arr ();
  for (size_t i = 0; i < c_num; i++)
    {
    for (size_t j = 0; j < r_num; j++)
      printf ("%10.3e ", arr[i + c_num * j]);
    printf ("\n");
    }

}
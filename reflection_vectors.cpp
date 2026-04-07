#include "reflection_vectors.h"
#include "block_string.h"
void
reflection_vectors::push (reflection_type rv_type, size_t r_index,
                          size_t c_index)
{
  size_t k = get_k ();
  size_t shift = get_size (rv_type, r_index, c_index);
  shift_plus (shift);
}

size_t
reflection_vectors::get_size (reflection_type rv_type, size_t r_index,
                              size_t c_index)
{
  size_t k = get_k ();

  size_t shift{};
  if (rv_type == reflection_type::triangular)
    {

      if (c_index == k - 1)
        shift = get_ost_triangular_rv_size ();
      else
        shift = get_triangular_rv_size ();
    }
  else
    {
      if (r_index == k - 1)
        shift = get_ost_reset_rv_size ();
      else
        shift = get_reset_rv_size ();
    }
  return shift;
}

void
reflection_vectors::pop (reflection_type rv_type, size_t r_index,
                         size_t c_index)
{
  size_t k = get_k ();
  size_t shift{};
  size_t b_size = get_b_size ();
  size_t r = get_r ();
  size_t vector_size{};
  if (rv_type == reflection_type::triangular)
    {

      if (c_index == k - 1)
        {
          shift = get_ost_triangular_rv_size ();
          vector_size = r;
        }
      else
        {
          shift = get_triangular_rv_size ();
          vector_size = b_size;
        }
    }
  else
    {
      if (r_index == k - 1)
        {
          shift = get_ost_reset_rv_size ();
          vector_size = r + 1;
        }
      else
        {
          shift = get_reset_rv_size ();
          vector_size = b_size + 1;
        }
    }
  set_vector_size (vector_size);
  shift_minus (shift);
}

reflection_vectors &
reflection_vectors::operator= (const reflection_vectors &x)
{
  if (this == &x)
    return *this;
  set_vector_size (x.get_vector_size ());
  size_t stop = x.get_rv_size ();
  set_rv_size (stop);
  double *arr = get_shift_arr (), *x_arr = x.get_arr ();
  for (size_t i = 0; i < stop; i++)
    arr[i] = x_arr[i];
  return *this;
}

execution_status
reflection_vectors::init_reflection_vectors (size_t m_size, size_t b_size,
                                             double eps)
{
  if (get_b_size () != 0)
    {
      printf ("reflection_vectors::memory_allocate:: повторное выделение "
              "памяти\n");
      return execution_status::runtime_error;
    }
  init_m_sizes (m_size, b_size);
  set_eps (eps);
  return execution_status::success;
}

void
reflection_vectors::print (size_t size)
{
  double *arr = get_shift_arr ();
  printf ("v_size = %ld\n", get_vector_size ());
  printf ("rv_print______________________\n");
  for (size_t i = 0; i < size; i++)
    {
      printf (" %lf", arr[i]);
    }
  printf ("\n");
  printf ("___________________________\n");
}

execution_status
reflection_vectors::build_triangular_reflection (block_view &b)
{
  if (!b.square ())
    {
      printf ("reflection::diagonalization_of_block: block is not square\n");
      return execution_status::runtime_error;
    }
  size_t size = b.get_c_num ();
  set_vector_size (size);
  if (get_vector_size () == 1)
    {
      set_vector_size (0);
      return execution_status::success;
    }
  double eps = get_eps ();
  double *arr = get_shift_arr (), *b_arr = b.get_arr ();
  size_t vec_num = 0;

  double *str0 = b_arr;

  double sum0 = 0.0;
  size_t vec_size = size;
  double *vec = arr;

  for (size_t u = 1; u < vec_size; u++)
    {
      sum0 += str0[u] * str0[u];
      vec[u] = str0[u];
      str0[u] = 0;
    }
  double norm = sum0 + str0[0] * str0[0];
  norm = sqrt (norm);
  vec[0] = str0[0] - norm;
  str0[0] = norm;
  norm = sum0 + vec[0] * vec[0];
  norm = sqrt (norm);
  if (norm < eps)
    for (size_t u = 0; u < vec_size; u++)
      vec[u] = 0.0;
  else
    for (size_t u = 0; u < vec_size; u++)
      vec[u] /= norm;
  str0 += size;
  vec_num++;
  for (size_t row = 1; row < size - 1; row++)
    {
      size_t vec_size = size;
      double *vec = arr;

      double sum0 = 0.0;

      for (size_t u = 0; u < vec_size; u++)
        {
          sum0 += str0[u] * vec[u];
        }
      sum0 *= -2;
      double *sub_str0 = str0;
      for (size_t i = 1; i < vec_num; i++)
        {
          double *sub_vec = vec + vec_size;
          double sub_sum0 = 0.0;

          sub_str0[0] += vec[0] * sum0;

          for (size_t u = 1; u < vec_size; u++)
            {
              sub_str0[u] += vec[u] * sum0;

              sub_sum0 += sub_str0[u] * sub_vec[u - 1];
            }
          sum0 = -2 * sub_sum0;
          vec = sub_vec;
          vec_size--;
          sub_str0++;
        }
      for (size_t u = 0; u < vec_size; u++)
        {
          sub_str0[u] += vec[u] * sum0;
        }
      sub_str0++;
      vec += vec_size;
      vec_size--;

      sum0 = 0.0;
      // if (vec_size == 1)
      // return execution_status::success;

      for (size_t u = 1; u < vec_size; u++)
        {
          sum0 += sub_str0[u] * sub_str0[u];
          vec[u] = sub_str0[u];
          sub_str0[u] = 0;
        }
      double norm = sum0 + sub_str0[0] * sub_str0[0];
      norm = sqrt (norm);
      vec[0] = sub_str0[0] - norm;
      sub_str0[0] = norm;
      norm = sum0 + vec[0] * vec[0];
      norm = sqrt (norm);
      if (norm < eps)
        for (size_t u = 0; u < vec_size; u++)
          vec[u] = 0.0;
      else
        for (size_t u = 0; u < vec_size; u++)
          vec[u] /= norm;

      vec_num++;
      str0 += size;
    }

  vec = arr;
  vec_size = size;
  str0 = b_arr + size * (size - 1);
  sum0 = 0.0;
  for (size_t u = 0; u < vec_size; u++)
    sum0 += str0[u] * vec[u];
  sum0 *= -2;
  for (size_t i = 1; i < vec_num; i++)
    {
      double *sub_vec = vec + vec_size;
      double sub_sum0 = 0.0;
      str0[0] += vec[0] * sum0;

      for (size_t u = 1; u < vec_size; u++)
        {
          str0[u] += vec[u] * sum0;
          sub_sum0 += str0[u] * sub_vec[u - 1];
        }
      sum0 = -2 * sub_sum0;
      vec = sub_vec;
      vec_size--;
      str0++;
    }
  for (size_t u = 0; u < vec_size; u++)
    str0[u] += vec[u] * sum0;
  set_rv_size (get_triangular_rv_size ());
  return execution_status::success;
}
execution_status
reflection_vectors::spread_triangular_reflection_and_ref (block_view &b)
{
  double *bl_arr = b.get_arr ();
  size_t r_n = b.get_r_num ();
  size_t c_n = b.get_c_num ();
  double *arr = get_shift_arr ();

  if (r_n == 1)
    return execution_status::success;

  size_t reflections_count = r_n - 1;
  size_t col = 0;

  // РАЗВЕРНУТО ПО 2
  for (; col + 2 <= c_n; col += 2)
    {
      size_t arr_index = 0;
      size_t vec_size = r_n;

      size_t ind0 = col;
      size_t ind1 = col + 1;

      size_t last_arr_ind = 0;

      double sum0 = 0, sum1 = 0;
      for (size_t i = 0; i < vec_size; i++)
        {
          double buf_arr = arr[arr_index];

          sum0 += bl_arr[ind0] * buf_arr;
          sum1 += bl_arr[ind1] * buf_arr;

          ind0 += c_n;
          ind1 += c_n;

          arr_index++;
        }

      for (size_t i = 1; i < reflections_count; i++)
        {
          vec_size = r_n - i;

          ind0 = c_n * i + col;
          ind1 = c_n * i + col + 1;

          bl_arr[ind0 - c_n] -= 2 * arr[last_arr_ind] * sum0;
          bl_arr[ind1 - c_n] -= 2 * arr[last_arr_ind] * sum1;

          last_arr_ind++;

          double sub_sum0 = 0;
          double sub_sum1 = 0;

          for (size_t j = 0; j < vec_size; j++)
            {
              bl_arr[ind0] -= 2 * arr[last_arr_ind] * sum0;
              bl_arr[ind1] -= 2 * arr[last_arr_ind] * sum1;

              last_arr_ind++;

              double buf_arr = arr[arr_index];

              sub_sum0 += bl_arr[ind0] * buf_arr;
              sub_sum1 += bl_arr[ind1] * buf_arr;

              arr_index++;

              ind0 += c_n;
              ind1 += c_n;
            }

          sum0 = sub_sum0;
          sum1 = sub_sum1;
        }

      ind0 = c_n * (r_n - 2) + col;
      ind1 = c_n * (r_n - 2) + col + 1;

      for (size_t i = 0; i < 2; i++)
        {
          bl_arr[ind0] -= 2 * arr[last_arr_ind] * sum0;
          bl_arr[ind1] -= 2 * arr[last_arr_ind] * sum1;

          ind0 += c_n;
          ind1 += c_n;

          last_arr_ind++;
        }
    }

  // ОСТАТОК 1
  if (col < c_n)
    {
      size_t arr_index = 0;
      size_t vec_size = r_n;
      size_t index = col;

      double sum = 0;

      for (size_t i = 0; i < vec_size; i++)
        {
          sum += bl_arr[index] * arr[arr_index];

          arr_index++;
          index += c_n;
        }

      double sub_sum = 0;
      size_t last_arr_ind = 0;

      for (size_t i = 1; i < reflections_count; i++)
        {
          vec_size = r_n - i;
          index = c_n * i + col;

          bl_arr[index - c_n] -= 2 * arr[last_arr_ind] * sum;
          last_arr_ind++;

          sub_sum = 0;
          for (size_t j = 0; j < vec_size; j++)
            {
              bl_arr[index] -= 2 * arr[last_arr_ind] * sum;
              sub_sum += bl_arr[index] * arr[arr_index];

              index += c_n;
              arr_index++;
              last_arr_ind++;
            }
          sum = sub_sum;
        }

      index = c_n * (r_n - 2) + col;
      for (size_t i = 0; i < 2; i++)
        {
          bl_arr[index] -= 2 * arr[last_arr_ind] * sum;
          index += c_n;
          last_arr_ind++;
        }
    }

  return execution_status::success;
}
execution_status
reflection_vectors::spread_triangular_reflection (block_view &b)
{
  double *b_arr = b.get_arr ();
  size_t r_n = b.get_r_num ();
  size_t c_n = b.get_c_num ();
  double *arr = get_shift_arr ();

  if (c_n == 1)
    return execution_status::success;

  size_t vec_num = c_n - 1;
  size_t row = 0;

  double *str0 = b_arr;
  double *str1 = b_arr + c_n;
  size_t vec_size = c_n;

  for (; row + 2 <= r_n; row += 2)
    {
      double sum0 = 0.0;
      double sum1 = 0.0;
      double *vec = arr;
      double *sub_str0 = str0;
      double *sub_str1 = str1;
      vec_size = c_n;

      for (size_t u = 0; u < vec_size; u++)
        {
          sum0 += sub_str0[u] * vec[u];
          sum1 += sub_str1[u] * vec[u];
        }
      sum0 *= -2;
      sum1 *= -2;
      for (size_t i = 1; i < vec_num; i++)
        {
          double *sub_vec = vec + vec_size;
          double sub_sum0 = 0.0;
          double sub_sum1 = 0.0;

          sub_str0[0] += vec[0] * sum0;
          sub_str1[0] += vec[0] * sum1;
          for (size_t u = 1; u < vec_size; u++)
            {
              sub_str0[u] += vec[u] * sum0;
              sub_str1[u] += vec[u] * sum1;

              sub_sum0 += sub_str0[u] * sub_vec[u - 1];
              sub_sum1 += sub_str1[u] * sub_vec[u - 1];
            }
          vec = sub_vec;
          sum0 = -2 * sub_sum0;
          sum1 = -2 * sub_sum1;
          vec_size--;
          sub_str0++;
          sub_str1++;
        }
      for (size_t u = 0; u < vec_size; u++)
        {
          sub_str0[u] += vec[u] * sum0;
          sub_str1[u] += vec[u] * sum1;
        }
      str0 += 2 * c_n;
      str1 += 2 * c_n;
    }

  if ((r_n % 2) == 0)
    return execution_status::success;

  double sum0 = 0.0;
  double *vec = arr;
  double *sub_str0 = str0;
  vec_size = c_n;

  for (size_t u = 0; u < vec_size; u++)
    sum0 += sub_str0[u] * vec[u];
  sum0 *= -2;

  for (size_t i = 1; i < vec_num; i++)
    {
      double *sub_vec = vec + vec_size;
      double sub_sum0 = 0.0;

      sub_str0[0] += vec[0] * sum0;

      for (size_t u = 1; u < vec_size; u++)
        {
          sub_str0[u] += vec[u] * sum0;
          sub_sum0 += sub_str0[u] * sub_vec[u - 1];
        }
      vec = sub_vec;
      sum0 = -2 * sub_sum0;

      vec_size--;
      sub_str0++;
    }
  for (size_t u = 0; u < vec_size; u++)
    sub_str0[u] += vec[u] * sum0;

  return execution_status::success;
}
execution_status
reflection_vectors::build_reset_reflection_and_ref (block_view &di_b,
                                                    block_view &b)
{
  double eps = get_eps ();
  if (!di_b.square ())
    {
      printf ("reflection::block_reset: diagonal block is not square\n");
      return execution_status::runtime_error;
    }

  size_t r_n = b.get_r_num ();          // строк в поддиагональном блоке
  size_t c_n = b.get_c_num ();          // столбцов в поддиагональном блоке
  size_t di_b_size = di_b.get_r_num (); // размер диагонального блока
  set_rv_size ((r_n + 1) * c_n);

  if (c_n != di_b_size)
    {
      printf ("reflection::block_reset: column count mismatch\n");
      return execution_status::runtime_error;
    }

  if (get_arr_size () < (r_n + 1) * c_n)
    {
      printf ("reflection::block_reset: small size\n");
      return execution_status::runtime_error;
    }

  set_vector_size (r_n + 1);
  double *b_arr = b.get_arr ();
  double *di_b_arr = di_b.get_arr ();
  double *arr = get_shift_arr ();
  size_t index = 0;

  double sum = 0;

  // Сохраняем элементы поддиагонального блока и обнуляем их
  for (size_t j = 0; j < r_n; j++)
    {
      double val = b_arr[j * c_n + 0];
      sum += val * val;
      arr[index + j + 1] = val;
      b_arr[j * c_n + 0] = 0; // Обнуляем поддиагональный блок
    }

  double diag_val = di_b_arr[0 * di_b_size + 0];
  double squared_norm = sum + diag_val * diag_val;
  double norm = sqrt (fabs (squared_norm));

  // Строим вектор отражения v
  arr[index] = diag_val - norm;
  di_b_arr[0 * di_b_size + 0] = norm; // Обновляем диагональный элемент

  // Нормализуем вектор отражения
  double v_norm_sq = sum + arr[index] * arr[index];
  double v_norm = sqrt (fabs (v_norm_sq));

  if (fabs (v_norm) < eps)
    {
      for (size_t u = 0; u < r_n + 1; u++)
        arr[index + u] = 0;
    }
  else
    {
      for (size_t u = 0; u < r_n + 1; u++)
        arr[index + u] /= v_norm;
    }

  index += r_n + 1;

  if (c_n == 1)
    return execution_status::success;

  size_t vec_index = 0;
  double dot_product = 0;

  dot_product += di_b_arr[0 * di_b_size + 1] * arr[vec_index];
  for (size_t u = 0; u < r_n; u++)
    dot_product += b_arr[u * c_n + 1] * arr[vec_index + u + 1];

  di_b_arr[0 * di_b_size + 1] -= 2 * dot_product * arr[vec_index];
  for (size_t u = 0; u < r_n; u++)
    b_arr[u * c_n + 1] -= 2 * dot_product * arr[vec_index + u + 1];

  sum = 0;

  for (size_t j = 0; j < r_n; j++)
    {
      double val = b_arr[j * c_n + 1];
      sum += val * val;
      arr[index + j + 1] = val;
      b_arr[j * c_n + 1] = 0;
    }

  diag_val = di_b_arr[1 * di_b_size + 1];
  squared_norm = sum + diag_val * diag_val;
  norm = sqrt (fabs (squared_norm));

  arr[index] = diag_val - norm;
  di_b_arr[1 * di_b_size + 1] = norm;

  v_norm_sq = sum + arr[index] * arr[index];
  v_norm = sqrt (fabs (v_norm_sq));

  if (fabs (v_norm) < eps)
    {
      for (size_t u = 0; u < r_n + 1; u++)
        arr[index + u] = 0;
    }
  else
    {
      for (size_t u = 0; u < r_n + 1; u++)
        arr[index + u] /= v_norm;
    }

  index += r_n + 1;

  for (size_t i = 2; i + 1 < c_n; i += 2)
    {

      vec_index = 0;

      // Вычисляем скаляры для первого вектора отражения (k=0)
      double dot0 = 0, dot1 = 0;
      dot0 += di_b_arr[0 * di_b_size + i] * arr[vec_index];
      dot1 += di_b_arr[0 * di_b_size + i + 1] * arr[vec_index];
      for (size_t u = 0; u < r_n; u++)
        {
          dot0 += b_arr[u * c_n + i] * arr[vec_index + u + 1];
          dot1 += b_arr[u * c_n + i + 1] * arr[vec_index + u + 1];
        }

      // Применяем все отражения к двум столбцам
      for (size_t k = 1; k < i; k++)
        {
          // Применяем предыдущий вектор (k-1)
          di_b_arr[(k - 1) * di_b_size + i] -= 2 * dot0 * arr[vec_index];
          di_b_arr[(k - 1) * di_b_size + i + 1] -= 2 * dot1 * arr[vec_index];
          for (size_t u = 0; u < r_n; u++)
            {
              b_arr[u * c_n + i] -= 2 * dot0 * arr[vec_index + u + 1];
              b_arr[u * c_n + i + 1] -= 2 * dot1 * arr[vec_index + u + 1];
            }

          // Вычисляем скаляры для текущего вектора (k)
          vec_index += r_n + 1;
          dot0 = 0;
          dot1 = 0;
          dot0 += di_b_arr[k * di_b_size + i] * arr[vec_index];
          dot1 += di_b_arr[k * di_b_size + i + 1] * arr[vec_index];
          for (size_t u = 0; u < r_n; u++)
            {
              dot0 += b_arr[u * c_n + i] * arr[vec_index + u + 1];
              dot1 += b_arr[u * c_n + i + 1] * arr[vec_index + u + 1];
            }
        }

      // Применяем последний вектор (i-1)
      di_b_arr[(i - 1) * di_b_size + i] -= 2 * dot0 * arr[vec_index];
      di_b_arr[(i - 1) * di_b_size + i + 1] -= 2 * dot1 * arr[vec_index];
      for (size_t u = 0; u < r_n; u++)
        {
          b_arr[u * c_n + i] -= 2 * dot0 * arr[vec_index + u + 1];
          b_arr[u * c_n + i + 1] -= 2 * dot1 * arr[vec_index + u + 1];
        }

      sum = 0;
      for (size_t j = 0; j < r_n; j++)
        {
          double val = b_arr[j * c_n + i];
          sum += val * val;
          arr[index + j + 1] = val;
          b_arr[j * c_n + i] = 0;
        }

      diag_val = di_b_arr[i * di_b_size + i];
      squared_norm = sum + diag_val * diag_val;
      norm = sqrt (fabs (squared_norm));

      arr[index] = diag_val - norm;
      di_b_arr[i * di_b_size + i] = norm;

      v_norm_sq = sum + arr[index] * arr[index];
      v_norm = sqrt (fabs (v_norm_sq));

      if (fabs (v_norm) < eps)
        {
          for (size_t u = 0; u < r_n + 1; u++)
            arr[index + u] = 0;
        }
      else
        {
          for (size_t u = 0; u < r_n + 1; u++)
            arr[index + u] /= v_norm;
        }

      index += r_n + 1;

      vec_index = index - (r_n + 1); // индекс только что построенного вектора
      dot_product = 0;

      dot_product += di_b_arr[i * di_b_size + i + 1] * arr[vec_index];
      for (size_t u = 0; u < r_n; u++)
        dot_product += b_arr[u * c_n + i + 1] * arr[vec_index + u + 1];

      di_b_arr[i * di_b_size + i + 1] -= 2 * dot_product * arr[vec_index];
      for (size_t u = 0; u < r_n; u++)
        b_arr[u * c_n + i + 1] -= 2 * dot_product * arr[vec_index + u + 1];

      sum = 0;
      for (size_t j = 0; j < r_n; j++)
        {
          double val = b_arr[j * c_n + i + 1];
          sum += val * val;
          arr[index + j + 1] = val;
          b_arr[j * c_n + i + 1] = 0;
        }

      diag_val = di_b_arr[(i + 1) * di_b_size + i + 1];
      squared_norm = sum + diag_val * diag_val;
      norm = sqrt (fabs (squared_norm));

      arr[index] = diag_val - norm;
      di_b_arr[(i + 1) * di_b_size + i + 1] = norm;

      v_norm_sq = sum + arr[index] * arr[index];
      v_norm = sqrt (fabs (v_norm_sq));

      if (fabs (v_norm) < eps)
        {
          for (size_t u = 0; u < r_n + 1; u++)
            arr[index + u] = 0;
        }
      else
        {
          for (size_t u = 0; u < r_n + 1; u++)
            arr[index + u] /= v_norm;
        }

      index += r_n + 1;
    }

  // 3. ОСТАТОК - ЕСЛИ c_n НЕЧЕТНОЕ
  if (c_n % 2 == 1)
    {
      size_t i = c_n - 1;

      // ПРИМЕНЯЕМ ВСЕ ПРЕДЫДУЩИЕ ОТРАЖЕНИЯ К ПОСЛЕДНЕМУ СТОЛБЦУ
      vec_index = 0;
      double dot_product = 0;

      dot_product += di_b_arr[0 * di_b_size + i] * arr[vec_index];
      for (size_t u = 0; u < r_n; u++)
        dot_product += b_arr[u * c_n + i] * arr[vec_index + u + 1];

      for (size_t k = 1; k < i; k++)
        {
          di_b_arr[(k - 1) * di_b_size + i]
              -= 2 * dot_product * arr[vec_index];
          for (size_t u = 0; u < r_n; u++)
            b_arr[u * c_n + i] -= 2 * dot_product * arr[vec_index + u + 1];

          vec_index += r_n + 1;
          dot_product = 0;
          dot_product += di_b_arr[k * di_b_size + i] * arr[vec_index];
          for (size_t u = 0; u < r_n; u++)
            dot_product += b_arr[u * c_n + i] * arr[vec_index + u + 1];
        }

      di_b_arr[(i - 1) * di_b_size + i] -= 2 * dot_product * arr[vec_index];
      for (size_t u = 0; u < r_n; u++)
        b_arr[u * c_n + i] -= 2 * dot_product * arr[vec_index + u + 1];

      // СТРОИМ ВЕКТОР ОТРАЖЕНИЯ ДЛЯ ПОСЛЕДНЕГО СТОЛБЦА
      sum = 0;
      for (size_t j = 0; j < r_n; j++)
        {
          double val = b_arr[j * c_n + i];
          sum += val * val;
          arr[index + j + 1] = val;
          b_arr[j * c_n + i] = 0;
        }

      diag_val = di_b_arr[i * di_b_size + i];
      squared_norm = sum + diag_val * diag_val;
      norm = sqrt (fabs (squared_norm));

      arr[index] = diag_val - norm;
      di_b_arr[i * di_b_size + i] = norm;

      v_norm_sq = sum + arr[index] * arr[index];
      v_norm = sqrt (fabs (v_norm_sq));

      if (fabs (v_norm) < eps)
        {
          for (size_t u = 0; u < r_n + 1; u++)
            arr[index + u] = 0;
        }
      else
        {
          for (size_t u = 0; u < r_n + 1; u++)
            arr[index + u] /= v_norm;
        }
    }

  return execution_status::success;
}

execution_status
reflection_vectors::build_reset_reflection (block_view &di_b, block_view &b)
{
  double eps = get_eps ();
  if (!di_b.square ())
    {
      printf ("reflection::block_reset: diagonal block is not square\n");
      return execution_status::runtime_error;
    }

  size_t c_n = b.get_c_num ();          // строк в поддиагональном блоке
  size_t r_n = b.get_r_num ();          // столбцов в поддиагональном блоке
  size_t di_b_size = di_b.get_r_num (); // размер диагонального блока
  set_rv_size ((c_n + 1) * r_n);
  // b.print ();
  // di_b.print ();
  if (r_n != di_b_size)
    {
      printf ("reflection::block_reset: column count mismatch\n");
      return execution_status::runtime_error;
    }

  if (get_arr_size () < (r_n + 1) * c_n)
    {
      printf ("reflection::block_reset: small size\n");
      return execution_status::runtime_error;
    }
  double *b_arr = b.get_arr ();
  double *di_b_arr = di_b.get_arr ();
  double *arr = get_shift_arr ();
  size_t v_num{};
  set_vector_size (c_n + 1);
  double *str = b_arr;
  double *di_str = di_b_arr;

  double *vec = arr;
  double sum0 = 0.0;
  sum0 = 0.0;
  size_t v_size = get_vector_size ();

  for (size_t u = 0; u < v_size - 1; u++)
    {
      sum0 += str[u] * str[u];
      vec[u + 1] = str[u];
      str[u] = 0;
    }
  // printf ("sum0 = %lf\n", sum0);
  double *di_str_element = di_str + v_num;
  // printf ("diag_val  = %lf\n\n", *di_str_element);
  double norm = sum0 + (*di_str_element) * (*di_str_element);
  norm = sqrt (norm);
  vec[0] = *di_str_element - norm;
  *di_str_element = norm;
  norm = sum0 + vec[0] * vec[0];
  norm = sqrt (norm);

  if (norm < eps)
    for (size_t u = 0; u < v_size; u++)
      vec[u] = 0;
  else
    for (size_t u = 0; u < v_size; u++)
      vec[u] /= norm;
  v_num++;

  str += c_n;
  di_str += r_n;

  for (size_t i = 1; i < r_n; i++)
    {
      double sum0 = 0.0;

      double *vec = arr;

      sum0 += di_str[0] * vec[0];
      for (size_t j = 1; j < v_size; j++)
        sum0 += str[j - 1] * vec[j];

      for (size_t j = 1; j < v_num; j++)
        {
          double *sub_vec = vec + v_size;
          double sub_sum0 = 0.0;

          di_str[j - 1] -= 2 * vec[0] * sum0;
          sub_sum0 += di_str[j] * sub_vec[0];

          for (size_t u = 1; u < v_size; u++)
            {
              str[u - 1] -= 2 * vec[u] * sum0;
              sub_sum0 += str[u - 1] * sub_vec[u];
            }

          vec = sub_vec;
          sum0 = sub_sum0;
        }
      di_str[v_num - 1] -= 2 * vec[0] * sum0;
      for (size_t u = 1; u < v_size; u++)
        str[u - 1] -= 2 * vec[u] * sum0;

      vec += v_size;
      sum0 = 0.0;

      for (size_t u = 0; u < v_size - 1; u++)
        {
          sum0 += str[u] * str[u];
          vec[u + 1] = str[u];
          str[u] = 0;
        }

      double *di_str_element = di_str + v_num;
      double norm = sum0 + (*di_str_element) * (*di_str_element);
      norm = sqrt (norm);
      vec[0] = *di_str_element - norm;
      *di_str_element = norm;
      norm = sum0 + vec[0] * vec[0];
      norm = sqrt (norm);

      if (norm < eps)
        for (size_t u = 0; u < v_size; u++)
          vec[u] = 0;
      else
        for (size_t u = 0; u < v_size; u++)
          vec[u] /= norm;

      v_num++;
      str += c_n;
      di_str += r_n;
    }
  return execution_status::success;
}
execution_status
reflection_vectors::spread_reset_reflection_and_ref (block_view &di_bl,
                                                     block_view &bl)
{
  size_t c_n = bl.get_c_num ();
  size_t vec_size = get_vector_size ();

  double *arr = get_shift_arr (), *bl_arr = bl.get_arr (),
         *di_bl_arr = di_bl.get_arr ();

  size_t reflection_number = get_b_size ();

  // Обрабатываем каждый столбец по отдельности
  for (size_t col = 0; col < c_n; col++)
    {
      double sum = 0;
      size_t arr_index = 0;

      // Вычисляем начальную сумму с диагональным блоком
      sum += di_bl_arr[col] * arr[arr_index];
      arr_index++;

      // Добавляем вклад от поддиагонального блока
      size_t index = col;
      for (size_t i = 1; i < vec_size; i++)
        {
          sum += bl_arr[index] * arr[arr_index];
          index += c_n;
          arr_index++;
        }

      size_t last_arr_ind = 0;

      // Применяем все отражения к текущему столбцу
      for (size_t i = 1; i < reflection_number; i++)
        {
          // Обновляем диагональный блок
          di_bl_arr[c_n * (i - 1) + col] -= 2 * arr[last_arr_ind] * sum;
          last_arr_ind++;

          // Вычисляем новую сумму для следующего отражения
          double sub_sum = 0;
          sub_sum += di_bl_arr[c_n * i + col] * arr[arr_index];
          arr_index++;

          index = col;
          for (size_t j = 1; j < vec_size; j++)
            {
              bl_arr[index] -= 2 * arr[last_arr_ind] * sum;
              sub_sum += bl_arr[index] * arr[arr_index];

              last_arr_ind++;
              arr_index++;
              index += c_n;
            }

          sum = sub_sum;
        }

      // Финальное обновление для последнего отражения
      di_bl_arr[c_n * (reflection_number - 1) + col]
          -= 2 * arr[last_arr_ind] * sum;
      last_arr_ind++;

      index = col;
      for (size_t j = 1; j < vec_size; j++)
        {
          bl_arr[index] -= 2 * arr[last_arr_ind] * sum;
          last_arr_ind++;
          index += c_n;
        }
    }

  return execution_status::success;
}

execution_status
reflection_vectors::spread_reset_reflection (block_view &di_bl, block_view &bl)
{
  size_t r_n = bl.get_r_num ();
  size_t c_n = bl.get_c_num ();

  double *arr = get_shift_arr ();
  double *b_arr = bl.get_arr ();
  double *di_b_arr = di_bl.get_arr ();

  size_t vec_num = get_b_size ();
  size_t row = 0;
  double *str0 = b_arr;
  double *str1 = b_arr + c_n;
  double *di_str0 = di_b_arr;
  double *di_str1 = di_b_arr + vec_num;

  size_t v_size = get_vector_size ();

  for (; row + 2 <= r_n; row += 2)
    {
      double *vec = arr;
      double sum0 = 0.0;
      double sum1 = 0.0;

      sum0 += di_str0[0] * vec[0];
      sum1 += di_str1[0] * vec[0];
      for (size_t u = 1; u < v_size; u++)
        {
          sum0 += str0[u - 1] * vec[u];
          sum1 += str1[u - 1] * vec[u];
        }

      for (size_t i = 1; i < vec_num; i++)
        {
          double *sub_vec = vec + v_size;
          double sub_sum0 = 0.0;
          double sub_sum1 = 0.0;

          sum0 *= -2;
          sum1 *= -2;

          di_str0[i - 1] += vec[0] * sum0;
          di_str1[i - 1] += vec[0] * sum1;

          sub_sum0 += di_str0[i] * sub_vec[0];
          sub_sum1 += di_str1[i] * sub_vec[0];

          for (size_t u = 1; u < v_size; u++)
            {
              str0[u - 1] += vec[u] * sum0;
              str1[u - 1] += vec[u] * sum1;

              sub_sum0 += str0[u - 1] * sub_vec[u];
              sub_sum1 += str1[u - 1] * sub_vec[u];
            }

          vec = sub_vec;
          sum0 = sub_sum0;
          sum1 = sub_sum1;
        }
      di_str0[vec_num - 1] -= 2 * vec[0] * sum0;
      di_str1[vec_num - 1] -= 2 * vec[0] * sum1;
      for (size_t u = 1; u < v_size; u++)
        {
          str0[u - 1] -= 2 * vec[u] * sum0;
          str1[u - 1] -= 2 * vec[u] * sum1;
        }

      str0 += 2 * c_n;
      str1 += 2 * c_n;
      di_str0 += 2 * vec_num;
      di_str1 += 2 * vec_num;
    }
  if (r_n % 2 == 0)
    return execution_status::success;

  double *vec = arr;
  double sum0 = 0.0;

  sum0 += di_str0[0] * vec[0];
  for (size_t u = 1; u < v_size; u++)
    {
      sum0 += str0[u - 1] * vec[u];
    }

  for (size_t i = 1; i < vec_num; i++)
    {
      double *sub_vec = vec + v_size;
      double sub_sum0 = 0.0;
      sum0 *= -2;

      di_str0[i - 1] += vec[0] * sum0;

      sub_sum0 += di_str0[i] * sub_vec[0];

      for (size_t u = 1; u < v_size; u++)
        {
          str0[u - 1] += vec[u] * sum0;

          sub_sum0 += str0[u - 1] * sub_vec[u];
        }

      vec = sub_vec;
      sum0 = sub_sum0;
    }
  di_str0[vec_num - 1] -= 2 * vec[0] * sum0;
  for (size_t u = 1; u < v_size; u++)
    {
      str0[u - 1] -= 2 * vec[u] * sum0;
    }

  return execution_status::success;
}

execution_status
reflection_vectors::spread_transposition_triangular_reflection (block_view &b)
{
  // b.print ();
  double *b_arr = b.get_arr ();
  size_t r_n = b.get_r_num ();
  size_t c_n = b.get_c_num ();
  double *arr = get_shift_arr ();

  if (c_n == 1)
    return execution_status::success;

  size_t vec_num = c_n - 1;
  size_t row = 0;

  double *str0 = b_arr + c_n - 2;
  double *str1 = b_arr + 2 * c_n - 2;
  size_t vec_size = 2;
  size_t len = (c_n + 2) * (c_n - 1) / 2 - 2;
  double *last_vector = arr + len;

  for (; row + 2 <= r_n; row += 2)
    {
      double sum0 = 0.0;
      double sum1 = 0.0;
      double *vec = last_vector;
      double *sub_str0 = str0;
      double *sub_str1 = str1;
      vec_size = 2;

      for (size_t u = 0; u < vec_size; u++)
        {
          sum0 += sub_str0[u] * vec[u];
          sum1 += sub_str1[u] * vec[u];
        }
      sum0 *= -2;
      sum1 *= -2;
      // for (size_t u = 0; u < vec_size; u++)
      //   printf ("%lf ", vec[u]);
      // printf ("\n");
      vec_size++;
      sub_str0--;
      sub_str1--;
      for (size_t i = 1; i < vec_num; i++)
        {
          double *sub_vec = vec - vec_size;
          double sub_sum0 = 0.0;
          double sub_sum1 = 0.0;
          // for (size_t u = 0; u < vec_size; u++)
          //   printf ("%lf ", sub_vec[u]);
          // printf ("\n");
          sub_sum0 += sub_str0[0] * sub_vec[0];
          sub_sum1 += sub_str1[0] * sub_vec[0];
          for (size_t u = 1; u < vec_size; u++)
            {
              sub_str0[u] += vec[u - 1] * sum0;
              sub_str1[u] += vec[u - 1] * sum1;

              sub_sum0 += sub_str0[u] * sub_vec[u];
              sub_sum1 += sub_str1[u] * sub_vec[u];
            }
          vec = sub_vec;
          sum0 = -2 * sub_sum0;
          sum1 = -2 * sub_sum1;
          vec_size++;
          sub_str0--;
          sub_str1--;
        }
      for (size_t u = 1; u < vec_size; u++)
        {
          sub_str0[u] += vec[u - 1] * sum0;
          sub_str1[u] += vec[u - 1] * sum1;
        }
      str0 += 2 * c_n;
      str1 += 2 * c_n;
    }

  if ((r_n % 2) == 0)
    return execution_status::success;

  double sum0 = 0.0;
  double *vec = last_vector;
  double *sub_str0 = str0;
  vec_size = 2;

  for (size_t u = 0; u < vec_size; u++)
    {
      sum0 += sub_str0[u] * vec[u];
    }
  sum0 *= -2;

  vec_size++;
  sub_str0--;

  for (size_t i = 1; i < vec_num; i++)
    {
      double *sub_vec = vec - vec_size;
      double sub_sum0 = 0.0;

      sub_sum0 += sub_str0[0] * sub_vec[0];

      for (size_t u = 1; u < vec_size; u++)
        {
          sub_str0[u] += vec[u - 1] * sum0;

          sub_sum0 += sub_str0[u] * sub_vec[u];
        }
      vec = sub_vec;
      sum0 = -2 * sub_sum0;

      vec_size++;
      sub_str0--;
    }
  for (size_t u = 1; u < vec_size; u++)
    {
      sub_str0[u] += vec[u - 1] * sum0;
    }

  return execution_status::success;
}

execution_status
reflection_vectors::spread_transposition_reset_reflection (block_view &di_bl,
                                                           block_view &bl)
{
  size_t r_n = bl.get_r_num ();
  size_t c_n = bl.get_c_num ();

  set_vector_size (c_n + 1);

  double *arr = get_shift_arr ();
  double *b_arr = bl.get_arr ();
  double *di_b_arr = di_bl.get_arr ();

  size_t vec_num = get_b_size ();
  size_t row = 0;
  double *str0 = b_arr;
  double *str1 = b_arr + c_n;
  double *di_str0 = di_b_arr;
  double *di_str1 = di_b_arr + vec_num;

  size_t v_size = get_vector_size ();

  double *last_vector = arr + v_size * (vec_num - 1);

  for (; row + 2 <= r_n; row += 2)
    {
      double *vec = last_vector;
      double sum0 = 0.0;
      double sum1 = 0.0;

      sum0 += di_str0[vec_num - 1] * vec[0];
      sum1 += di_str1[vec_num - 1] * vec[0];
      for (size_t u = 1; u < v_size; u++)
        {
          sum0 += str0[u - 1] * vec[u];
          sum1 += str1[u - 1] * vec[u];
        }

      for (size_t i = 1; i < vec_num; i++)
        {
          double *sub_vec = vec - v_size;
          double sub_sum0 = 0.0;
          double sub_sum1 = 0.0;

          sum0 *= -2;
          sum1 *= -2;

          di_str0[vec_num - i] += vec[0] * sum0;
          di_str1[vec_num - i] += vec[0] * sum1;

          sub_sum0 += di_str0[vec_num - i - 1] * sub_vec[0];
          sub_sum1 += di_str1[vec_num - i - 1] * sub_vec[0];

          for (size_t u = 1; u < v_size; u++)
            {
              str0[u - 1] += vec[u] * sum0;
              str1[u - 1] += vec[u] * sum1;

              sub_sum0 += str0[u - 1] * sub_vec[u];
              sub_sum1 += str1[u - 1] * sub_vec[u];
            }

          vec = sub_vec;
          sum0 = sub_sum0;
          sum1 = sub_sum1;
        }
      sum0 *= -2;
      sum1 *= -2;
      di_str0[0] += vec[0] * sum0;
      di_str1[0] += vec[0] * sum1;
      for (size_t u = 1; u < v_size; u++)
        {
          str0[u - 1] += vec[u] * sum0;
          str1[u - 1] += vec[u] * sum1;
        }

      str0 += 2 * c_n;
      str1 += 2 * c_n;
      di_str0 += 2 * vec_num;
      di_str1 += 2 * vec_num;
    }
  if (r_n % 2 == 0)
    return execution_status::success;

  double *vec = last_vector;
  double sum0 = 0.0;

  sum0 += di_str0[vec_num - 1] * vec[0];
  for (size_t u = 1; u < v_size; u++)
    {
      sum0 += str0[u - 1] * vec[u];
    }

  for (size_t i = 1; i < vec_num; i++)
    {
      double *sub_vec = vec - v_size;
      double sub_sum0 = 0.0;
      sum0 *= -2;

      di_str0[vec_num - i] += vec[0] * sum0;

      sub_sum0 += di_str0[vec_num - i - 1] * sub_vec[0];

      for (size_t u = 1; u < v_size; u++)
        {
          str0[u - 1] += vec[u] * sum0;

          sub_sum0 += str0[u - 1] * sub_vec[u];
        }

      vec = sub_vec;
      sum0 = sub_sum0;
    }
  sum0 *= -2;
  di_str0[0] += vec[0] * sum0;
  for (size_t u = 1; u < v_size; u++)
    {
      str0[u - 1] += vec[u] * sum0;
    }

  return execution_status::success;
}

void
reflection_vectors::copy_triangular_reflection (reflection_vectors &x)
{
  double *arr = get_shift_arr ();
  double *x_arr = x.get_arr ();
  size_t v_size = x.get_vector_size ();
  set_vector_size (v_size);
  size_t size = (v_size + 2) * (v_size - 1) / 2;
  for (size_t i = 0; i < size; i++)
    arr[i] = x_arr[i];
}

void
reflection_vectors::copy_reset_reflection (reflection_vectors &x)
{
  double *arr = get_shift_arr ();
  double *x_arr = x.get_arr ();
  size_t v_size = x.get_vector_size ();
  set_vector_size (v_size);
  size_t b_size = get_b_size ();
  size_t size = v_size * b_size;
  for (size_t i = 0; i < size; i++)
    arr[i] = x_arr[i];
}

void
reflection_vectors::spread_triangular_reflection (block_string &str,
                                                  block_view *blocks,
                                                  size_t c_start, size_t c_end)
{
  for (size_t c_index = c_start; c_index < c_end; c_index++)
    {
      str.get_block (blocks[0], c_index);
      spread_triangular_reflection (blocks[0]);
    }
}

void
reflection_vectors::spread_reset_reflection (block_string &str,
                                             block_string &sub_str,
                                             block_view *blocks,
                                             size_t c_start, size_t c_end)
{
  for (size_t c_index = c_start; c_index < c_end; c_index++)
    {
      str.get_block (blocks[0], c_index);
      sub_str.get_block (blocks[1], c_index);
      spread_reset_reflection (blocks[0], blocks[1]);
    }
}
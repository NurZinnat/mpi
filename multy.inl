inline execution_status
block_view::block_multiply (block_view &x, block_view &y)
{
  if (x.get_c_num () != y.get_r_num ())
    {
      printf ("bla:не совпдают рамерности\n");
      return execution_status::inval_arg;
    }
  size_t r_n = x.get_r_num ();
  size_t c_n = y.get_c_num ();
  size_t mid = x.get_c_num ();
  size_t x_row_len = x.get_c_num ();
  size_t y_row_len = y.get_c_num ();
  double *x_arr = x.get_arr (), *y_arr = y.get_arr (), *arr = get_arr ();

  // Обнуление результата
  for (size_t i = 0; i < r_n; i++)
    for (size_t j = 0; j < c_n; j++)
      arr[c_n * i + j] = 0.0;

  size_t row_stop = r_n - r_n % 4;
  size_t col_stop = c_n - c_n % 4;
  size_t mid_stop = mid - mid % 4;
  size_t zero = 0;

  size_t *k_start = &zero;
  size_t i = 0;

  if (x.get_m_type () == matrix_type::triangular_upper)
    k_start = &i;

  for (i = 0; i < row_stop; i += 4)
    {
      for (size_t j = 0; j < col_stop; j += 4)
        {
          double r00 = 0, r01 = 0, r02 = 0, r03 = 0;
          double r10 = 0, r11 = 0, r12 = 0, r13 = 0;
          double r20 = 0, r21 = 0, r22 = 0, r23 = 0;
          double r30 = 0, r31 = 0, r32 = 0, r33 = 0;
          // Основные блоки 4x4
          for (size_t k = *k_start; k < mid_stop; k += 4)
            {
              double *x_row0 = x_arr + x_row_len * i + k;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;
              double *x_row3 = x_row2 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                     x03 = x_row0[3];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                     x13 = x_row1[3];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2],
                     x23 = x_row2[3];
              double x30 = x_row3[0], x31 = x_row3[1], x32 = x_row3[2],
                     x33 = x_row3[3];

              double *y_row0 = y_arr + y_row_len * k + j;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;
              double *y_row3 = y_row2 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];
              double y20 = y_row2[0], y21 = y_row2[1], y22 = y_row2[2],
                     y23 = y_row2[3];
              double y30 = y_row3[0], y31 = y_row3[1], y32 = y_row3[2],
                     y33 = y_row3[3];

              r00 += x00 * y00 + x01 * y10 + x02 * y20 + x03 * y30;
              r01 += x00 * y01 + x01 * y11 + x02 * y21 + x03 * y31;
              r02 += x00 * y02 + x01 * y12 + x02 * y22 + x03 * y32;
              r03 += x00 * y03 + x01 * y13 + x02 * y23 + x03 * y33;

              r10 += x10 * y00 + x11 * y10 + x12 * y20 + x13 * y30;
              r11 += x10 * y01 + x11 * y11 + x12 * y21 + x13 * y31;
              r12 += x10 * y02 + x11 * y12 + x12 * y22 + x13 * y32;
              r13 += x10 * y03 + x11 * y13 + x12 * y23 + x13 * y33;

              r20 += x20 * y00 + x21 * y10 + x22 * y20 + x23 * y30;
              r21 += x20 * y01 + x21 * y11 + x22 * y21 + x23 * y31;
              r22 += x20 * y02 + x21 * y12 + x22 * y22 + x23 * y32;
              r23 += x20 * y03 + x21 * y13 + x22 * y23 + x23 * y33;

              r30 += x30 * y00 + x31 * y10 + x32 * y20 + x33 * y30;
              r31 += x30 * y01 + x31 * y11 + x32 * y21 + x33 * y31;
              r32 += x30 * y02 + x31 * y12 + x32 * y22 + x33 * y32;
              r33 += x30 * y03 + x31 * y13 + x32 * y23 + x33 * y33;
            }

          // Хвост по k
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;
              double *x_row3 = x_row2 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2];
              double x30 = x_row3[0], x31 = x_row3[1], x32 = x_row3[2];

              double *y_row0 = y_arr + y_row_len * mid_stop + j;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];
              double y20 = y_row2[0], y21 = y_row2[1], y22 = y_row2[2],
                     y23 = y_row2[3];

              r00 += x00 * y00 + x01 * y10 + x02 * y20;
              r01 += x00 * y01 + x01 * y11 + x02 * y21;
              r02 += x00 * y02 + x01 * y12 + x02 * y22;
              r03 += x00 * y03 + x01 * y13 + x02 * y23;

              r10 += x10 * y00 + x11 * y10 + x12 * y20;
              r11 += x10 * y01 + x11 * y11 + x12 * y21;
              r12 += x10 * y02 + x11 * y12 + x12 * y22;
              r13 += x10 * y03 + x11 * y13 + x12 * y23;

              r20 += x20 * y00 + x21 * y10 + x22 * y20;
              r21 += x20 * y01 + x21 * y11 + x22 * y21;
              r22 += x20 * y02 + x21 * y12 + x22 * y22;
              r23 += x20 * y03 + x21 * y13 + x22 * y23;

              r30 += x30 * y00 + x31 * y10 + x32 * y20;
              r31 += x30 * y01 + x31 * y11 + x32 * y21;
              r32 += x30 * y02 + x31 * y12 + x32 * y22;
              r33 += x30 * y03 + x31 * y13 + x32 * y23;
            }
          else if (k_remain == 2)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;
              double *x_row3 = x_row2 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1];
              double x10 = x_row1[0], x11 = x_row1[1];
              double x20 = x_row2[0], x21 = x_row2[1];
              double x30 = x_row3[0], x31 = x_row3[1];

              double *y_row0 = y_arr + y_row_len * mid_stop + j;
              double *y_row1 = y_row0 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];

              r00 += x00 * y00 + x01 * y10;
              r01 += x00 * y01 + x01 * y11;
              r02 += x00 * y02 + x01 * y12;
              r03 += x00 * y03 + x01 * y13;

              r10 += x10 * y00 + x11 * y10;
              r11 += x10 * y01 + x11 * y11;
              r12 += x10 * y02 + x11 * y12;
              r13 += x10 * y03 + x11 * y13;

              r20 += x20 * y00 + x21 * y10;
              r21 += x20 * y01 + x21 * y11;
              r22 += x20 * y02 + x21 * y12;
              r23 += x20 * y03 + x21 * y13;

              r30 += x30 * y00 + x31 * y10;
              r31 += x30 * y01 + x31 * y11;
              r32 += x30 * y02 + x31 * y12;
              r33 += x30 * y03 + x31 * y13;
            }
          else if (k_remain == 1)
            {
              double x0k = x_arr[x_row_len * i + mid_stop];
              double x1k = x_arr[x_row_len * (i + 1) + mid_stop];
              double x2k = x_arr[x_row_len * (i + 2) + mid_stop];
              double x3k = x_arr[x_row_len * (i + 3) + mid_stop];

              double *y_row = y_arr + y_row_len * mid_stop + j;
              double yk0 = y_row[0], yk1 = y_row[1], yk2 = y_row[2],
                     yk3 = y_row[3];

              r00 += x0k * yk0;
              r01 += x0k * yk1;
              r02 += x0k * yk2;
              r03 += x0k * yk3;

              r10 += x1k * yk0;
              r11 += x1k * yk1;
              r12 += x1k * yk2;
              r13 += x1k * yk3;

              r20 += x2k * yk0;
              r21 += x2k * yk1;
              r22 += x2k * yk2;
              r23 += x2k * yk3;

              r30 += x3k * yk0;
              r31 += x3k * yk1;
              r32 += x3k * yk2;
              r33 += x3k * yk3;
            }

          // Сохранение результатов
          double *res_row = arr + c_n * i + j;
          res_row[0] = r00;
          res_row[1] = r01;
          res_row[2] = r02;
          res_row[3] = r03;

          res_row += c_n;
          res_row[0] = r10;
          res_row[1] = r11;
          res_row[2] = r12;
          res_row[3] = r13;

          res_row += c_n;
          res_row[0] = r20;
          res_row[1] = r21;
          res_row[2] = r22;
          res_row[3] = r23;

          res_row += c_n;
          res_row[0] = r30;
          res_row[1] = r31;
          res_row[2] = r32;
          res_row[3] = r33;
        }

      // Остаточные столбцы для блочных строк
      size_t j_remain = c_n - col_stop;

      if (j_remain == 3)
        {
          for (size_t j = col_stop; j < c_n; j++)
            {
              double r0 = 0, r1 = 0, r2 = 0, r3 = 0;

              // Основные блоки по k
              for (size_t k = *k_start; k < mid_stop; k += 4)
                {
                  double *x_row0 = x_arr + x_row_len * i + k;
                  double *x_row1 = x_row0 + x_row_len;
                  double *x_row2 = x_row1 + x_row_len;
                  double *x_row3 = x_row2 + x_row_len;

                  double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                         x03 = x_row0[3];
                  double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                         x13 = x_row1[3];
                  double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2],
                         x23 = x_row2[3];
                  double x30 = x_row3[0], x31 = x_row3[1], x32 = x_row3[2],
                         x33 = x_row3[3];

                  double *y_col = y_arr + y_row_len * k + j;
                  double y0 = y_col[0];
                  double y1 = y_col[y_row_len];
                  double y2 = y_col[2 * y_row_len];
                  double y3 = y_col[3 * y_row_len];

                  r0 += x00 * y0 + x01 * y1 + x02 * y2 + x03 * y3;
                  r1 += x10 * y0 + x11 * y1 + x12 * y2 + x13 * y3;
                  r2 += x20 * y0 + x21 * y1 + x22 * y2 + x23 * y3;
                  r3 += x30 * y0 + x31 * y1 + x32 * y2 + x33 * y3;
                }

              // Хвост по k для j
              size_t k_remain = mid - mid_stop;
              if (k_remain == 3)
                {
                  double *x_row0 = x_arr + x_row_len * i + mid_stop;
                  double *x_row1 = x_row0 + x_row_len;
                  double *x_row2 = x_row1 + x_row_len;
                  double *x_row3 = x_row2 + x_row_len;

                  double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
                  double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];
                  double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2];
                  double x30 = x_row3[0], x31 = x_row3[1], x32 = x_row3[2];

                  double *y_col0 = y_arr + y_row_len * mid_stop + j;
                  double *y_col1 = y_col0 + y_row_len;
                  double *y_col2 = y_col1 + y_row_len;

                  double y0 = y_col0[0];
                  double y1 = y_col1[0];
                  double y2 = y_col2[0];

                  r0 += x00 * y0 + x01 * y1 + x02 * y2;
                  r1 += x10 * y0 + x11 * y1 + x12 * y2;
                  r2 += x20 * y0 + x21 * y1 + x22 * y2;
                  r3 += x30 * y0 + x31 * y1 + x32 * y2;
                }
              else if (k_remain == 2)
                {
                  double *x_row0 = x_arr + x_row_len * i + mid_stop;
                  double *x_row1 = x_row0 + x_row_len;
                  double *x_row2 = x_row1 + x_row_len;
                  double *x_row3 = x_row2 + x_row_len;

                  double x00 = x_row0[0], x01 = x_row0[1];
                  double x10 = x_row1[0], x11 = x_row1[1];
                  double x20 = x_row2[0], x21 = x_row2[1];
                  double x30 = x_row3[0], x31 = x_row3[1];

                  double *y_col0 = y_arr + y_row_len * mid_stop + j;
                  double *y_col1 = y_col0 + y_row_len;

                  double y0 = y_col0[0];
                  double y1 = y_col1[0];

                  r0 += x00 * y0 + x01 * y1;
                  r1 += x10 * y0 + x11 * y1;
                  r2 += x20 * y0 + x21 * y1;
                  r3 += x30 * y0 + x31 * y1;
                }
              else if (k_remain == 1)
                {
                  double x0k = x_arr[x_row_len * i + mid_stop];
                  double x1k = x_arr[x_row_len * (i + 1) + mid_stop];
                  double x2k = x_arr[x_row_len * (i + 2) + mid_stop];
                  double x3k = x_arr[x_row_len * (i + 3) + mid_stop];

                  double ykj = y_arr[y_row_len * mid_stop + j];

                  r0 += x0k * ykj;
                  r1 += x1k * ykj;
                  r2 += x2k * ykj;
                  r3 += x3k * ykj;
                }

              arr[c_n * i + j] = r0;
              arr[c_n * (i + 1) + j] = r1;
              arr[c_n * (i + 2) + j] = r2;
              arr[c_n * (i + 3) + j] = r3;
            }
        }
      else if (j_remain == 2)
        {
          // Обрабатываем сразу 2 столбца
          double r00 = 0, r01 = 0;
          double r10 = 0, r11 = 0;
          double r20 = 0, r21 = 0;
          double r30 = 0, r31 = 0;

          for (size_t k = *k_start; k < mid_stop; k += 4)
            {
              double *x_row0 = x_arr + x_row_len * i + k;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;
              double *x_row3 = x_row2 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                     x03 = x_row0[3];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                     x13 = x_row1[3];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2],
                     x23 = x_row2[3];
              double x30 = x_row3[0], x31 = x_row3[1], x32 = x_row3[2],
                     x33 = x_row3[3];

              double *y_col0 = y_arr + y_row_len * k + col_stop;
              double *y_col1 = y_col0 + 1;

              double y00 = y_col0[0], y01 = y_col1[0];
              double y10 = y_col0[y_row_len], y11 = y_col1[y_row_len];
              double y20 = y_col0[2 * y_row_len], y21 = y_col1[2 * y_row_len];
              double y30 = y_col0[3 * y_row_len], y31 = y_col1[3 * y_row_len];

              r00 += x00 * y00 + x01 * y10 + x02 * y20 + x03 * y30;
              r01 += x00 * y01 + x01 * y11 + x02 * y21 + x03 * y31;

              r10 += x10 * y00 + x11 * y10 + x12 * y20 + x13 * y30;
              r11 += x10 * y01 + x11 * y11 + x12 * y21 + x13 * y31;

              r20 += x20 * y00 + x21 * y10 + x22 * y20 + x23 * y30;
              r21 += x20 * y01 + x21 * y11 + x22 * y21 + x23 * y31;

              r30 += x30 * y00 + x31 * y10 + x32 * y20 + x33 * y30;
              r31 += x30 * y01 + x31 * y11 + x32 * y21 + x33 * y31;
            }

          // Хвост по k для 2 столбцов
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;
              double *x_row3 = x_row2 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2];
              double x30 = x_row3[0], x31 = x_row3[1], x32 = x_row3[2];

              double *y_row0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1];
              double y10 = y_row1[0], y11 = y_row1[1];
              double y20 = y_row2[0], y21 = y_row2[1];

              r00 += x00 * y00 + x01 * y10 + x02 * y20;
              r01 += x00 * y01 + x01 * y11 + x02 * y21;

              r10 += x10 * y00 + x11 * y10 + x12 * y20;
              r11 += x10 * y01 + x11 * y11 + x12 * y21;

              r20 += x20 * y00 + x21 * y10 + x22 * y20;
              r21 += x20 * y01 + x21 * y11 + x22 * y21;

              r30 += x30 * y00 + x31 * y10 + x32 * y20;
              r31 += x30 * y01 + x31 * y11 + x32 * y21;
            }
          else if (k_remain == 2)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;
              double *x_row3 = x_row2 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1];
              double x10 = x_row1[0], x11 = x_row1[1];
              double x20 = x_row2[0], x21 = x_row2[1];
              double x30 = x_row3[0], x31 = x_row3[1];

              double *y_row0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_row1 = y_row0 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1];
              double y10 = y_row1[0], y11 = y_row1[1];

              r00 += x00 * y00 + x01 * y10;
              r01 += x00 * y01 + x01 * y11;

              r10 += x10 * y00 + x11 * y10;
              r11 += x10 * y01 + x11 * y11;

              r20 += x20 * y00 + x21 * y10;
              r21 += x20 * y01 + x21 * y11;

              r30 += x30 * y00 + x31 * y10;
              r31 += x30 * y01 + x31 * y11;
            }
          else if (k_remain == 1)
            {
              double x0k = x_arr[x_row_len * i + mid_stop];
              double x1k = x_arr[x_row_len * (i + 1) + mid_stop];
              double x2k = x_arr[x_row_len * (i + 2) + mid_stop];
              double x3k = x_arr[x_row_len * (i + 3) + mid_stop];

              double *y_row = y_arr + y_row_len * mid_stop + col_stop;
              double yk0 = y_row[0], yk1 = y_row[1];

              r00 += x0k * yk0;
              r01 += x0k * yk1;
              r10 += x1k * yk0;
              r11 += x1k * yk1;
              r20 += x2k * yk0;
              r21 += x2k * yk1;
              r30 += x3k * yk0;
              r31 += x3k * yk1;
            }

          arr[c_n * i + col_stop] = r00;
          arr[c_n * i + col_stop + 1] = r01;
          arr[c_n * (i + 1) + col_stop] = r10;
          arr[c_n * (i + 1) + col_stop + 1] = r11;
          arr[c_n * (i + 2) + col_stop] = r20;
          arr[c_n * (i + 2) + col_stop + 1] = r21;
          arr[c_n * (i + 3) + col_stop] = r30;
          arr[c_n * (i + 3) + col_stop + 1] = r31;
        }
      else if (j_remain == 1)
        {
          double r0 = 0, r1 = 0, r2 = 0, r3 = 0;

          for (size_t k = *k_start; k < mid_stop; k += 4)
            {
              double *x_row0 = x_arr + x_row_len * i + k;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;
              double *x_row3 = x_row2 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                     x03 = x_row0[3];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                     x13 = x_row1[3];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2],
                     x23 = x_row2[3];
              double x30 = x_row3[0], x31 = x_row3[1], x32 = x_row3[2],
                     x33 = x_row3[3];

              double *y_col = y_arr + y_row_len * k + col_stop;
              double y0 = y_col[0];
              double y1 = y_col[y_row_len];
              double y2 = y_col[2 * y_row_len];
              double y3 = y_col[3 * y_row_len];

              r0 += x00 * y0 + x01 * y1 + x02 * y2 + x03 * y3;
              r1 += x10 * y0 + x11 * y1 + x12 * y2 + x13 * y3;
              r2 += x20 * y0 + x21 * y1 + x22 * y2 + x23 * y3;
              r3 += x30 * y0 + x31 * y1 + x32 * y2 + x33 * y3;
            }

          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;
              double *x_row3 = x_row2 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2];
              double x30 = x_row3[0], x31 = x_row3[1], x32 = x_row3[2];

              double *y_col0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_col1 = y_col0 + y_row_len;
              double *y_col2 = y_col1 + y_row_len;

              double y0 = y_col0[0];
              double y1 = y_col1[0];
              double y2 = y_col2[0];

              r0 += x00 * y0 + x01 * y1 + x02 * y2;
              r1 += x10 * y0 + x11 * y1 + x12 * y2;
              r2 += x20 * y0 + x21 * y1 + x22 * y2;
              r3 += x30 * y0 + x31 * y1 + x32 * y2;
            }
          else if (k_remain == 2)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;
              double *x_row3 = x_row2 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1];
              double x10 = x_row1[0], x11 = x_row1[1];
              double x20 = x_row2[0], x21 = x_row2[1];
              double x30 = x_row3[0], x31 = x_row3[1];

              double *y_col0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_col1 = y_col0 + y_row_len;

              double y0 = y_col0[0];
              double y1 = y_col1[0];

              r0 += x00 * y0 + x01 * y1;
              r1 += x10 * y0 + x11 * y1;
              r2 += x20 * y0 + x21 * y1;
              r3 += x30 * y0 + x31 * y1;
            }
          else if (k_remain == 1)
            {
              double x0k = x_arr[x_row_len * i + mid_stop];
              double x1k = x_arr[x_row_len * (i + 1) + mid_stop];
              double x2k = x_arr[x_row_len * (i + 2) + mid_stop];
              double x3k = x_arr[x_row_len * (i + 3) + mid_stop];

              double ykj = y_arr[y_row_len * mid_stop + col_stop];

              r0 += x0k * ykj;
              r1 += x1k * ykj;
              r2 += x2k * ykj;
              r3 += x3k * ykj;
            }

          arr[c_n * i + col_stop] = r0;
          arr[c_n * (i + 1) + col_stop] = r1;
          arr[c_n * (i + 2) + col_stop] = r2;
          arr[c_n * (i + 3) + col_stop] = r3;
        }
    }

  // Остаточные строки
  size_t i_remain = r_n - row_stop;

  if (i_remain == 3)
    {
      size_t i = row_stop;

      // Основные блоки по j
      for (size_t j = 0; j < col_stop; j += 4)
        {
          double r00 = 0, r01 = 0, r02 = 0, r03 = 0;
          double r10 = 0, r11 = 0, r12 = 0, r13 = 0;
          double r20 = 0, r21 = 0, r22 = 0, r23 = 0;

          // Основные блоки по k
          for (size_t k = *k_start; k < mid_stop; k += 4)
            {
              double *x_row0 = x_arr + x_row_len * i + k;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                     x03 = x_row0[3];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                     x13 = x_row1[3];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2],
                     x23 = x_row2[3];

              double *y_row0 = y_arr + y_row_len * k + j;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;
              double *y_row3 = y_row2 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];
              double y20 = y_row2[0], y21 = y_row2[1], y22 = y_row2[2],
                     y23 = y_row2[3];
              double y30 = y_row3[0], y31 = y_row3[1], y32 = y_row3[2],
                     y33 = y_row3[3];

              r00 += x00 * y00 + x01 * y10 + x02 * y20 + x03 * y30;
              r01 += x00 * y01 + x01 * y11 + x02 * y21 + x03 * y31;
              r02 += x00 * y02 + x01 * y12 + x02 * y22 + x03 * y32;
              r03 += x00 * y03 + x01 * y13 + x02 * y23 + x03 * y33;

              r10 += x10 * y00 + x11 * y10 + x12 * y20 + x13 * y30;
              r11 += x10 * y01 + x11 * y11 + x12 * y21 + x13 * y31;
              r12 += x10 * y02 + x11 * y12 + x12 * y22 + x13 * y32;
              r13 += x10 * y03 + x11 * y13 + x12 * y23 + x13 * y33;

              r20 += x20 * y00 + x21 * y10 + x22 * y20 + x23 * y30;
              r21 += x20 * y01 + x21 * y11 + x22 * y21 + x23 * y31;
              r22 += x20 * y02 + x21 * y12 + x22 * y22 + x23 * y32;
              r23 += x20 * y03 + x21 * y13 + x22 * y23 + x23 * y33;
            }

          // Хвост по k для 3 строк
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2];

              double *y_row0 = y_arr + y_row_len * mid_stop + j;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];
              double y20 = y_row2[0], y21 = y_row2[1], y22 = y_row2[2],
                     y23 = y_row2[3];

              r00 += x00 * y00 + x01 * y10 + x02 * y20;
              r01 += x00 * y01 + x01 * y11 + x02 * y21;
              r02 += x00 * y02 + x01 * y12 + x02 * y22;
              r03 += x00 * y03 + x01 * y13 + x02 * y23;

              r10 += x10 * y00 + x11 * y10 + x12 * y20;
              r11 += x10 * y01 + x11 * y11 + x12 * y21;
              r12 += x10 * y02 + x11 * y12 + x12 * y22;
              r13 += x10 * y03 + x11 * y13 + x12 * y23;

              r20 += x20 * y00 + x21 * y10 + x22 * y20;
              r21 += x20 * y01 + x21 * y11 + x22 * y21;
              r22 += x20 * y02 + x21 * y12 + x22 * y22;
              r23 += x20 * y03 + x21 * y13 + x22 * y23;
            }
          else if (k_remain == 2)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1];
              double x10 = x_row1[0], x11 = x_row1[1];
              double x20 = x_row2[0], x21 = x_row2[1];

              double *y_row0 = y_arr + y_row_len * mid_stop + j;
              double *y_row1 = y_row0 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];

              r00 += x00 * y00 + x01 * y10;
              r01 += x00 * y01 + x01 * y11;
              r02 += x00 * y02 + x01 * y12;
              r03 += x00 * y03 + x01 * y13;

              r10 += x10 * y00 + x11 * y10;
              r11 += x10 * y01 + x11 * y11;
              r12 += x10 * y02 + x11 * y12;
              r13 += x10 * y03 + x11 * y13;

              r20 += x20 * y00 + x21 * y10;
              r21 += x20 * y01 + x21 * y11;
              r22 += x20 * y02 + x21 * y12;
              r23 += x20 * y03 + x21 * y13;
            }
          else if (k_remain == 1)
            {
              double x0k = x_arr[x_row_len * i + mid_stop];
              double x1k = x_arr[x_row_len * (i + 1) + mid_stop];
              double x2k = x_arr[x_row_len * (i + 2) + mid_stop];

              double *y_row = y_arr + y_row_len * mid_stop + j;
              double yk0 = y_row[0], yk1 = y_row[1], yk2 = y_row[2],
                     yk3 = y_row[3];

              r00 += x0k * yk0;
              r01 += x0k * yk1;
              r02 += x0k * yk2;
              r03 += x0k * yk3;
              r10 += x1k * yk0;
              r11 += x1k * yk1;
              r12 += x1k * yk2;
              r13 += x1k * yk3;
              r20 += x2k * yk0;
              r21 += x2k * yk1;
              r22 += x2k * yk2;
              r23 += x2k * yk3;
            }

          // Сохранение результатов
          double *res_row = arr + c_n * i + j;
          res_row[0] = r00;
          res_row[1] = r01;
          res_row[2] = r02;
          res_row[3] = r03;
          res_row += c_n;
          res_row[0] = r10;
          res_row[1] = r11;
          res_row[2] = r12;
          res_row[3] = r13;
          res_row += c_n;
          res_row[0] = r20;
          res_row[1] = r21;
          res_row[2] = r22;
          res_row[3] = r23;
        }

      // Остаточные столбцы для 3 строк
      size_t j_remain = c_n - col_stop;

      if (j_remain == 3)
        {
          for (size_t j = col_stop; j < c_n; j++)
            {
              double r0 = 0, r1 = 0, r2 = 0;

              for (size_t k = *k_start; k < mid_stop; k += 4)
                {
                  double *x_row0 = x_arr + x_row_len * i + k;
                  double *x_row1 = x_row0 + x_row_len;
                  double *x_row2 = x_row1 + x_row_len;

                  double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                         x03 = x_row0[3];
                  double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                         x13 = x_row1[3];
                  double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2],
                         x23 = x_row2[3];

                  double *y_col = y_arr + y_row_len * k + j;
                  double y0 = y_col[0];
                  double y1 = y_col[y_row_len];
                  double y2 = y_col[2 * y_row_len];
                  double y3 = y_col[3 * y_row_len];

                  r0 += x00 * y0 + x01 * y1 + x02 * y2 + x03 * y3;
                  r1 += x10 * y0 + x11 * y1 + x12 * y2 + x13 * y3;
                  r2 += x20 * y0 + x21 * y1 + x22 * y2 + x23 * y3;
                }

              // Хвост по k для остаточных столбцов
              size_t k_remain = mid - mid_stop;

              if (k_remain == 3)
                {
                  double *x_row0 = x_arr + x_row_len * i + mid_stop;
                  double *x_row1 = x_row0 + x_row_len;
                  double *x_row2 = x_row1 + x_row_len;

                  double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
                  double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];
                  double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2];

                  double *y_col0 = y_arr + y_row_len * mid_stop + j;
                  double *y_col1 = y_col0 + y_row_len;
                  double *y_col2 = y_col1 + y_row_len;

                  double y0 = y_col0[0];
                  double y1 = y_col1[0];
                  double y2 = y_col2[0];

                  r0 += x00 * y0 + x01 * y1 + x02 * y2;
                  r1 += x10 * y0 + x11 * y1 + x12 * y2;
                  r2 += x20 * y0 + x21 * y1 + x22 * y2;
                }
              else if (k_remain == 2)
                {
                  double *x_row0 = x_arr + x_row_len * i + mid_stop;
                  double *x_row1 = x_row0 + x_row_len;
                  double *x_row2 = x_row1 + x_row_len;

                  double x00 = x_row0[0], x01 = x_row0[1];
                  double x10 = x_row1[0], x11 = x_row1[1];
                  double x20 = x_row2[0], x21 = x_row2[1];

                  double *y_col0 = y_arr + y_row_len * mid_stop + j;
                  double *y_col1 = y_col0 + y_row_len;

                  double y0 = y_col0[0];
                  double y1 = y_col1[0];

                  r0 += x00 * y0 + x01 * y1;
                  r1 += x10 * y0 + x11 * y1;
                  r2 += x20 * y0 + x21 * y1;
                }
              else if (k_remain == 1)
                {
                  double x0k = x_arr[x_row_len * i + mid_stop];
                  double x1k = x_arr[x_row_len * (i + 1) + mid_stop];
                  double x2k = x_arr[x_row_len * (i + 2) + mid_stop];

                  double ykj = y_arr[y_row_len * mid_stop + j];

                  r0 += x0k * ykj;
                  r1 += x1k * ykj;
                  r2 += x2k * ykj;
                }

              arr[c_n * i + j] = r0;
              arr[c_n * (i + 1) + j] = r1;
              arr[c_n * (i + 2) + j] = r2;
            }
        }
      else if (j_remain == 2)
        {
          // Обрабатываем сразу 2 столбца для 3 строк
          double r00 = 0, r01 = 0;
          double r10 = 0, r11 = 0;
          double r20 = 0, r21 = 0;

          for (size_t k = *k_start; k < mid_stop; k += 4)
            {
              double *x_row0 = x_arr + x_row_len * i + k;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                     x03 = x_row0[3];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                     x13 = x_row1[3];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2],
                     x23 = x_row2[3];

              double *y_col0 = y_arr + y_row_len * k + col_stop;
              double *y_col1 = y_col0 + 1;

              double y00 = y_col0[0], y01 = y_col1[0];
              double y10 = y_col0[y_row_len], y11 = y_col1[y_row_len];
              double y20 = y_col0[2 * y_row_len], y21 = y_col1[2 * y_row_len];
              double y30 = y_col0[3 * y_row_len], y31 = y_col1[3 * y_row_len];

              r00 += x00 * y00 + x01 * y10 + x02 * y20 + x03 * y30;
              r01 += x00 * y01 + x01 * y11 + x02 * y21 + x03 * y31;

              r10 += x10 * y00 + x11 * y10 + x12 * y20 + x13 * y30;
              r11 += x10 * y01 + x11 * y11 + x12 * y21 + x13 * y31;

              r20 += x20 * y00 + x21 * y10 + x22 * y20 + x23 * y30;
              r21 += x20 * y01 + x21 * y11 + x22 * y21 + x23 * y31;
            }

          // Хвост по k для 2 столбцов и 3 строк
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2];

              double *y_row0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1];
              double y10 = y_row1[0], y11 = y_row1[1];
              double y20 = y_row2[0], y21 = y_row2[1];

              r00 += x00 * y00 + x01 * y10 + x02 * y20;
              r01 += x00 * y01 + x01 * y11 + x02 * y21;

              r10 += x10 * y00 + x11 * y10 + x12 * y20;
              r11 += x10 * y01 + x11 * y11 + x12 * y21;

              r20 += x20 * y00 + x21 * y10 + x22 * y20;
              r21 += x20 * y01 + x21 * y11 + x22 * y21;
            }
          else if (k_remain == 2)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1];
              double x10 = x_row1[0], x11 = x_row1[1];
              double x20 = x_row2[0], x21 = x_row2[1];

              double *y_row0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_row1 = y_row0 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1];
              double y10 = y_row1[0], y11 = y_row1[1];

              r00 += x00 * y00 + x01 * y10;
              r01 += x00 * y01 + x01 * y11;

              r10 += x10 * y00 + x11 * y10;
              r11 += x10 * y01 + x11 * y11;

              r20 += x20 * y00 + x21 * y10;
              r21 += x20 * y01 + x21 * y11;
            }
          else if (k_remain == 1)
            {
              double x0k = x_arr[x_row_len * i + mid_stop];
              double x1k = x_arr[x_row_len * (i + 1) + mid_stop];
              double x2k = x_arr[x_row_len * (i + 2) + mid_stop];

              double *y_row = y_arr + y_row_len * mid_stop + col_stop;
              double yk0 = y_row[0], yk1 = y_row[1];

              r00 += x0k * yk0;
              r01 += x0k * yk1;
              r10 += x1k * yk0;
              r11 += x1k * yk1;
              r20 += x2k * yk0;
              r21 += x2k * yk1;
            }

          arr[c_n * i + col_stop] = r00;
          arr[c_n * i + col_stop + 1] = r01;
          arr[c_n * (i + 1) + col_stop] = r10;
          arr[c_n * (i + 1) + col_stop + 1] = r11;
          arr[c_n * (i + 2) + col_stop] = r20;
          arr[c_n * (i + 2) + col_stop + 1] = r21;
        }
      else if (j_remain == 1)
        {
          double r0 = 0, r1 = 0, r2 = 0;

          for (size_t k = *k_start; k < mid_stop; k += 4)
            {
              double *x_row0 = x_arr + x_row_len * i + k;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                     x03 = x_row0[3];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                     x13 = x_row1[3];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2],
                     x23 = x_row2[3];

              double *y_col = y_arr + y_row_len * k + col_stop;
              double y0 = y_col[0];
              double y1 = y_col[y_row_len];
              double y2 = y_col[2 * y_row_len];
              double y3 = y_col[3 * y_row_len];

              r0 += x00 * y0 + x01 * y1 + x02 * y2 + x03 * y3;
              r1 += x10 * y0 + x11 * y1 + x12 * y2 + x13 * y3;
              r2 += x20 * y0 + x21 * y1 + x22 * y2 + x23 * y3;
            }

          // Хвост по k для 1 столбца и 3 строк
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];
              double x20 = x_row2[0], x21 = x_row2[1], x22 = x_row2[2];

              double *y_col0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_col1 = y_col0 + y_row_len;
              double *y_col2 = y_col1 + y_row_len;

              double y0 = y_col0[0];
              double y1 = y_col1[0];
              double y2 = y_col2[0];

              r0 += x00 * y0 + x01 * y1 + x02 * y2;
              r1 += x10 * y0 + x11 * y1 + x12 * y2;
              r2 += x20 * y0 + x21 * y1 + x22 * y2;
            }
          else if (k_remain == 2)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;
              double *x_row2 = x_row1 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1];
              double x10 = x_row1[0], x11 = x_row1[1];
              double x20 = x_row2[0], x21 = x_row2[1];

              double *y_col0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_col1 = y_col0 + y_row_len;

              double y0 = y_col0[0];
              double y1 = y_col1[0];

              r0 += x00 * y0 + x01 * y1;
              r1 += x10 * y0 + x11 * y1;
              r2 += x20 * y0 + x21 * y1;
            }
          else if (k_remain == 1)
            {
              double x0k = x_arr[x_row_len * i + mid_stop];
              double x1k = x_arr[x_row_len * (i + 1) + mid_stop];
              double x2k = x_arr[x_row_len * (i + 2) + mid_stop];

              double ykj = y_arr[y_row_len * mid_stop + col_stop];

              r0 += x0k * ykj;
              r1 += x1k * ykj;
              r2 += x2k * ykj;
            }

          arr[c_n * i + col_stop] = r0;
          arr[c_n * (i + 1) + col_stop] = r1;
          arr[c_n * (i + 2) + col_stop] = r2;
        }
    }
  else if (i_remain == 2)
    {
      size_t i = row_stop;

      // Основные блоки по j
      for (size_t j = 0; j < col_stop; j += 4)
        {
          double r00 = 0, r01 = 0, r02 = 0, r03 = 0;
          double r10 = 0, r11 = 0, r12 = 0, r13 = 0;

          // Основные блоки по k
          for (size_t k = *k_start; k < mid_stop; k += 4)
            {
              double *x_row0 = x_arr + x_row_len * i + k;
              double *x_row1 = x_row0 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                     x03 = x_row0[3];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                     x13 = x_row1[3];

              double *y_row0 = y_arr + y_row_len * k + j;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;
              double *y_row3 = y_row2 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];
              double y20 = y_row2[0], y21 = y_row2[1], y22 = y_row2[2],
                     y23 = y_row2[3];
              double y30 = y_row3[0], y31 = y_row3[1], y32 = y_row3[2],
                     y33 = y_row3[3];

              r00 += x00 * y00 + x01 * y10 + x02 * y20 + x03 * y30;
              r01 += x00 * y01 + x01 * y11 + x02 * y21 + x03 * y31;
              r02 += x00 * y02 + x01 * y12 + x02 * y22 + x03 * y32;
              r03 += x00 * y03 + x01 * y13 + x02 * y23 + x03 * y33;

              r10 += x10 * y00 + x11 * y10 + x12 * y20 + x13 * y30;
              r11 += x10 * y01 + x11 * y11 + x12 * y21 + x13 * y31;
              r12 += x10 * y02 + x11 * y12 + x12 * y22 + x13 * y32;
              r13 += x10 * y03 + x11 * y13 + x12 * y23 + x13 * y33;
            }

          // Хвост по k для 2 строк
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];

              double *y_row0 = y_arr + y_row_len * mid_stop + j;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];
              double y20 = y_row2[0], y21 = y_row2[1], y22 = y_row2[2],
                     y23 = y_row2[3];

              r00 += x00 * y00 + x01 * y10 + x02 * y20;
              r01 += x00 * y01 + x01 * y11 + x02 * y21;
              r02 += x00 * y02 + x01 * y12 + x02 * y22;
              r03 += x00 * y03 + x01 * y13 + x02 * y23;

              r10 += x10 * y00 + x11 * y10 + x12 * y20;
              r11 += x10 * y01 + x11 * y11 + x12 * y21;
              r12 += x10 * y02 + x11 * y12 + x12 * y22;
              r13 += x10 * y03 + x11 * y13 + x12 * y23;
            }
          else if (k_remain == 2)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1];
              double x10 = x_row1[0], x11 = x_row1[1];

              double *y_row0 = y_arr + y_row_len * mid_stop + j;
              double *y_row1 = y_row0 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];

              r00 += x00 * y00 + x01 * y10;
              r01 += x00 * y01 + x01 * y11;
              r02 += x00 * y02 + x01 * y12;
              r03 += x00 * y03 + x01 * y13;

              r10 += x10 * y00 + x11 * y10;
              r11 += x10 * y01 + x11 * y11;
              r12 += x10 * y02 + x11 * y12;
              r13 += x10 * y03 + x11 * y13;
            }
          else if (k_remain == 1)
            {
              double x0k = x_arr[x_row_len * i + mid_stop];
              double x1k = x_arr[x_row_len * (i + 1) + mid_stop];

              double *y_row = y_arr + y_row_len * mid_stop + j;
              double yk0 = y_row[0], yk1 = y_row[1], yk2 = y_row[2],
                     yk3 = y_row[3];

              r00 += x0k * yk0;
              r01 += x0k * yk1;
              r02 += x0k * yk2;
              r03 += x0k * yk3;
              r10 += x1k * yk0;
              r11 += x1k * yk1;
              r12 += x1k * yk2;
              r13 += x1k * yk3;
            }

          // Сохранение результатов
          double *res_row = arr + c_n * i + j;
          res_row[0] = r00;
          res_row[1] = r01;
          res_row[2] = r02;
          res_row[3] = r03;
          res_row += c_n;
          res_row[0] = r10;
          res_row[1] = r11;
          res_row[2] = r12;
          res_row[3] = r13;
        }

      // Остаточные столбцы для 2 строк
      size_t j_remain = c_n - col_stop;

      if (j_remain == 3)
        {
          for (size_t j = col_stop; j < c_n; j++)
            {
              double r0 = 0, r1 = 0;

              for (size_t k = 0; k < mid_stop; k += 4)
                {
                  double *x_row0 = x_arr + x_row_len * i + k;
                  double *x_row1 = x_row0 + x_row_len;

                  double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                         x03 = x_row0[3];
                  double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                         x13 = x_row1[3];

                  double *y_col = y_arr + y_row_len * k + j;
                  double y0 = y_col[0];
                  double y1 = y_col[y_row_len];
                  double y2 = y_col[2 * y_row_len];
                  double y3 = y_col[3 * y_row_len];

                  r0 += x00 * y0 + x01 * y1 + x02 * y2 + x03 * y3;
                  r1 += x10 * y0 + x11 * y1 + x12 * y2 + x13 * y3;
                }

              // Хвост по k для остаточных столбцов
              size_t k_remain = mid - mid_stop;

              if (k_remain == 3)
                {
                  double *x_row0 = x_arr + x_row_len * i + mid_stop;
                  double *x_row1 = x_row0 + x_row_len;

                  double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
                  double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];

                  double *y_col0 = y_arr + y_row_len * mid_stop + j;
                  double *y_col1 = y_col0 + y_row_len;
                  double *y_col2 = y_col1 + y_row_len;

                  double y0 = y_col0[0];
                  double y1 = y_col1[0];
                  double y2 = y_col2[0];

                  r0 += x00 * y0 + x01 * y1 + x02 * y2;
                  r1 += x10 * y0 + x11 * y1 + x12 * y2;
                }
              else if (k_remain == 2)
                {
                  double *x_row0 = x_arr + x_row_len * i + mid_stop;
                  double *x_row1 = x_row0 + x_row_len;

                  double x00 = x_row0[0], x01 = x_row0[1];
                  double x10 = x_row1[0], x11 = x_row1[1];

                  double *y_col0 = y_arr + y_row_len * mid_stop + j;
                  double *y_col1 = y_col0 + y_row_len;

                  double y0 = y_col0[0];
                  double y1 = y_col1[0];

                  r0 += x00 * y0 + x01 * y1;
                  r1 += x10 * y0 + x11 * y1;
                }
              else if (k_remain == 1)
                {
                  double x0k = x_arr[x_row_len * i + mid_stop];
                  double x1k = x_arr[x_row_len * (i + 1) + mid_stop];

                  double ykj = y_arr[y_row_len * mid_stop + j];

                  r0 += x0k * ykj;
                  r1 += x1k * ykj;
                }

              arr[c_n * i + j] = r0;
              arr[c_n * (i + 1) + j] = r1;
            }
        }
      else if (j_remain == 2)
        {
          // Обрабатываем сразу 2 столбца для 2 строк
          double r00 = 0, r01 = 0;
          double r10 = 0, r11 = 0;

          for (size_t k = 0; k < mid_stop; k += 4)
            {
              double *x_row0 = x_arr + x_row_len * i + k;
              double *x_row1 = x_row0 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                     x03 = x_row0[3];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                     x13 = x_row1[3];

              double *y_col0 = y_arr + y_row_len * k + col_stop;
              double *y_col1 = y_col0 + 1;

              double y00 = y_col0[0], y01 = y_col1[0];
              double y10 = y_col0[y_row_len], y11 = y_col1[y_row_len];
              double y20 = y_col0[2 * y_row_len], y21 = y_col1[2 * y_row_len];
              double y30 = y_col0[3 * y_row_len], y31 = y_col1[3 * y_row_len];

              r00 += x00 * y00 + x01 * y10 + x02 * y20 + x03 * y30;
              r01 += x00 * y01 + x01 * y11 + x02 * y21 + x03 * y31;

              r10 += x10 * y00 + x11 * y10 + x12 * y20 + x13 * y30;
              r11 += x10 * y01 + x11 * y11 + x12 * y21 + x13 * y31;
            }

          // Хвост по k для 2 столбцов и 2 строк
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];

              double *y_row0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1];
              double y10 = y_row1[0], y11 = y_row1[1];
              double y20 = y_row2[0], y21 = y_row2[1];

              r00 += x00 * y00 + x01 * y10 + x02 * y20;
              r01 += x00 * y01 + x01 * y11 + x02 * y21;

              r10 += x10 * y00 + x11 * y10 + x12 * y20;
              r11 += x10 * y01 + x11 * y11 + x12 * y21;
            }
          else if (k_remain == 2)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1];
              double x10 = x_row1[0], x11 = x_row1[1];

              double *y_row0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_row1 = y_row0 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1];
              double y10 = y_row1[0], y11 = y_row1[1];

              r00 += x00 * y00 + x01 * y10;
              r01 += x00 * y01 + x01 * y11;

              r10 += x10 * y00 + x11 * y10;
              r11 += x10 * y01 + x11 * y11;
            }
          else if (k_remain == 1)
            {
              double x0k = x_arr[x_row_len * i + mid_stop];
              double x1k = x_arr[x_row_len * (i + 1) + mid_stop];

              double *y_row = y_arr + y_row_len * mid_stop + col_stop;
              double yk0 = y_row[0], yk1 = y_row[1];

              r00 += x0k * yk0;
              r01 += x0k * yk1;
              r10 += x1k * yk0;
              r11 += x1k * yk1;
            }

          arr[c_n * i + col_stop] = r00;
          arr[c_n * i + col_stop + 1] = r01;
          arr[c_n * (i + 1) + col_stop] = r10;
          arr[c_n * (i + 1) + col_stop + 1] = r11;
        }
      else if (j_remain == 1)
        {
          double r0 = 0, r1 = 0;

          for (size_t k = 0; k < mid_stop; k += 4)
            {
              double *x_row0 = x_arr + x_row_len * i + k;
              double *x_row1 = x_row0 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2],
                     x03 = x_row0[3];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2],
                     x13 = x_row1[3];

              double *y_col = y_arr + y_row_len * k + col_stop;
              double y0 = y_col[0];
              double y1 = y_col[y_row_len];
              double y2 = y_col[2 * y_row_len];
              double y3 = y_col[3 * y_row_len];

              r0 += x00 * y0 + x01 * y1 + x02 * y2 + x03 * y3;
              r1 += x10 * y0 + x11 * y1 + x12 * y2 + x13 * y3;
            }

          // Хвост по k для 1 столбца и 2 строк
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1], x02 = x_row0[2];
              double x10 = x_row1[0], x11 = x_row1[1], x12 = x_row1[2];

              double *y_col0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_col1 = y_col0 + y_row_len;
              double *y_col2 = y_col1 + y_row_len;

              double y0 = y_col0[0];
              double y1 = y_col1[0];
              double y2 = y_col2[0];

              r0 += x00 * y0 + x01 * y1 + x02 * y2;
              r1 += x10 * y0 + x11 * y1 + x12 * y2;
            }
          else if (k_remain == 2)
            {
              double *x_row0 = x_arr + x_row_len * i + mid_stop;
              double *x_row1 = x_row0 + x_row_len;

              double x00 = x_row0[0], x01 = x_row0[1];
              double x10 = x_row1[0], x11 = x_row1[1];

              double *y_col0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_col1 = y_col0 + y_row_len;

              double y0 = y_col0[0];
              double y1 = y_col1[0];

              r0 += x00 * y0 + x01 * y1;
              r1 += x10 * y0 + x11 * y1;
            }
          else if (k_remain == 1)
            {
              double x0k = x_arr[x_row_len * i + mid_stop];
              double x1k = x_arr[x_row_len * (i + 1) + mid_stop];

              double ykj = y_arr[y_row_len * mid_stop + col_stop];

              r0 += x0k * ykj;
              r1 += x1k * ykj;
            }

          arr[c_n * i + col_stop] = r0;
          arr[c_n * (i + 1) + col_stop] = r1;
        }
    }
  else if (i_remain == 1)
    {
      size_t i = row_stop;

      // Основные блоки по j
      for (size_t j = 0; j < col_stop; j += 4)
        {
          double r0 = 0, r1 = 0, r2 = 0, r3 = 0;

          // Основные блоки по k
          for (size_t k = 0; k < mid_stop; k += 4)
            {
              double *x_row = x_arr + x_row_len * i + k;
              double x0 = x_row[0], x1 = x_row[1], x2 = x_row[2],
                     x3 = x_row[3];

              double *y_row0 = y_arr + y_row_len * k + j;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;
              double *y_row3 = y_row2 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];
              double y20 = y_row2[0], y21 = y_row2[1], y22 = y_row2[2],
                     y23 = y_row2[3];
              double y30 = y_row3[0], y31 = y_row3[1], y32 = y_row3[2],
                     y33 = y_row3[3];

              r0 += x0 * y00 + x1 * y10 + x2 * y20 + x3 * y30;
              r1 += x0 * y01 + x1 * y11 + x2 * y21 + x3 * y31;
              r2 += x0 * y02 + x1 * y12 + x2 * y22 + x3 * y32;
              r3 += x0 * y03 + x1 * y13 + x2 * y23 + x3 * y33;
            }

          // Хвост по k для 1 строки
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row = x_arr + x_row_len * i + mid_stop;
              double x0 = x_row[0], x1 = x_row[1], x2 = x_row[2];

              double *y_row0 = y_arr + y_row_len * mid_stop + j;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];
              double y20 = y_row2[0], y21 = y_row2[1], y22 = y_row2[2],
                     y23 = y_row2[3];

              r0 += x0 * y00 + x1 * y10 + x2 * y20;
              r1 += x0 * y01 + x1 * y11 + x2 * y21;
              r2 += x0 * y02 + x1 * y12 + x2 * y22;
              r3 += x0 * y03 + x1 * y13 + x2 * y23;
            }
          else if (k_remain == 2)
            {
              double *x_row = x_arr + x_row_len * i + mid_stop;
              double x0 = x_row[0], x1 = x_row[1];

              double *y_row0 = y_arr + y_row_len * mid_stop + j;
              double *y_row1 = y_row0 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1], y02 = y_row0[2],
                     y03 = y_row0[3];
              double y10 = y_row1[0], y11 = y_row1[1], y12 = y_row1[2],
                     y13 = y_row1[3];

              r0 += x0 * y00 + x1 * y10;
              r1 += x0 * y01 + x1 * y11;
              r2 += x0 * y02 + x1 * y12;
              r3 += x0 * y03 + x1 * y13;
            }
          else if (k_remain == 1)
            {
              double xk = x_arr[x_row_len * i + mid_stop];

              double *y_row = y_arr + y_row_len * mid_stop + j;
              double y0 = y_row[0], y1 = y_row[1], y2 = y_row[2],
                     y3 = y_row[3];

              r0 += xk * y0;
              r1 += xk * y1;
              r2 += xk * y2;
              r3 += xk * y3;
            }

          // Сохранение результатов
          double *res_row = arr + c_n * i + j;
          res_row[0] = r0;
          res_row[1] = r1;
          res_row[2] = r2;
          res_row[3] = r3;
        }

      // Остаточные столбцы для 1 строки
      size_t j_remain = c_n - col_stop;

      if (j_remain == 3)
        {
          for (size_t j = col_stop; j < c_n; j++)
            {
              double r = 0;

              for (size_t k = 0; k < mid_stop; k += 4)
                {
                  double *x_row = x_arr + x_row_len * i + k;
                  double x0 = x_row[0], x1 = x_row[1], x2 = x_row[2],
                         x3 = x_row[3];

                  double *y_col = y_arr + y_row_len * k + j;
                  double y0 = y_col[0];
                  double y1 = y_col[y_row_len];
                  double y2 = y_col[2 * y_row_len];
                  double y3 = y_col[3 * y_row_len];

                  r += x0 * y0 + x1 * y1 + x2 * y2 + x3 * y3;
                }

              // Хвост по k для остаточных столбцов
              size_t k_remain = mid - mid_stop;

              if (k_remain == 3)
                {
                  double *x_row = x_arr + x_row_len * i + mid_stop;
                  double x0 = x_row[0], x1 = x_row[1], x2 = x_row[2];

                  double *y_col0 = y_arr + y_row_len * mid_stop + j;
                  double *y_col1 = y_col0 + y_row_len;
                  double *y_col2 = y_col1 + y_row_len;

                  double y0 = y_col0[0];
                  double y1 = y_col1[0];
                  double y2 = y_col2[0];

                  r += x0 * y0 + x1 * y1 + x2 * y2;
                }
              else if (k_remain == 2)
                {
                  double *x_row = x_arr + x_row_len * i + mid_stop;
                  double x0 = x_row[0], x1 = x_row[1];

                  double *y_col0 = y_arr + y_row_len * mid_stop + j;
                  double *y_col1 = y_col0 + y_row_len;

                  double y0 = y_col0[0];
                  double y1 = y_col1[0];

                  r += x0 * y0 + x1 * y1;
                }
              else if (k_remain == 1)
                {
                  double xk = x_arr[x_row_len * i + mid_stop];
                  double ykj = y_arr[y_row_len * mid_stop + j];

                  r += xk * ykj;
                }

              arr[c_n * i + j] = r;
            }
        }
      else if (j_remain == 2)
        {
          // Обрабатываем сразу 2 столбца для 1 строки
          double r0 = 0, r1 = 0;

          for (size_t k = 0; k < mid_stop; k += 4)
            {
              double *x_row = x_arr + x_row_len * i + k;
              double x0 = x_row[0], x1 = x_row[1], x2 = x_row[2],
                     x3 = x_row[3];

              double *y_col0 = y_arr + y_row_len * k + col_stop;
              double *y_col1 = y_col0 + 1;

              double y00 = y_col0[0], y01 = y_col1[0];
              double y10 = y_col0[y_row_len], y11 = y_col1[y_row_len];
              double y20 = y_col0[2 * y_row_len], y21 = y_col1[2 * y_row_len];
              double y30 = y_col0[3 * y_row_len], y31 = y_col1[3 * y_row_len];

              r0 += x0 * y00 + x1 * y10 + x2 * y20 + x3 * y30;
              r1 += x0 * y01 + x1 * y11 + x2 * y21 + x3 * y31;
            }

          // Хвост по k для 2 столбцов и 1 строки
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row = x_arr + x_row_len * i + mid_stop;
              double x0 = x_row[0], x1 = x_row[1], x2 = x_row[2];

              double *y_row0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_row1 = y_row0 + y_row_len;
              double *y_row2 = y_row1 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1];
              double y10 = y_row1[0], y11 = y_row1[1];
              double y20 = y_row2[0], y21 = y_row2[1];

              r0 += x0 * y00 + x1 * y10 + x2 * y20;
              r1 += x0 * y01 + x1 * y11 + x2 * y21;
            }
          else if (k_remain == 2)
            {
              double *x_row = x_arr + x_row_len * i + mid_stop;
              double x0 = x_row[0], x1 = x_row[1];

              double *y_row0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_row1 = y_row0 + y_row_len;

              double y00 = y_row0[0], y01 = y_row0[1];
              double y10 = y_row1[0], y11 = y_row1[1];

              r0 += x0 * y00 + x1 * y10;
              r1 += x0 * y01 + x1 * y11;
            }
          else if (k_remain == 1)
            {
              double xk = x_arr[x_row_len * i + mid_stop];

              double *y_row = y_arr + y_row_len * mid_stop + col_stop;
              double y0 = y_row[0], y1 = y_row[1];

              r0 += xk * y0;
              r1 += xk * y1;
            }

          arr[c_n * i + col_stop] = r0;
          arr[c_n * i + col_stop + 1] = r1;
        }
      else if (j_remain == 1)
        {
          double r = 0;

          for (size_t k = 0; k < mid_stop; k += 4)
            {
              double *x_row = x_arr + x_row_len * i + k;
              double x0 = x_row[0], x1 = x_row[1], x2 = x_row[2],
                     x3 = x_row[3];

              double *y_col = y_arr + y_row_len * k + col_stop;
              double y0 = y_col[0];
              double y1 = y_col[y_row_len];
              double y2 = y_col[2 * y_row_len];
              double y3 = y_col[3 * y_row_len];

              r += x0 * y0 + x1 * y1 + x2 * y2 + x3 * y3;
            }

          // Хвост по k для 1 столбца и 1 строки
          size_t k_remain = mid - mid_stop;

          if (k_remain == 3)
            {
              double *x_row = x_arr + x_row_len * i + mid_stop;
              double x0 = x_row[0], x1 = x_row[1], x2 = x_row[2];

              double *y_col0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_col1 = y_col0 + y_row_len;
              double *y_col2 = y_col1 + y_row_len;

              double y0 = y_col0[0];
              double y1 = y_col1[0];
              double y2 = y_col2[0];

              r += x0 * y0 + x1 * y1 + x2 * y2;
            }
          else if (k_remain == 2)
            {
              double *x_row = x_arr + x_row_len * i + mid_stop;
              double x0 = x_row[0], x1 = x_row[1];

              double *y_col0 = y_arr + y_row_len * mid_stop + col_stop;
              double *y_col1 = y_col0 + y_row_len;

              double y0 = y_col0[0];
              double y1 = y_col1[0];

              r += x0 * y0 + x1 * y1;
            }
          else if (k_remain == 1)
            {
              double xk = x_arr[x_row_len * i + mid_stop];
              double ykj = y_arr[y_row_len * mid_stop + col_stop];

              r += xk * ykj;
            }

          arr[c_n * i + col_stop] = r;
        }
    }

  set_r_num (r_n);
  set_c_num (c_n);
  return execution_status::success;
}
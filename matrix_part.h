#ifndef MATRIX_PART_H
#define MATRIX_PART_H
#include "block_string.h"
#include "block_view.h"
#include "data.h"
class matrix_part : public m_sizes, public data
{
private:
  size_t part_index{};
  size_t p{};
  size_t part_size{};

  double norm{};

  bool ost_flag{};

public:
  matrix_part () = default;
  ~matrix_part () = default;
  matrix_part (const matrix_part &x) = delete;
  matrix_part (matrix_part &&x) = delete;
  matrix_part &operator= (const matrix_part &x) = delete;
  matrix_part &operator= (matrix_part &&x) = delete;

  size_t get_part_size ();
  execution_status init_matrix_part (size_t m_size, size_t b_size,
                                     size_t _index, size_t _p);
  void copy_b_str (size_t str_index, double *arr);
  void init_by_formula (size_t s);
  double formula (size_t s, size_t i, size_t j);
  execution_status read_b_row_from_file_to_arr (size_t global_b_row_index,
                                                size_t &size, double *arr,
                                                FILE *fp);
  execution_status get_row_part (size_t row_index, size_t row_size,
                                 double *row);
  execution_status get_col_part (size_t col_index, size_t b_index,
                                 double *col);
  size_t calculate_norm_part (size_t global_b_col_index, double *norm_part);
  double *get_b_row_pointer (size_t b_row_index);
  void set_norm (double val);
  size_t get_local_bost_size (size_t index);
  void get_block (block_view &b, size_t local_r_index, size_t c_index);
  void get_b_transpozition_block (block_view &b, size_t local_r_index,
                                  size_t c_index);
  void get_transpozition_block (block_view &b, size_t local_r_index,
                                  size_t c_index);
  void set_transpozition_block (block_view &b, size_t local_r_index,
                                  size_t c_index);
  void set_block (block_view &b, size_t local_r_index, size_t c_index);
  void get_b_str (block_string &b_str, size_t local_index, size_t start,
                  size_t end);
};

#endif // MATRIX_PART_H
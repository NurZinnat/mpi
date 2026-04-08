#ifndef BLOCK_STRING_H
#define BLOCK_STRING_H
#include "block_view.h"
#include "execution_status.h"

class block_string : public data_view, private m_sizes
{
  size_t local_index{};
  size_t global_index{};
  size_t start_r_index{};
  size_t r_num{};
  size_t ost{};
  size_t str_size{};

public:
  block_string () = default;
  ~block_string () = default;

  block_string (const block_string &x) = delete;
  block_string (block_string &&x) = delete;
  block_string &operator= (const block_string &x) = delete;
  block_string &operator= (block_string &&x) = delete;

  void init_block_string (size_t m_size, size_t b_size, double *arr,
                          size_t _local_index, size_t _global_index);

  // Геттеры (const) для ВСЕХ полей
  size_t get_local_index () const;
  size_t get_global_index () const;
  size_t get_start_r_index () const;
  size_t get_r_num () const;
  size_t get_shift () const;
  size_t get_str_size () const;

  // Сеттеры для ВСЕХ полей
  void set_local_b_r_index (size_t value);
  void set_global_b_r_index (size_t value);
  void set_start_r_index (size_t value);
  void set_r_num (size_t value);
  void set_shift (size_t value);
  void set_str_size (size_t value);


  void init_by_formula (size_t s);
  double formula (size_t s, size_t i, size_t j);
  execution_status read_b_row_from_file_to_arr (FILE *fp);
  void get_block (block_view &b, size_t c_b_index);
  void set_block (block_view &b, size_t c_b_index);
  void set_params (size_t start, size_t end, size_t _r_size, double *_arr);
};

#endif
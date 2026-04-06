#ifndef BLOCK_STRING_H
#define BLOCK_STRING_H
#include "block_view.h"
#include "execution_status.h"

class block_string : public data_view, private m_sizes
{
  size_t local_b_r_index{};
  size_t global_b_r_index{};
  size_t start_r_index{};
  size_t r_size{};
  size_t shift{};
  size_t bxb_size{};
  size_t ost_bxb_size{};

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
  size_t get_local_b_r_index () const;
  size_t get_global_b_r_index () const;
  size_t get_start_r_index () const;
  size_t get_r_size () const;
  size_t get_shift () const;

  // Сеттеры для ВСЕХ полей
  void set_local_b_r_index (size_t value);
  void set_global_b_r_index (size_t value);
  void set_start_r_index (size_t value);
  void set_r_size (size_t value);
  void set_shift (size_t value);

  void get_block (block_view &b, size_t c_b_index);

  void init_by_formula (size_t s);
  double formula (size_t s, size_t i, size_t j);
  execution_status read_b_row_from_file_to_arr (FILE *fp);
};

#endif
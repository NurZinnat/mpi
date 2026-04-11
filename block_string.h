#ifndef BLOCK_STRING_H
#define BLOCK_STRING_H
#include "block_view.h"
#include "execution_status.h"

class block_string : public data_view, public m_sizes
{
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

  void init_block_string (size_t m_size, size_t b_size);

  // Геттеры (const) для ВСЕХ полей
 
  size_t get_r_num () const;
  size_t get_str_size () const;

  // Сеттеры для ВСЕХ полей
 
  void set_r_num (size_t value);
  void set_str_size (size_t value);


  void get_block (block_view &b, size_t c_b_index);
  void set_block (block_view &b, size_t c_b_index);
  void set_params (double *arr, size_t _str_size, size_t _r_num, size_t _ost);
  void print_params ();
};

#endif
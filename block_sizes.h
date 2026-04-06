#ifndef BLOCK_SIZES_H
#define BLOCK_SIZES_H

#include <stddef.h>
#include "matrix_type.h"

class block_sizes
{
protected:
  size_t r_num{};
  size_t c_num{};
  matrix_type m_type{};


public:
  block_sizes () = default;
  ~block_sizes () = default;
  block_sizes (const block_sizes &x) = default;
  block_sizes (block_sizes &&x) = default;
  block_sizes &operator= (const block_sizes &x) = default;
  block_sizes &operator= (block_sizes &&x) = default;

  // Getters
  size_t get_r_num () const;
  size_t get_c_num () const;
  matrix_type get_m_type () const;

  // Setters
  void set_r_num (size_t _r_num);
  void set_c_num (size_t _c_num);
  void set_m_type (matrix_type _m_type);

  // Utility
  bool is_square () const;
};

#endif // BLOCK_SIZES_H
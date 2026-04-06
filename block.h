#ifndef BLOCK_H
#define BLOCK_H

#include "block_sizes.h"
#include "data.h"
#include "m_sizes.h"
#include "matrix_type.h"
#include "precision.h"
#include "block_view.h"

#include <math.h>
class block : public data, public m_sizes, public precision, public block_sizes
{
public:
  block () = default;
  ~block () = default;
  block (const block &x) = delete;
  block &operator= (const block &x);
  block &operator+= (const block &x);
  block (block &&x) = delete;
  block &operator= (block &&x) = delete;
  block &operator*= (double val);

 
  void print ();

  execution_status init_block (size_t m_size, size_t b_size, double eps);
  bool square ();

  inline execution_status block_multiply (block_view &x, block_view &y);
  inline execution_status sum_block_multiply (block_view &x, block_view &y);

  execution_status inverse_triangular_upper (block_view &x);
  void zero_padding ();
};
// #include "multy.inl"
// #include "sum_multy.inl"

#endif // BLOCK_H
#ifndef BLOCK_H
#define BLOCK_H

#include "data.h"
#include "m_sizes.h"
#include "matrix_type.h"
#include "precision.h"

#include <math.h>
class block : public data, public m_sizes, public precision
{
  size_t r_num{};
  size_t c_num{};

  matrix_type m_type{};

private:
public:
  block () = default;
  ~block () = default;
  block (const block &x) = delete;
  block &operator= (const block &x);
  block &operator+= (const block &x);
  block (block &&x) = delete;
  block &operator= (block &&x) = delete;
  block &operator*= (double val);

  // gets::

  size_t get_r_num () const;
  size_t get_c_num () const;
  matrix_type get_m_type ();

  // sets::

  void set_r_num (size_t _r_num);
  void set_c_num (size_t _c_num);
  void set_m_type (matrix_type _m_type);
  void print ();

  execution_status init_block (size_t _b_size, double _eps);
  bool square ();

  inline execution_status block_multiply (block &x, block &y);
  inline execution_status sum_block_multiply (block &x, block &y);

  execution_status inverse_triangular_upper (block &x);
  void zero_padding ();
};
// #include "multy.inl"
// #include "sum_multy.inl"

#endif // BLOCK_H
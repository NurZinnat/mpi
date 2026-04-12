#ifndef BLOCK_VIEW_H
#define BLOCK_VIEW_H

#include "block_sizes.h"
#include "data_view.h"
#include "execution_status.h"
#include "m_sizes.h"
#include "matrix_type.h"
#include "precision.h"

#include <math.h>

class block_view : public block_sizes,
                   public m_sizes,
                   public data_view,
                   public precision
{

public:
  block_view () = default;
  ~block_view () = default;
  block_view (const block_view &x) = delete;
  block_view &operator= (const block_view &x);
  block_view &operator+= (const block_view &x);
  block_view (block_view &&x) = delete;
  block_view &operator= (block_view &&x) = delete;
  block_view &operator*= (double val);

  void print ();
  void init_block_view (size_t m_size, size_t b_size, double eps);
  bool square ();
  execution_status inverse_triangular_upper (block_view &x);
  execution_status inverse_triangular_lower (block_view &x);
  void zero_padding ();
  void print_debug ();
  inline execution_status block_multiply (block_view &x, block_view &y);
  inline execution_status sum_block_multiply (block_view &x, block_view &y);
  void sum_abs_columns (double *res_arr);
  void print_real ();
};

#include "multy.inl"
#include "sum_multy.inl"

#endif // BLOCK_VIEW_H
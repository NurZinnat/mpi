#ifndef BLOCK_VIEW_H
#define BLOCK_VIEW_H

#include "block_sizes.h"
#include "data_view.h"
#include "m_sizes.h"
#include "matrix_type.h"
#include "precision.h"

#include <math.h>

class block_view : public block_sizes, public m_sizes, public data_view
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
  void init_block_view (size_t m_size, size_t b_size);
  bool square ();
};

#endif // BLOCK_VIEW_H
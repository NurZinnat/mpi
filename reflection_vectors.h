#ifndef REFLECTION_VECTORS_H
#define REFLECTION_VECTORS_H

#include "block_view.h"
#include "data_view.h"
#include "rv_sizes.h"

class block_string;

class reflection_vectors : public m_sizes,
                           public precision,
                           public rv_sizes,
                           public data_view
{
public:
  enum class reflection_type
  {
    triangular,
    reset
  };

public:
  reflection_vectors () = default;
  ~reflection_vectors () = default;
  reflection_vectors (const reflection_vectors &x) = delete;
  reflection_vectors &operator= (const reflection_vectors &x);
  reflection_vectors (reflection_vectors &&x) = delete;
  reflection_vectors &operator= (reflection_vectors &&x) = delete;

  execution_status init_reflection_vectors (size_t m_size, size_t b_size,
                                            double eps);

  execution_status build_triangular_reflection (block_view &x);
  execution_status spread_triangular_reflection (block_view &b);
  execution_status spread_triangular_reflection_and_ref (block_view &b);
  execution_status spread_transposition_triangular_reflection (block_view &b);
  execution_status build_reset_reflection (block_view &b, block_view &di_b);
  execution_status build_reset_reflection_and_ref (block_view &b,
                                                   block_view &di_b);

  execution_status spread_reset_reflection (block_view &bl, block_view &di_bl);
  execution_status spread_reset_reflection_and_ref (block_view &bl,
                                                    block_view &di_bl);
  execution_status spread_transposition_reset_reflection (block_view &bl,
                                                          block_view &di_bl);

  void print (size_t size);

  void copy_triangular_reflection (reflection_vectors &x);
  void copy_reset_reflection (reflection_vectors &x);
  void push (reflection_type rv_type, size_t r_index, size_t c_index);
  void pop (reflection_type rv_type, size_t r_index, size_t c_index);

  void spread_triangular_reflection (block_string &str, block_view *blocks,
                                     size_t c_start, size_t c_end);
  void spread_reset_reflection (block_string &str,
                                block_string &sub_str, block_view *blocks, size_t c_start,
                                size_t c_end);
};

#endif // REFLECTION_VECTORS_H

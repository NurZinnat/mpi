#ifndef REFLECTION_VECTORS_H
#define REFLECTION_VECTORS_H

#include "block.h"
#include "rv_sizes.h"

class reflection_vectors : public data,
                           public m_sizes,
                           public precision,
                           public rv_sizes
{
  
public:
  reflection_vectors () = default;
  ~reflection_vectors () = default;
  reflection_vectors (const reflection_vectors &x) = delete;
  reflection_vectors &operator= (const reflection_vectors &x);
  reflection_vectors (reflection_vectors &&x) = delete;
  reflection_vectors &operator= (reflection_vectors &&x) = delete;

  execution_status init_reflection_vectors (size_t _b_size, double _eps);

  execution_status build_triangular_reflection (block &x);
  execution_status spread_triangular_reflection (block &b);
  execution_status spread_triangular_reflection_and_ref (block &b);
  execution_status spread_transposition_triangular_reflection (block &b);
  execution_status build_reset_reflection (block &b, block &di_b);
  execution_status build_reset_reflection_and_ref (block &b, block &di_b);

  execution_status spread_reset_reflection (block &bl, block &di_bl);
  execution_status spread_reset_reflection_and_ref (block &bl, block &di_bl);
  execution_status spread_transposition_reset_reflection (block &bl,
                                                          block &di_bl);

  void print (size_t size);

  void copy_triangular_reflection (reflection_vectors &x);
  void copy_reset_reflection (reflection_vectors &x);  
  

};



class reflection_vectors_storage
{
  double *arr{}; // в данной версии память не вяделяется. Присваиваем левый указатель.
  size_t arr_size{};

public:
  reflection_vectors_storage () = default;
  ~reflection_vectors_storage () = default;
  reflection_vectors_storage (const reflection_vectors_storage &x) = delete;
  reflection_vectors_storage (reflection_vectors_storage &&x) = delete;
  reflection_vectors_storage operator= (const reflection_vectors_storage &x)
      = delete;
  reflection_vectors_storage operator= (reflection_vectors_storage &&x)
      = delete;
  void set_reflection_storage (double *val);
  void push (size_t val_size, double *val);
    void push (reflection_vectors &rv);

  // в данной версии предполагается, что всегда есть запрошенная длина
  void pop (size_t val_size, double *val);
  void pop (reflection_vectors &rv);

};

#endif // REFLECTION_VECTORS_H

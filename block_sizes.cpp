#include "block_sizes.h"

// Getters

size_t
block_sizes::get_r_num () const
{
  return r_num;
}

size_t
block_sizes::get_c_num () const
{
  return c_num;
}

matrix_type
block_sizes::get_m_type () const
{
  return m_type;
}

// Setters

void
block_sizes::set_r_num (size_t _r_num)
{
  r_num = _r_num;
}

void
block_sizes::set_c_num (size_t _c_num)
{
  c_num = _c_num;
}

void
block_sizes::set_m_type (matrix_type _m_type)
{
  m_type = _m_type;
}

// Utility

bool
block_sizes::is_square () const
{
  return r_num == c_num;
}
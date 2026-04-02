#include "rv_sizes.h"

void
rv_sizes::init_rv_sizes (size_t _b_size, size_t _r)
{
  b_size = _b_size;
  r = _r;
  triangular_rv_size = (b_size + 2) * (b_size - 1) / 2;
  ost_triangular_rv_size = (r + 2) * (r - 2) / 2;
  reset_rv_size = (b_size + 1) * b_size;
  ost_reset_rv_size = (r + 1) * b_size;
}
void
rv_sizes::resolve_params (reflection_type rv_type)
{
  switch (rv_type)
    {
    case reflection_type::triangular:
      vector_size = b_size;
      rv_size = triangular_rv_size;
      break;
    case reflection_type::ost_triangular:
      vector_size = r;
      rv_size = ost_triangular_rv_size;
      break;
    case reflection_type::reset:
      vector_size = (b_size + 1);
      rv_size = reset_rv_size;
      break;
    case reflection_type::ost_reset:
      vector_size = (r + 1);
      rv_size = ost_reset_rv_size;
      break;
    default:
      break;
    }
}

// Геттеры
size_t
rv_sizes::get_vector_size () const
{
  return vector_size;
}

size_t
rv_sizes::get_rv_size () const
{
  return rv_size;
}

size_t
rv_sizes::get_triangular_rv_size () const
{
  return triangular_rv_size;
}

size_t
rv_sizes::get_ost_triangular_rv_size () const
{
  return ost_triangular_rv_size;
}

size_t
rv_sizes::get_reset_rv_size () const
{
  return reset_rv_size;
}

size_t
rv_sizes::get_ost_reset_rv_size () const
{
  return ost_reset_rv_size;
}

// Сеттеры
void
rv_sizes::set_vector_size (size_t value)
{
  vector_size = value;
}

void
rv_sizes::set_rv_size (size_t value)
{
  rv_size = value;
}

void
rv_sizes::set_triangular_rv_size (size_t value)
{
  triangular_rv_size = value;
}

void
rv_sizes::set_ost_triangular_rv_size (size_t value)
{
  ost_triangular_rv_size = value;
}

void
rv_sizes::set_reset_rv_size (size_t value)
{
  reset_rv_size = value;
}

void
rv_sizes::set_ost_reset_rv_size (size_t value)
{
  ost_reset_rv_size = value;
}


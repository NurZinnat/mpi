#include "m_sizes.h"

m_sizes::m_sizes (const m_sizes &x)
{
  m_size = x.m_size;
  b_size = x.b_size;
  k = x.k;
  r = x.r;
}

m_sizes::m_sizes (m_sizes &&x)
{
  m_size = x.m_size;
  b_size = x.b_size;
  k = x.k;
  r = x.r;
  x.r = 0;
  x.k = 0;
  x.b_size = 0;
  x.m_size = 0;
}

m_sizes &
m_sizes::operator= (const m_sizes &x)
{
  m_size = x.m_size;
  b_size = x.b_size;
  k = x.k;
  r = x.r;
  return *this;
}

m_sizes &
m_sizes::operator= (m_sizes &&x)
{
  m_size = x.m_size;
  b_size = x.b_size;
  k = x.k;
  r = x.r;
  x.r = 0;
  x.k = 0;
  x.b_size = 0;
  x.m_size = 0;
  return *this;
}

execution_status
m_sizes::init_m_sizes (size_t _m_size, size_t _b_size)
{
  if (_m_size == 0 || _b_size == 0)
    return execution_status::inval_arg;
  m_size = _m_size;
  b_size = _b_size;
  k = m_size / b_size;
  r = m_size % b_size;
  return execution_status::success;
}

size_t
m_sizes::get_m_size () const
{
  return m_size;
}

size_t
m_sizes::get_b_size () const
{
  return b_size;
}

size_t
m_sizes::get_k () const
{
  return k;
}

size_t
m_sizes::get_r () const
{
  return r;
}

void
m_sizes::set_b_size (size_t val)
{
  b_size = val;
}


size_t
m_sizes::get_bost_size (size_t index)
{
  return m_size - b_size * index > r ? b_size : r;
}
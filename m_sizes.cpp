#include "m_sizes.h"

// Конструктор копирования
m_sizes::m_sizes (const m_sizes &x)
{
  m_size = x.m_size;
  b_size = x.b_size;
  k = x.k;
  r = x.r;
  ordinary_b_str_size = x.ordinary_b_str_size;
  ost_b_str_size = x.ost_b_str_size;
  ordinary_block_size = x.ordinary_block_size;
  ost_block_size = x.ost_block_size;
  ost_ost_block_size = x.ost_ost_block_size;
}

// Конструктор перемещения
m_sizes::m_sizes (m_sizes &&x)
{
  m_size = x.m_size;
  b_size = x.b_size;
  k = x.k;
  r = x.r;
  ordinary_b_str_size = x.ordinary_b_str_size;
  ost_b_str_size = x.ost_b_str_size;
  ordinary_block_size = x.ordinary_block_size;
  ost_block_size = x.ost_block_size;
  ost_ost_block_size = x.ost_ost_block_size;

  x.m_size = 0;
  x.b_size = 0;
  x.k = 0;
  x.r = 0;
  x.ordinary_b_str_size = 0;
  x.ost_b_str_size = 0;
  x.ordinary_block_size = 0;
  x.ost_block_size = 0;
  x.ost_ost_block_size = 0;
}

// Оператор копирования
m_sizes &
m_sizes::operator= (const m_sizes &x)
{
  if (this != &x)
    {
      m_size = x.m_size;
      b_size = x.b_size;
      k = x.k;
      r = x.r;
      ordinary_b_str_size = x.ordinary_b_str_size;
      ost_b_str_size = x.ost_b_str_size;
      ordinary_block_size = x.ordinary_block_size;
      ost_block_size = x.ost_block_size;
      ost_ost_block_size = x.ost_ost_block_size;
    }
  return *this;
}

// Оператор перемещения
m_sizes &
m_sizes::operator= (m_sizes &&x)
{
  if (this != &x)
    {
      m_size = x.m_size;
      b_size = x.b_size;
      k = x.k;
      r = x.r;
      ordinary_b_str_size = x.ordinary_b_str_size;
      ost_b_str_size = x.ost_b_str_size;
      ordinary_block_size = x.ordinary_block_size;
      ost_block_size = x.ost_block_size;
      ost_ost_block_size = x.ost_ost_block_size;

      x.m_size = 0;
      x.b_size = 0;
      x.k = 0;
      x.r = 0;
      x.ordinary_b_str_size = 0;
      x.ost_b_str_size = 0;
      x.ordinary_block_size = 0;
      x.ost_block_size = 0;
      x.ost_ost_block_size = 0;
    }
  return *this;
}

// Инициализация
execution_status
m_sizes::init_m_sizes (size_t _m_size, size_t _b_size)
{
  if (_m_size == 0 || _b_size == 0)
    return execution_status::inval_arg;

  m_size = _m_size;
  b_size = _b_size;
  k = m_size / b_size;
  r = m_size % b_size;

  if (r)
    k++;

  ordinary_b_str_size = m_size * b_size;
  ost_b_str_size = m_size * r;
  ordinary_block_size = b_size * b_size;
  ost_block_size = r * b_size;
  ost_ost_block_size = r * r;

  return execution_status::success;
}

// Геттеры
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

size_t
m_sizes::get_ordinary_b_str_size () const
{
  return ordinary_b_str_size;
}

size_t
m_sizes::get_ost_b_str_size () const
{
  return ost_b_str_size;
}

size_t
m_sizes::get_ordinary_block_size () const
{
  return ordinary_block_size;
}

size_t
m_sizes::get_ost_block_size () const
{
  return ost_block_size;
}

size_t
m_sizes::get_ost_ost_block_size () const
{
  return ost_ost_block_size;
}

// Сеттеры
void
m_sizes::set_b_size (size_t val)
{
  b_size = val;
}

void
m_sizes::set_ordinary_block_size (size_t val)
{
  ordinary_block_size = val;
}

void
m_sizes::set_ost_block_size (size_t val)
{
  ost_block_size = val;
}

void
m_sizes::set_ost_ost_block_size (size_t val)
{
  ost_ost_block_size = val;
}

// Дополнительные методы
size_t
m_sizes::get_bost_size (size_t index)
{
  return m_size - b_size * index > r ? b_size : r;
}
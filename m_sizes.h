#ifndef M_SIZES_H
#define M_SIZES_H

#include "execution_status.h"
#include <stdio.h>
#include <stdlib.h>

class m_sizes
{
private:
  size_t m_size{};
  size_t b_size{};
  size_t k{};
  size_t r{};
  size_t ordinary_b_str_size{};
  size_t ost_b_str_size{};
  size_t ordinary_block_size{};
  size_t ost_block_size{};
  size_t ost_ost_block_size{};

public:
  m_sizes () = default;
  ~m_sizes () = default;
  m_sizes (const m_sizes &x);
  m_sizes (m_sizes &&x);
  m_sizes &operator= (const m_sizes &x);
  m_sizes &operator= (m_sizes &&x);

  execution_status init_m_sizes (size_t _m_size, size_t _b_size);

  // Геттеры
  size_t get_m_size () const;
  size_t get_b_size () const;
  size_t get_k () const;
  size_t get_r () const;
  size_t get_ordinary_b_str_size () const;
  size_t get_ost_b_str_size () const;
  size_t get_ordinary_block_size () const;
  size_t get_ost_block_size () const;
  size_t get_ost_ost_block_size () const;

  // Сеттеры
  void set_b_size (size_t val);
  void set_ordinary_block_size (size_t val);
  void set_ost_block_size (size_t val);
  void set_ost_ost_block_size (size_t val);

  size_t get_bost_size (size_t index);
};

#endif // M_SIZES_H
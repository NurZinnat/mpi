#ifndef RV_SIZES_H
#define RV_SIZES_H
#include <stdlib.h>
class rv_sizes
{
public:
  enum class reflection_type
  {
    triangular,
    ost_triangular,
    reset,
    ost_reset
  };

private:
  size_t vector_size{};
  size_t rv_size{};

  size_t b_size{};
  size_t r{};
  size_t triangular_rv_size{};
  size_t ost_triangular_rv_size{};
  size_t reset_rv_size{};
  size_t ost_reset_rv_size{};

public:
  rv_sizes () = default;
  ~rv_sizes () = default;
  rv_sizes (const rv_sizes &x) = default;
  rv_sizes (rv_sizes &&x) = default;
  rv_sizes &operator= (const rv_sizes &x) = default;
  rv_sizes &operator= (rv_sizes &&x) = default;

  size_t get_vector_size () const;
  size_t get_rv_size () const;
  size_t get_triangular_rv_size () const;
  size_t get_ost_triangular_rv_size () const;

  size_t get_reset_rv_size () const;
  size_t get_ost_reset_rv_size () const;

  void set_vector_size (size_t value);
  void set_rv_size (size_t value);
  void set_triangular_rv_size (size_t value);
  void set_ost_triangular_rv_size (size_t value);

  void set_reset_rv_size (size_t value);
  void set_ost_reset_rv_size (size_t value);

  void init_rv_sizes (size_t _b_size, size_t _r);
  void resolve_params (reflection_type rv_type);
};

#endif // RV_SIZES_H
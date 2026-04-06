#ifndef DATA_VIEW_H
#define DATA_VIEW_H
#include <stdlib.h>
class data_view
{
  double *arr{};
  size_t arr_size{};

public:
  data_view () = default;
  ~data_view () = default;
  data_view (const data_view &x) = delete;
  data_view (data_view &&x) = delete;
  data_view &operator= (const data_view &x) = delete;
  data_view &operator= (data_view &&x) = delete;

  double *get_arr () const;
  double *get_shift_arr () const;
  size_t get_arr_size () const;
  void set_arr (double *val);
  void set_arr_size (size_t val);
  void shift_plus (size_t shift);
  void shift_minus (size_t shift);
};

#endif // DATA_VIEW_H
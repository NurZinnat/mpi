#ifndef DATA_H
#define DATA_H

#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include "execution_status.h"

class data
{
private:
  std::unique_ptr<double[]> arr{};
  size_t arr_size{};

public:
  data () = default;
  ~data () = default;
  data (const data &x) = delete;
  data (data &&x) = delete;
  data &operator= (const data &x) = delete;
  data &operator= (data &&x) = delete;
  execution_status data_memory_allocate (size_t _arr_size);

  double *get_arr () const;
  size_t get_arr_size () const;
  execution_status reinit (size_t n, const data &x);
};

#endif // DATA_H

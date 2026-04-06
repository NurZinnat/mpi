#include "block_view.h"

void
block_view::print ()
{
  printf ("\n\nblock_view_print:\n");
  printf ("r_num = %ld\nc_num = %ld\n", get_r_num (), get_c_num ());
  double *arr = get_arr ();
  for (size_t i = 0; i < get_r_num (); i++)
    {
      size_t index = get_c_num () * i;
      for (size_t j = 0; j < get_c_num (); j++)
        printf (" %10.3e", arr[index++]);
      printf ("\n");
    }
  printf ("\n\n\n");
}

void
block_view::init_block_view (size_t m_size, size_t b_size)
{
  init_m_sizes (m_size, b_size);
  set_r_num (b_size);
  set_c_num (b_size);
}

bool
block_view::square ()
{
  return get_r_num () == get_c_num ();
}
#include "application.h"

void
application::triangulization_local_part (size_t step)
{
  size_t k = get_k ();
  size_t p = get_active_process ();
  size_t p_index = get_process_index ();
  size_t start = step / p + ((p_index < step % p) ? 1 : 0);
  size_t part_size = MATRIX.get_part_size ();
  build_triangular_reflection (start, step);
  push_rv (rv);
  spread_triangular_reflection (start, step + 1, k);
  for (size_t r_index = start + 1; r_index < part_size; r_index++)
  {
    build_reset_reflection (r_index, start, step);
    push_rv (rv);
    spread_reset_reflection (r_index, start, step + 1, k);
  }
}

void
application::triangulization_communicate_part (size_t step)
{
  size_t bin_step = 1;
  size_t bin_step_2x = 2;

}
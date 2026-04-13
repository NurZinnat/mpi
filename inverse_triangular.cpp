#include "application.h"

execution_status
application::inverse_diagonal_blocks ()
{
  size_t part_size = matrix.get_part_size ();
  size_t p = get_active_process ();
  size_t c_index = get_process_index ();
  blocks[0].set_arr (for_blocks.get ());
  for (size_t r_index = 0; r_index < part_size; r_index++)
    {
      matrix.get_block (blocks[1], r_index, c_index);
      blocks[1].set_m_type (matrix_type::triangular_lower);
      status = blocks[0].inverse_triangular_lower (blocks[1]);
      if (status != execution_status::success)
        return status;
      matrix.set_block (blocks[0], r_index, c_index);
      c_index += p;
    }
  return execution_status::success;
}
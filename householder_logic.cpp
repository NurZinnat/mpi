#include "application.h"

void
application::triangulization_local_part (size_t step)
{
  size_t k = get_k ();
  size_t p = get_active_process ();
  size_t p_index = get_process_index ();
  size_t start = step / p + ((p_index < step % p) ? 1 : 0);
  size_t part_size = matrix.get_part_size ();
  size_t part_index = get_process_index ();
  build_triangular_reflection (start, step);
  spread_triangular_reflection (start, step + 1, k);
  return;
  rv.push (reflection_vectors::reflection_type::triangular, step, step);
  for (size_t r_index = start + 1; r_index < part_size; r_index++)
    {
      build_reset_reflection (start, r_index, step);
      spread_reset_reflection (start, r_index, step + 1, k);
      rv.push (reflection_vectors::reflection_type::reset,
               r_index * p + part_index, step);
    }
}

void
application::triangulization_communicate_part (size_t step)
{
  size_t k = get_k ();
  size_t p = get_active_process ();
  size_t p_index = get_process_index ();
  size_t bin_step = 1;
  size_t bin_step_x2 = 2;
  size_t len = k - step;
  while (bin_step < len)
    {
      group.create_group_for_triangulization (step, len, bin_step);
      size_t group_id = group.get_group_id ();
      size_t index_in_group = group.get_index ();
      size_t group_size = group.get_group_size ();
      size_t local_r_index = step / p + ((p_index < step % p) ? 1 : 0);
      size_t r_num_0 = matrix.get_bost_size (step + bin_step_x2 * group_id);
      size_t r_num_last
          = matrix.get_bost_size (step + bin_step_x2 * group_id + bin_step);
      if (index_in_group == 0)
        {
          for (size_t i = 1; i < group_size; i++)
            {
              find_diapazon (k - step, group_size, i, step);
              matrix.get_b_str (str[0], local_r_index, start, end);
              group.send_message (i, str[0], step);
            }

          find_diapazon (k - step, group_size, index_in_group, step);
          matrix.get_b_str (str[0], local_r_index, start, end);
          str[1].set_params (start, end, r_num_last, get_bufer ());
          group.recv_message (group_size - 1, str[1], step);
          str[0].get_block (blocks[0], 0);
          str[1].get_block (blocks[1], 0);
          rv.build_reset_reflection (blocks[0], blocks[1]);
        }
      else if (index_in_group == group_size - 1)
        {
          for (size_t i = 0; i < group_size - 1; i++)
            {
              find_diapazon (k - step, group_size, i, step);
              matrix.get_b_str (str[1], local_r_index, start, end);
              group.send_message (i, str[1], step);
            }

          find_diapazon (k - step, group_size, index_in_group, step);
          matrix.get_b_str (str[1], local_r_index, start, end);
          str[0].set_params (start, end, r_num_0, get_bufer ());
          group.recv_message (0, str[0], step);
        }
      else
        {
          find_diapazon (k - step, group_size, index_in_group, step);

          str[0].set_params (start, end, r_num_0, get_bufer ());
          group.recv_message (0, str[0], step);

          str[1].set_params (start, end, r_num_last, get_bufer ());
          group.recv_message (group_size - 1, str[1], step);
        }
      double *rv_arr = rv.get_shift_arr ();
      size_t rv_arr_size = b_size * r_num_last;
      rv.set_vector_size (r_num_last + 1);
      group.broad_cast (rv_arr_size, rv_arr, 0);
      size_t str_start = 0, str_end = end - start;
      str_start += index_in_group == 0;
      rv.spread_reset_reflection (str[0], str[1], blocks, str_start, str_end);

      if (index_in_group == 0)
        {
          group.send_message (group_size - 1, str[1], step);
          for (size_t i = 1; i < group_size; i++)
            {
              find_diapazon (k - step, group_size, i, step);
              matrix.get_b_str (str[0], local_r_index, start, end);
              group.recv_message (i, str[0], step);
            }
        }
      else if (index_in_group == group_size - 1)
        {
          group.send_message (0, str[0], step);
          for (size_t i = 0; i < group_size - 1; i++)
            {
              find_diapazon (k - step, group_size, i, step);
              matrix.get_b_str (str[1], local_r_index, start, end);
              group.recv_message (i, str[1], step);
            }
        }
      else
        {
          group.send_message (0, str[0], step);
          group.send_message (group_size - 1, str[1], step);
        }
      bin_step <<= 1;
      bin_step_x2 <<= 1;
    }
}

void
application::triangulization ()
{
  size_t k = get_k ();
  for (size_t step = 0; step < k; step++)
    {
      if (step == 1)
        return;
      triangulization_local_part (step);
      //triangulization_communicate_part (step);
    }
}
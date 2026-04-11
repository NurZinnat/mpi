#include "algorithm"
#include "application.h"
#include "simple_functions.h"

void
application::calculate_left_discrepancy ()
{
  double *result_arr = bufer_1.get ();
  size_t k = get_k ();
  size_t part_size = matrix.get_part_size ();
  size_t p_index = get_process_index ();
  // printf ("b_size = %ld\n", b_size);
  //  return;
  for (size_t c_index = 0; c_index < k; c_index++)
    {
      arr_set_val (b_size, result_arr, 0.0);
      for (size_t r_index = 0; r_index < k; r_index++)
        {
          arr_set_val (b_size * b_size * 2, for_blocks.get (), 0.0);
          blocks[0].set_arr (for_blocks.get ());
          for (size_t i = 0; i < part_size; i++)
            {
              sub_matrix.get_block (blocks[1], i, c_index);
              matrix.get_block (blocks[2], i, r_index);
              blocks[0].sum_block_multiply (blocks[1], blocks[2]);
            }
          size_t r_num = blocks[0].get_r_num ();
          size_t c_num = blocks[0].get_c_num ();
          size_t size = r_num * c_num;
          blocks[0].set_arr (for_blocks.get () + size);
          reduce_sum_arr (0, for_blocks.get (), blocks[0].get_arr (), size);
          blocks[0].sum_abs_columns (result_arr);
        }
      if (p_index == 0)
        r1 = std::max (r1, max_from_array (b_size, result_arr));
    }
  broadcast (0, 1, &r1);
}

void
application::calculate_right_discrepancy ()
{
  size_t k = get_k ();
  size_t p = get_process_index ();
  size_t my_idnex = get_process_index ();
  size_t part_size = matrix.get_part_size ();
  double *result_arr = bufer_1.get ();
  double *res_res_arr = bufer_2.get ();
  blocks[0].set_arr (for_blocks.get ());
  for (size_t c_index = 0; c_index < k; c_index++)
    {
      size_t send_index = c_index % p;
      size_t local_r_index = c_index / p;
      double *message = send_index == my_idnex
                            ? matrix.get_b_row_pointer (local_r_index)
                            : get_bufer ();
      size_t r_num = matrix.get_bost_size (c_index);
      size_t message_size = r_num * m_size;
      broadcast (send_index, message_size, message);
      strings[0].set_params (message, k, r_num, matrix.get_bost_size (k));
      arr_set_val (b_size, result_arr, 0.0);
      for (size_t i = 0; i < part_size; i++)
        {
          arr_set_val (b_size * b_size * 2, for_blocks.get (), 0.0);
          strings[1].set_params (sub_matrix.get_b_row_pointer (i), k,
                                 sub_matrix.get_local_bost_size (i),
                                 sub_matrix.get_bost_size (k));
          multiply_strings (strings, blocks);
          blocks[0].sum_abs_columns (result_arr);
        }
      reduce_sum_arr (0, result_arr, res_res_arr, b_size);
      r2 = std::max (r2, max_from_array (b_size, res_res_arr));
    }
  broadcast (0, 1, &r2);
}
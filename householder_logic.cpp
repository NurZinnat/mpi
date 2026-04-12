#include "application.h"

void
application::triangulization_local_part (size_t step)
{
  size_t k = get_k ();
  size_t p = get_active_process ();
  size_t p_index = get_process_index ();
  size_t part_size = matrix.get_part_size ();
  size_t part_index = get_process_index ();
  size_t start = step / p + ((p_index < step % p) ? 1 : 0);
  if (start >= part_size)
    {
      // printf ("kaka\n");
      return;
    }
  // if (step == k - 1)
  //   printf ("index = %ld start = %ld\n", p_index, start);

  if (step != k - 1 && start * p + part_index == k - 1)
    return;
  build_triangular_reflection (start, step);
  spread_triangular_reflection (start, step + 1, k);
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
application::triangulization_communicate_part (size_t step, size_t step_tag)
{

  size_t k = get_k ();
  size_t p = get_active_process ();
  size_t p_index = get_process_index ();
  size_t bin_step = 1;
  size_t bin_step_x2 = 2;
  size_t len = std::min (k - step, p);
  // printf ("k = %ld\n", k);
  size_t count = 0;
  while (bin_step < len)
    {
      size_t tag = step_tag + bin_step;
      size_t broadcast_tag = tag + p;
      bool flag = step == 0 && count == 0;
      group.create_group_for_triangulization (step % p, len, bin_step, flag);
      size_t group_id = group.get_group_id ();
      size_t index_in_group = group.get_index ();
      size_t group_size = group.get_group_size ();
      size_t local_r_index = step / p + ((p_index < step % p) ? 1 : 0);
      size_t r_num_0 = matrix.get_bost_size (step + bin_step_x2 * group_id);
      size_t r_num_last
          = matrix.get_bost_size (step + bin_step_x2 * group_id + bin_step);
      // if (flag)
      //   {
      //     size_t *index_map = group.get_index_map ();
      //     for (size_t i = 0; i < p; i++)
      //       {
      //         if (p_index == i /*&& group_id == 1*/)

      //           {
      //             printf (
      //                 "my_index = %ld, group_num = %ld, group_id = %ld, "
      //                 "group_size = %ld, index_in_group = %ld, local_r_index
      //                 "
      //                 "= %ld, r_num_0 = %ld, r_num_last = %ld\n",
      //                 p_index, group.get_group_num (), group_id, group_size,
      //                 index_in_group, local_r_index, r_num_0, r_num_last);
      //             for (size_t u = 0; u < group_size; u++)
      //               printf ("index_map[%ld] = %ld\n", u, index_map[u]);
      //             printf ("\n\n\n\n");
      //           }
      //         barier ();
      //       }
      //     // return;
      //   }

      if (index_in_group == 0)
        {
          for (size_t i = 1; i < group_size; i++)
            {
              find_diapazon (k - step, group_size, i, step);
              matrix.get_b_str (strings[0], local_r_index, start, end);
              group.send_message_i (i, strings[0], tag);
              // double *real_end = matrix.get_arr ()
              //                    + m_size * b_size * local_r_index
              //                    + matrix.get_local_bost_size
              //                    (local_r_index) * m_size;
              // double *string_end = strings[0].get_arr () +
              // strings[0].get_arr_size (); if (string_end > real_end)
              //   printf ("AAAAAAAAAAAAAAAAAAA k = %ld, start = %ld, end =
              //   %ld\n", k, start, end);
            }

          find_diapazon (k - step, group_size, index_in_group, step);
          matrix.get_b_str (strings[0], local_r_index, start, end);
          strings[1].set_params (get_bufer (), end - start, r_num_last,
                                 matrix.get_bost_size (end - 1));

          // if (!flag)
          group.recv_message (group_size - 1, strings[1], tag);
          strings[0].get_block (blocks[0], 0);
          strings[1].get_block (blocks[1], 0);
          rv.build_reset_reflection (blocks[0], blocks[1]);
        }
      else if (index_in_group == group_size - 1)
        {
          // if (!flag)
          for (size_t i = 0; i < group_size - 1; i++)
            {
              find_diapazon (k - step, group_size, i, step);
              matrix.get_b_str (strings[1], local_r_index, start, end);
              group.send_message (i, strings[1], tag);
            }

          find_diapazon (k - step, group_size, index_in_group, step);
          matrix.get_b_str (strings[1], local_r_index, start, end);
          strings[0].set_params (get_bufer (), end - start, r_num_0,
                                 matrix.get_bost_size (end - 1));
          group.recv_message (0, strings[0], tag);
        }
      else
        {
          find_diapazon (k - step, group_size, index_in_group, step);

          strings[0].set_params (get_bufer (), end - start, r_num_0,
                                 matrix.get_bost_size (end - 1));
          group.recv_message (0, strings[0], tag);

          strings[1].set_params (get_bufer () + strings[0].get_arr_size (),
                                 end - start, r_num_last,
                                 matrix.get_bost_size (end - 1));
          group.recv_message (group_size - 1, strings[1], tag);
        }

      double *rv_arr = rv.get_shift_arr ();
      size_t rv_arr_size = b_size * (r_num_last + 1);
      rv.set_vector_size (r_num_last + 1);

      group.broad_cast (rv_arr_size, rv_arr, 0, broadcast_tag ,flag);

      size_t str_start = 0, str_end = end - start;
      str_start += index_in_group == 0;
      rv.spread_reset_reflection (strings, str_start, str_end);
      rv.push (reflection_vectors::reflection_type::reset,
               step + bin_step_x2 * group_id + bin_step, step);
      // return;
      if (index_in_group == 0)
        {
          group.send_message (group_size - 1, strings[1], tag);
          for (size_t i = 1; i < group_size; i++)
            {
              find_diapazon (k - step, group_size, i, step);
              matrix.get_b_str (strings[0], local_r_index, start, end);
              group.recv_message (i, strings[0], tag);
            }
        }
      else if (index_in_group == group_size - 1)
        {
          group.send_message (0, strings[0], tag);
          for (size_t i = 0; i < group_size - 1; i++)
            {
              find_diapazon (k - step, group_size, i, step);
              matrix.get_b_str (strings[1], local_r_index, start, end);
              group.recv_message (i, strings[1], tag);
            }
        }
      else
        {
          group.send_message (0, strings[0], tag);
          group.send_message (group_size - 1, strings[1], tag);
        }
      bin_step <<= 1;
      bin_step_x2 <<= 1;
      if (count)
        bin_step *= 1;
      count++;
      if (flag)
        count++;
      if (flag)
        count--;
    }
}

void
application::triangulization ()
{
  size_t k = get_k ();
  size_t tag_step = get_active_process () * 2;
  size_t tag{};
  for (size_t step = 0; step < k; step++)
    {

      triangulization_local_part (step);

      triangulization_communicate_part (step, tag);
      tag = (tag + tag_step) % 32000;
      // if (step == 0)
      //   return;
    }
}
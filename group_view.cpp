#include "group_view.h"

execution_status
group_view::init_group_view (MPI_Comm &_comm, size_t p)
{
  comm = _comm;
  index_map = std::make_unique<size_t[]> (p);
}
size_t
group_view::get_group_id ()
{
  return group_id;
}

size_t
group_view::get_group_size ()
{
  return group_size;
}

size_t
group_view::get_index ()
{
  return index;
}

execution_status
group_view::send_message (size_t index, size_t message_size, size_t tag,
                          double *message)
{
  return mpi_message::send_message (comm, index_map[index], message_size, tag,
                                    message);
}

execution_status
group_view::recv_message (size_t index, size_t message_size, size_t tag,
                          double *message)
{
  return mpi_message::recv_message (comm, index_map[index], message_size, tag,
                                    message);
}

execution_status
group_view::send_message (size_t index, block_string &str, size_t tag)
{
  double *message = str.get_arr ();
  size_t message_size = str.get_arr_size ();
  return mpi_message::send_message (comm, index_map[index], message_size, tag,
                                    message);
}

execution_status
group_view::recv_message (size_t index, block_string &str, size_t tag)
{
  double *message = str.get_arr ();
  size_t message_size = str.get_arr_size ();
  return mpi_message::recv_message (comm, index_map[index], message_size, tag,
                                    message);
}

size_t
group_view::get_real_index_broad_cast (size_t index_in_group,
                                       size_t start_index)
{
  return (index_in_group >= start_index)
             ? index_in_group - start_index
             : group_size + index_in_group - start_index;
}

size_t
group_view::get_index_in_group_broad_cast (size_t real_index,
                                           size_t start_index)
{
  return (start_index + real_index) % group_size;
}

execution_status
group_view::broad_cast (size_t arr_size, double *arr, size_t send_index)
{
  size_t real_index = get_real_index_broad_cast (index, send_index);
  size_t start_index = 0;
  size_t bin = 0;
  while (start_index != real_index)
    {
      size_t diff = real_index - start_index;
      bin = 1;
      while (bin << 1 <= diff)
        bin <<= 1;
      start_index += bin;
    }
  if (bin)
    {
      size_t i = get_index_in_group_broad_cast (real_index - bin, send_index);
      mpi_message::recv_message (comm, index_map[i], arr_size, bin, arr);
      bin >>= 1;
      while (real_index + bin >= group_size)
        bin >>= 1;
    }
  else
    {
      bin = 1;
      while (bin << 1 < group_size)
        bin <<= 1;
    }

  while (bin)
    {
      size_t i = get_index_in_group_broad_cast (real_index + bin, send_index);
      mpi_message::send_message (comm, index_map[i], arr_size, bin, arr);
      bin >>= 1;
    }
  return execution_status::success;
}

void
group_view::create_group_for_triangulization (size_t start_index,
                                              size_t part_size,
                                              size_t bin_step)
{
  size_t bin_step_x2 = 2 * bin_step;
  int buf{};
  MPI_Comm_rank (comm, &buf);
  size_t my_index = size_t (buf);
  MPI_Comm_size (comm, &buf);
  size_t p = size_t (buf);
  size_t group_num = part_size / bin_step_x2;
  if (part_size % bin_step_x2 >= bin_step)
    group_num++;
  size_t index_in_part = (my_index >= start_index)
                             ? my_index - start_index
                             : my_index - start_index + p;
  group_id = index_in_part / bin_step_x2;
  size_t group_size = p / group_num;
  size_t ost = p % group_num;
  bool flag1 = true, flag2 = true;
  if (index_in_part % bin_step_x2 == 0)
    {
      if (group_id < group_num)
        {
          index = 0;
          if (group_id < ost)
            group_size++;
          flag1 = false;
        }
    }
  if (index_in_part % bin_step_x2 == bin_step)
    {

      if (group_id < group_size)
        {
          if (group_id < ost)
            group_size++;
          index = group_size - 1;
          flag2 = false;
        }
    }
  if (flag1 && flag2)
    {
      size_t real_index{};
      if (group_id < group_num)
        {
          real_index = index_in_part - 2 * group_id - 1;
          if (index_in_part % bin_step_x2 > bin_step)
            real_index--;
        }
      else
        real_index = index_in_part - 2 * group_num;
      size_t buf = (group_size - 1) * ost;
      if (buf >= real_index)
        {
          group_id = real_index / (group_size - 1);
          group_size++;
          index = real_index % (group_size - 1) + 1;
        }
      else
        {
          group_id = ost + (real_index - buf) / (group_size - 2);
          index = (real_index - buf) % (group_size - 2) + 1;
        }
    }
  index_map[0] = (bin_step_x2 * group_id + start_index) % p;
  index_map[group_size - 1]
      = (bin_step_x2 * group_id + bin_step + start_index) % p;
  if (group_size == 2)
    return;
  size_t real_index_start{};
  if (group_id < ost)
    real_index_start = (group_size - 1) * group_id;
  else
    real_index_start
        = (group_size - 1) * ost + (group_size - 2) * (group_id - ost);
  size_t last_index = bin_step_x2 * (group_num - 1) + bin_step;
  size_t real_index_last = last_index - group_num * 2;
  if (real_index_start >= real_index_last)
    {
      size_t ttt = last_index + (real_index_start - real_index_last);
      for (size_t i = 1; i < group_size - 1; i++)
        index_map[i] = (start_index + ttt++) % p;
      return;
    }
  buf = real_index_last - real_index_start;
  size_t k = buf / (bin_step_x2 - 2);
  size_t ost = buf % (bin_step_x2 - 2);
  if (ost)
    k++;
  size_t ttt = last_index - bin_step_x2 * k;
  ost = bin_step_x2 - ost;
  ttt += ost + ost > bin_step;
  size_t i = 1;
  while (i != group_size - 1)
    {
      size_t ost = ttt % bin_step_x2;
      if ((ost == 0 || ost == bin_step) && ttt / bin_step_x2 < group_num)
        {
          ttt++;
          continue;
        }
      index_map[i++] = (start_index + ttt++) % p;
    }
}

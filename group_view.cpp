#include "group_view.h"

size_t *
group_view::get_index_map ()
{
  return index_map.get ();
}
execution_status
group_view::init_group_view (MPI_Comm &_comm, size_t p)
{
  comm = _comm;
  index_map = std::make_unique<size_t[]> (p);
  requests = std::make_unique<MPI_Request[]> (p);
  statuses = std::make_unique<MPI_Status[]> (p);

  return execution_status::success;
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

execution_status
group_view::send_message_i (size_t index, block_string &str, size_t tag)
{
  double *message = str.get_arr ();
  size_t message_size = str.get_arr_size ();
  return mpi_message::send_message_i (comm, index_map[index], message_size,
                                      tag, message, requests[req_size++]);
}

execution_status
group_view::recv_message_i (size_t index, block_string &str, size_t tag)
{
  double *message = str.get_arr ();
  size_t message_size = str.get_arr_size ();
  return mpi_message::recv_message_i (comm, index_map[index], message_size,
                                      tag, message, requests[req_size++]);
}

execution_status
group_view::wait_all (size_t size)
{
  if (size > req_size)
    {
      printf ("group_view::wait_all:: size > real_req_size\n");
      return execution_status::runtime_error;
    }
  req_size = 0;
  return mpi_message::wait_all (size, requests.get (), statuses.get ());
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
group_view::broad_cast (size_t arr_size, double *arr, size_t send_index,
                        size_t tag, bool flag)
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
      mpi_message::recv_message (comm, index_map[i], arr_size, tag, arr);
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
      mpi_message::send_message_i (comm, index_map[i], arr_size, tag, arr,
                                   requests[i]);
      bin >>= 1;
    }
  return execution_status::success;
  if (flag)
    bin++;
}
size_t
group_view::get_group_num ()
{
  return group_num;
}
void
group_view::create_group_for_triangulization (size_t start_index,
                                              size_t part_size,
                                              size_t bin_step, bool flag)
{

  size_t bin_step_x2 = 2 * bin_step;
  int buf{};
  MPI_Comm_rank (comm, &buf);
  size_t my_index = size_t (buf);
  MPI_Comm_size (comm, &buf);
  size_t p = size_t (buf);

  group_num = part_size / bin_step_x2;
  if (part_size % bin_step_x2 > bin_step)
    group_num++;

  size_t index_in_part = (my_index >= start_index)
                             ? my_index - start_index
                             : my_index - start_index + p;
  group_id = index_in_part / bin_step_x2;
  group_size = p / group_num;
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
      // if (flag && group_id == 1)
      //   printf ("kaka\n");
      if (group_id < group_num)
        {
          if (group_id < ost)
            group_size++;
          index = group_size - 1;
          flag2 = false;
        }
      // if (flag)
      //   printf ("AAAAAAAAAAAAAAAAAAAA group_id = %ld\n", group_id);
    }
  // if (flag && group_id == 1)
  //   printf ("group_size = %ld\n", group_size);
  if (flag1 && flag2)
    {
      size_t real_index = index_in_part;
      size_t fictive_group_id = index_in_part / bin_step_x2;
      if (fictive_group_id < group_num)
        real_index -= fictive_group_id * 2 + 1
                      + (index_in_part % bin_step_x2 > bin_step);
      else
        real_index -= 2 * group_num;
      size_t buf = (group_size - 1) * ost;
      if (buf > real_index)
        {
          group_id = real_index / (group_size - 1);
          index = real_index % (group_size - 1) + 1;
          group_size++;
          // if (flag && group_id == 1)
          //   printf (
          //       "my_index = %ld index_in_part = %ld index = %ld real_index =
          //       "
          //       "%ld buf = %ld group_size = %ld\n",
          //       my_index, index_in_part, index, real_index, buf,
          //       group_size);
        }
      else
        {
          group_id = ost + (real_index - buf) / (group_size - 2);
          index = (real_index - buf) % (group_size - 2) + 1;
        }
    }
  // if (flag)
  //   printf ("my_index = %ld, p = %ld, start_index = %ld, part_size = %ld, "
  //           "bin_step = %ld, group_num = %ld, group_id = %ld index_in_group
  //           = "
  //           "%ld, index_in_part = %ld\n",
  //           my_index, p, start_index, part_size, bin_step, group_num,
  //           group_id, index, index_in_part);

  index_map[0] = (bin_step_x2 * group_id + start_index) % p;
  index_map[group_size - 1]
      = (bin_step_x2 * group_id + bin_step + start_index) % p;

  if (group_size == 2)
    return;
  size_t real_index_start{};
  if (group_id < ost)
    real_index_start = (group_size - 2) * group_id;
  else
    real_index_start
        = (group_size - 1) * ost + (group_size - 2) * (group_id - ost);

  size_t real_step = bin_step_x2 - 2;
  if (real_step == 0)
    {
      size_t last_index = bin_step_x2 * (group_num - 1) + bin_step + 1;
      size_t real_index_last = last_index - group_num * 2;
      size_t ttt = last_index + (real_index_start - real_index_last);
      // if (group_id == 1 && flag)
      //   printf ("last_index = %ld, real_last_index = %ld, real_index_start =
      //   "
      //           "%ld, ttt = %ld\n",
      //           last_index, real_index_last, real_index_start, ttt);
      for (size_t i = 1; i < group_size - 1; i++)
        index_map[i] = (start_index + ttt++) % p;
      return;
    }
  size_t kkk = real_index_start / real_step;
  size_t rrr = real_index_start / real_step;
  size_t ttt = bin_step_x2 * kkk + 1 + rrr + (1 + rrr >= bin_step);
  if (flag)
    kkk++;
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

#include "mpi_group.h"
#include <algorithm>

mpi_group::mpi_group (MPI_Comm main_comm, int color)
{
  init_mpi_group (main_comm, color);
}

mpi_group::~mpi_group () { free_mpi (); }

void
mpi_group::free_mpi ()
{
  int initialized;
  MPI_Initialized (&initialized);

  if (!initialized)
    {
      comm = MPI_COMM_NULL;
      return;
    }

  if (comm != MPI_COMM_NULL && comm != MPI_COMM_WORLD)
    {
      MPI_Comm_free (&comm);
    }
  comm = MPI_COMM_NULL;
}

MPI_Comm &
mpi_group::get_comm ()
{
  return comm;
}
void
mpi_group::init_mpi_group (MPI_Comm &main_comm, int color)
{
  int buf = 0;
  MPI_Comm_size (main_comm, &buf);
  total_process = size_t (buf);
  MPI_Comm_rank (main_comm, &buf);
  process_index = size_t (buf);
  MPI_Comm buf_comm{};
  MPI_Comm_split (main_comm, color, process_index, &buf_comm);
  if (comm != MPI_COMM_NULL && comm != MPI_COMM_WORLD)
    {
      MPI_Comm_free (&comm);
      comm = MPI_COMM_NULL;
    }
  active_process_flag = color != MPI_UNDEFINED;
  if (!active_process_flag)
    return;
  comm = buf_comm;
  MPI_Comm_size (comm, &buf);
  total_process = size_t (buf);
  MPI_Comm_rank (comm, &buf);
  process_index = size_t (buf);
  active_process = total_process;
}

void
mpi_group::split (int color)
{
  int buf{};
  MPI_Comm buf_comm{};
  MPI_Comm_split (comm, color, process_index, &buf_comm);
  if (comm != MPI_COMM_NULL && comm != MPI_COMM_WORLD)
    {
      MPI_Comm_free (&comm);
      comm = MPI_COMM_NULL;
    }

  active_process_flag = color != MPI_UNDEFINED;
  if (!active_process_flag)
    return;
  comm = buf_comm;
  MPI_Comm_size (comm, &buf);
  total_process = size_t (buf);
  MPI_Comm_rank (comm, &buf);
  process_index = size_t (buf);
  active_process = total_process;
}

bool
mpi_group::active ()
{
  return active_process_flag;
}

bool
mpi_group::inactive ()
{
  return !active_process_flag;
}

size_t
mpi_group::get_process_index () const
{
  return process_index;
}

size_t
mpi_group::get_total_process () const
{
  return total_process;
}

size_t
mpi_group::get_active_process () const
{
  return active_process;
}

bool
mpi_group::has_error (execution_status status)
{
  int res = 0;
  MPI_Allreduce (&status, &res, 1, MPI_INT, MPI_MAX, comm);
  return res;
}

void
mpi_group::reduce_sum_arr_all (double *producer, double *consumer, size_t size)
{
  MPI_Allreduce (producer, consumer, int (size), MPI_DOUBLE, MPI_SUM, comm);
}

void
mpi_group::reduce_sum_arr (size_t recv_index, double *producer,
                           double *consumer, size_t size)
{
  MPI_Reduce (producer, consumer, static_cast<int> (size), MPI_DOUBLE, MPI_SUM,
              static_cast<int> (recv_index), comm);
}

execution_status
mpi_group::send_message (size_t index, size_t message_size, size_t tag,
                         double *message)
{
  return mpi_message::send_message (comm, index, message_size, tag, message);
}

execution_status
mpi_group::recv_message (size_t index, size_t message_size, size_t tag,
                         double *message)
{
  return mpi_message::recv_message (comm, index, message_size, tag, message);
}

void
mpi_group::barier ()
{
  MPI_Barrier (comm);
}

execution_status
mpi_group::broadcast (size_t send_index, size_t arr_size, double *arr)
{
  return mpi_message::broadcast (comm, send_index, arr_size, arr);
}
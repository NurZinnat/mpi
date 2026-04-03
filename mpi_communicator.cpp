#include "mpi_communicator.h"

execution_status
mpi_communicator::init_mpi_communicator (size_t k, size_t size)
{

  int buf = 0;
  MPI_Comm_size (MPI_COMM_WORLD, &buf);
  total_process = size_t (buf);
  MPI_Comm_rank (MPI_COMM_WORLD, &buf);
  process_index = size_t (buf);
  int color = 0;
  if (process_index >= k)
    {
      color = MPI_UNDEFINED;
      active_process_flag = false;
    }
  active_process = std::min (total_process, k);
  MPI_Comm_split (MPI_COMM_WORLD, color, process_index, &comm);
  return data_memory_allocate (size);
}

void
mpi_communicator::default_init_mpi_communicator ()
{
  int buf = 0;
  MPI_Comm_size (MPI_COMM_WORLD, &buf);
  total_process = size_t (buf);
  MPI_Comm_rank (MPI_COMM_WORLD, &buf);
  process_index = size_t (buf);
  active_process = total_process;
  comm = MPI_COMM_WORLD;
}

bool
mpi_communicator::active ()
{
  return active_process_flag;
}

bool
mpi_communicator::inactive ()
{
  return !active_process_flag;
}

size_t
mpi_communicator::get_process_index () const
{
  return process_index;
}

size_t
mpi_communicator::get_total_process () const
{
  return total_process;
}

size_t
mpi_communicator::get_active_process () const
{
  return active_process;
}

void
mpi_communicator::reduce_sum_arr (double *producer, double *consumer,
                                  size_t size)
{
  MPI_Allreduce (producer, consumer, int (size), MPI_DOUBLE, MPI_SUM, comm);
}

bool
mpi_communicator::has_error (execution_status status)
{
  int res = 0;
  int prod = int (status);
  MPI_Allreduce (&prod, &res, 1, MPI_INT, MPI_MAX, comm);
  return res;
}

double *
mpi_communicator::get_bufer ()
{
  return get_arr ();
}

execution_status
mpi_communicator::send_message (size_t index, size_t message_size, size_t tag,
                                double *message)
{
  MPI_Send (message, message_size, MPI_DOUBLE, int (index), int (tag), comm);
  return execution_status::success;
}

execution_status
mpi_communicator::recv_message (size_t index, size_t message_size, size_t tag,
                                double *message)
{
  if (!message)
    message = get_arr ();
  MPI_Recv (message, message_size, MPI_DOUBLE, index, tag, comm,
            &recv_request);
  return execution_status::success;
}
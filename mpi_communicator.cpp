#include "mpi_communicator.h"

execution_status
mpi_communicator::init_mpi_communicator (size_t k, size_t size)
{

  int buf = 0;
  MPI_Comm_size (MPI_COMM_WORLD, &buf);
  size_t total_process = size_t (buf);
  MPI_Comm_rank (MPI_COMM_WORLD, &buf);
  size_t process_index = size_t (buf);
  int color = (process_index < k) ? 0 : MPI_UNDEFINED;
  init_mpi_group (MPI_COMM_WORLD, color);
  if (inactive ())
    return execution_status::success;
  return data_memory_allocate (size);
}
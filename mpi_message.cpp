#include "mpi_message.h"

execution_status
mpi_message::send_message (MPI_Comm &comm, size_t index, size_t message_size,
                           size_t tag, double *message)
{
  MPI_Send (message, message_size, MPI_DOUBLE, int (index), int (tag), comm);
  return execution_status::success;
}

execution_status
mpi_message::recv_message (MPI_Comm &comm, size_t index, size_t message_size,
                           size_t tag, double *message)
{
  MPI_Status recv_request{};
  MPI_Recv (message, message_size, MPI_DOUBLE, index, tag, comm,
            &recv_request);
  return execution_status::success;
}
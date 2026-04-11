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

  int actual_count;
  MPI_Get_count (&recv_request, MPI_DOUBLE, &actual_count);

  if (actual_count != (int)message_size)
    {
      printf ("ERROR: recv_message expected %ld, got %d from rank %ld\n",
              message_size, actual_count, index);
    }

  return execution_status::success;
}

execution_status
mpi_message::broadcast (MPI_Comm &comm, size_t send_index, size_t arr_size,
                        double *arr)
{
  MPI_Bcast (arr, arr_size, MPI_DOUBLE, send_index, comm);
  return execution_status::success;
}
#ifndef MPI_MESSAGE_H
#define MPI_MESSAGE_H
#include <mpi.h>

#include "execution_status.h"
#include <stdlib.h>

class mpi_message
{
public:
  mpi_message () = default;
  ~mpi_message () = default;
  mpi_message (const mpi_message &x) = delete;
  mpi_message (mpi_message &&x) = delete;
  mpi_message &operator= (const mpi_message &x) = delete;
  mpi_message &operator= (mpi_message &&x) = delete;
  execution_status send_message (MPI_Comm &comm, size_t index,
                                 size_t message_size, size_t tag,
                                 double *message);
  execution_status recv_message (MPI_Comm &comm, size_t index,
                                 size_t message_size, size_t tag,
                                 double *message);
  execution_status broadcast (MPI_Comm &comm, size_t send_index,
                              size_t arr_size, double *arr);
};

#endif // MPI_MESSAGE_H
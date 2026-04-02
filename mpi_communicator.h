#ifndef MPI_COMMUNICATOR_H
#define MPI_COMMUNICATOR_H
#include <mpi.h>

#include "data.h"
#include "execution_status.h"
#include <memory>
#include <stdlib.h>

class mpi_communicator : private data
{
  size_t process_index{};
  size_t total_process{};
  size_t active_process{};
  MPI_Comm comm{};
  MPI_Request send_request;
  MPI_Status recv_request;
  bool active_process_flag = true;

public:
  mpi_communicator () = default;
  ~mpi_communicator () = default;
  mpi_communicator (const mpi_communicator &x) = delete;
  mpi_communicator (mpi_communicator &&x) = delete;
  mpi_communicator &operator= (const mpi_communicator &x) = delete;
  mpi_communicator &operator= (mpi_communicator &&x) = delete;
  execution_status send_message (size_t index, size_t message_size, size_t tag,
                                 double *message);
  execution_status recv_message (size_t index, size_t message_size, size_t tag,
                                 double *message = nullptr);
  execution_status init_mpi_communicator (size_t k, size_t size);
  void default_init_mpi_communicator ();

  size_t get_process_index () const;
  size_t get_total_process () const;
  size_t get_active_process () const;
  bool active ();
  bool inactive ();
  void reduce_sum_arr (double *producer, double *consumer, size_t size);
  bool has_error (execution_status status);
  double *get_bufer ();
};

#endif // MPI_COMMUNICATOR_H
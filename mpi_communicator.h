#ifndef MPI_COMMUNICATOR_H
#define MPI_COMMUNICATOR_H
#include "mpi_group.h"

#include "data.h"
#include "execution_status.h"
#include <memory>

class mpi_communicator : private data, public mpi_group
{
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
  size_t get_process_index () const;
  size_t get_total_process () const;
  size_t get_active_process () const;
  void reduce_sum_arr (double *producer, double *consumer, size_t size);
  bool has_error (execution_status status);
  double *get_bufer ();
};

#endif // MPI_COMMUNICATOR_H
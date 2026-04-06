#ifndef MPI_GROUP_H
#define MPI_GROUP_H
#include "mpi_message.h"

#include "execution_status.h"
#include <stdlib.h>
class mpi_group : private mpi_message
{
  MPI_Comm comm{};
  size_t total_process{};
  size_t active_process{};
  size_t process_index{};
  bool active_process_flag{};

public:
  mpi_group () = default;
  mpi_group (MPI_Comm main_comm, int color);
  ~mpi_group ();
  mpi_group (const mpi_group &x) = delete;
  mpi_group (mpi_group &&x) = delete;
  mpi_group &operator= (const mpi_group &x) = delete;
  mpi_group &operator= (mpi_group &&x) = delete;

  void init_mpi_group (MPI_Comm main_comm, int color);
  size_t get_process_index () const;
  size_t get_total_process () const;
  size_t get_active_process () const;
  bool active ();
  bool inactive ();
  bool has_error (execution_status status);
  void reduce_sum_arr (double *producer, double *consumer, size_t size);
  execution_status send_message (size_t index, size_t message_size, size_t tag,
                                 double *message);
  execution_status recv_message (size_t index, size_t message_size, size_t tag,
                                 double *message);
};

#endif // MPI_GROUP_H
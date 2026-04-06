#ifndef GROUP_VIEW_H
#define GROUP_VIEW_H
#include "mpi_message.h"

#include <memory>
#include <stdlib.h>
class group_view : private mpi_message
{
  size_t group_id{};
  size_t group_size{};
  size_t process_index{};

  MPI_Comm comm{};
  std::unique_ptr<size_t[]> index_map{};
  size_t index_map_size{};

public:
  group_view () = default;
  ~group_view () = default;
  group_view (const group_view &x) = delete;
  group_view (group_view &&x) = delete;
  group_view &operator= (const group_view &x) = delete;
  group_view &operator= (group_view &&x) = delete;

  size_t get_group_id ();
  size_t get_group_size ();
  size_t get_process_index ();
  void create_group_for_triangulization (MPI_Comm &_comm, size_t start_index,
                                         size_t part_size, size_t bin_step);
  execution_status send_message (size_t index,
                                 size_t message_size, size_t tag,
                                 double *message);
  execution_status recv_message (size_t index,
                                 size_t message_size, size_t tag,
                                 double *message);
};
#endif // GROUP_VIEW_H
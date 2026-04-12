#ifndef GROUP_VIEW_H
#define GROUP_VIEW_H
#include "mpi_message.h"

#include "block_string.h"
#include <memory>
#include <stdlib.h>
class group_view : private mpi_message
{
  size_t group_id{};
  size_t group_size{};
  size_t index{};
  size_t group_num{};

  MPI_Comm comm{};
  size_t req_size{};
  std::unique_ptr<size_t[]> index_map{};
  std::unique_ptr<MPI_Request[]> requests{};
  std::unique_ptr<MPI_Status[]> statuses{};

public:
  group_view () = default;
  ~group_view () = default;
  group_view (const group_view &x) = delete;
  group_view (group_view &&x) = delete;
  group_view &operator= (const group_view &x) = delete;
  group_view &operator= (group_view &&x) = delete;
  execution_status init_group_view (MPI_Comm &_comm, size_t p);

  size_t get_group_id ();
  size_t get_group_size ();
  size_t get_index ();
  size_t get_group_num ();
  void create_group_for_triangulization (size_t start_index, size_t part_size,
                                         size_t bin_step, bool flag = false);
  execution_status send_message (size_t index, size_t message_size,
                                 size_t tag, double *message);
  execution_status recv_message (size_t index, size_t message_size,
                                 size_t tag, double *message);
  execution_status send_message (size_t index, block_string &str, size_t tag);
  execution_status recv_message (size_t index, block_string &str, size_t tag);
  execution_status send_message_i (size_t index, block_string &str, size_t tag);
  execution_status recv_message_i (size_t index, block_string &str, size_t tag);
  execution_status broad_cast (size_t arr_size, double *arr,
                               size_t send_index, size_t tag,  bool flag = false);
  size_t get_real_index_broad_cast (size_t index_in_group, size_t start_index);
  size_t get_index_in_group_broad_cast (size_t real_index, size_t start_index);
  size_t *get_index_map ();
  execution_status wait_all (size_t size);
};
#endif // GROUP_VIEW_H
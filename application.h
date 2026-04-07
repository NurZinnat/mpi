#ifndef APPLICATION_H
#define APPLICATION_H
#include "mpi_communicator.h"

#include "cmd_arg_parser.h"
#include "matrix_part.h"
#include "reflection_vectors.h"

class application : private m_sizes,
                    private mpi_communicator,
                    private cmd_arg_parser
{
  matrix_part matrix{};
  matrix_part sub_matrix{};
  block_view blocks[3];
  reflection_vectors rv{};

  execution_status status{};
  size_t print_size{};

  double norm{};

  char *prg_name{};
  size_t m_size{};
  size_t b_size{};
  size_t r{};
  size_t s{};
  char *file_name{};

  std::unique_ptr<double[]> bufer_1{};
  std::unique_ptr<double[]> bufer_2{};

public:
  application () = default;
  ~application () = default;
  application (const application &x) = delete;
  application (application &&x) = delete;
  application &operator= (const application &x) = delete;
  application &operator= (application &&x) = delete;

  execution_status init_application ();
  execution_status read_matrix ();
  void norm_calculate ();
  void print_matrix ();
  void print_transpozition_matrix ();
  void init_norm ();
  execution_status cmd_arg_parsing (size_t argc, char *argv[]);
  void application_main (size_t argc, char *argv[]);

  void build_triangular_reflection (size_t local_r_index, size_t c_index);
  void spread_triangular_reflection (size_t local_r_index, size_t c_start,
                                     size_t c_end);
  void build_reset_reflection (size_t local_r_index, size_t sub_local_r_index,
                               size_t c_index);
  void spread_reset_reflection (size_t local_r_index, size_t sub_local_r_index,
                                size_t c_start, size_t c_end);
  void triangulization ();
  void triangulization_local_part (size_t step);
  void triangulization_communicate_part (size_t step);

  size_t calculate_global_b_row_index (size_t local_b_row_index);
  size_t calculate_local_b_row_index (size_t global_b_row_index);
};

#endif // APPLICATION_H
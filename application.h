#ifndef APPLICATION_H
#define APPLICATION_H
#include "mpi_group.h"

#include "cmd_arg_parser.h"
#include "group_view.h"
#include "matrix_part.h"
#include "reflection_vectors.h"

class application : private m_sizes,
                    private mpi_group,
                    private cmd_arg_parser,
                    private data
{
  matrix_part matrix{};
  matrix_part sub_matrix{};
  reflection_vectors rv{};
  block_string strings[2];
  block_view blocks[3];

  execution_status status{};
  size_t print_size{};

  group_view group{};

  double norm{};

  char *prg_name{};
  size_t m_size{};
  size_t b_size{};
  size_t r{};
  size_t s{};
  char *file_name{};

  size_t start{};
  size_t end{};

  std::unique_ptr<double[]> bufer_1{};
  std::unique_ptr<double[]> bufer_2{};
  std::unique_ptr<double[]> for_blocks{};

  double r1{};
  double r2{};

  double t1{};
  double t2{};

  int n{};
  int m{};
  int ppppp{};

public:
  application () = default;
  ~application () = default;
  application (const application &x) = delete;
  application (application &&x) = delete;
  application &operator= (const application &x) = delete;
  application &operator= (application &&x) = delete;

  execution_status init_application ();
  execution_status read_matrix ();
  execution_status read_sub_matrix ();
  void norm_calculate ();
  void print_matrix ();
  void print_sub_matrix ();

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
  void triangulization_communicate_part (size_t step, size_t step_tag);

  size_t calculate_global_b_row_index (size_t local_b_row_index);
  size_t calculate_local_b_row_index (size_t global_b_row_index);

  double *get_bufer ();
  void find_diapazon (size_t len, size_t p, size_t index, size_t shift);
  void free_mpi ();
  void calculate_left_discrepancy ();
  void calculate_right_discrepancy ();
  void multiply_strings (block_string *strings, block_view *blocks);
  void print_results ();
};

#endif // APPLICATION_H
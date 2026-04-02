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
  matrix_part matrix_p[2];
  block blocks[3];
  reflection_vectors rv{};
  reflection_vectors_storage rv_storage{};

  execution_status status{};
  size_t print_size{};

  double norm{};

  char *prg_name{};
  size_t m_size{};
  size_t b_size{};
  size_t r{};
  size_t s{};
  char *file_name{};

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
};

#endif // APPLICATION_H
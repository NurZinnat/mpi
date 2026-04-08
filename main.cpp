#include "application.h"

int
main (int argc, char *argv[])
{
  MPI_Init (&argc, &argv);
  application app{};
  app.application_main (size_t (argc), argv);
  app.free_mpi ();
  MPI_Finalize ();
  return 0;
}
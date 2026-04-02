#include "application.h"
void
application::application_main (size_t argc, char *argv[])
{
  cmd_arg_parsing (argc, argv);
  if (has_error (status))
    return;

  init_application ();

  if (inactive ())
    return;
  if (has_error (status))
    return;
  
  read_matrix ();
  return;

  if (has_error (status))
    return;

  print_matrix ();
  printf ("\n");
  print_transpozition_matrix ();
}
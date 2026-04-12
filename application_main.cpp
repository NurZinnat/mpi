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
  if (has_error (status))
    return;

  //print_matrix ();

  norm_calculate ();

  if (get_process_index () == 0)
    printf ("\n\n\nnorm = %lf\n\n\n", norm);

  init_norm ();
  
  triangulization ();
  //print_matrix ();
  //return;

  read_sub_matrix ();
  if (has_error (status))
    return;
  //calculate_left_discrepancy ();
  // calculate_right_discrepancy ();
  // if (get_process_index () == 0)
  //   printf ("\n\n\n");
  print_matrix ();

  if (get_process_index () == 0)
    {
     // printf ("\n\n\n\n");
      print_results ();
    }
}
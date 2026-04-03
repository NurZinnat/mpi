#include "application.h"
#include "simple_functions.h"

execution_status
application::init_application ()
{
  init_m_sizes (m_size, b_size);
  init_mpi_communicator (get_k (), m_size * b_size);
  if (inactive ())
    {
      status = execution_status::inactive_process;
      return status;
    }
  print_size = r;
  for (size_t i = 0; i < 2; i++)
    {
      status = matrix_p[i].init_matrix_part (
          m_size, b_size, get_process_index (), get_active_process ());
      if (status != execution_status::success)
        return status;
    }
  rv_storage.set_reflection_storage (matrix_p[1].get_arr ());
  for (size_t i = 0; i < 3; i++)
    {
      status = blocks[i].init_block (m_size, b_size, 0);
      if (status != execution_status::success)
        return status;
    }
  status = rv.init_reflection_vectors (b_size, 0);
  return status;
}

execution_status
application::read_matrix ()
{
  if (s > 0)
    {
      matrix_p[0].init_by_formula (s);
      return execution_status::success;
    }
  size_t m_size = get_m_size ();
  size_t k = get_k ();
  size_t p = get_active_process ();
  size_t process_index = get_process_index ();
  FILE *fp = nullptr;
  if (process_index == 0)
    {
      fp = fopen (file_name, "r");
      if (!fp)
        status = execution_status::open;
    }
  if (has_error (status))
    {
      status = execution_status::undefined_error;
      return status;
    }
  if (process_index == 0)
    for (size_t b_row_index = 0; b_row_index < k; b_row_index++)
      {

        size_t consumer_process_index = b_row_index % p;

        if (consumer_process_index == 0)
          {
            double *bufer = matrix_p[0].get_b_row_pointer (b_row_index / p);
            size_t size{};
            status = matrix_p[0].read_b_row_from_file_to_arr (b_row_index,
                                                              size, bufer, fp);
            if (has_error (status))
              {
                status = execution_status::undefined_error;
                // printf ("kaka\n");
                fclose (fp);
                return status;
              }
          }
        else
          {
            double *message = get_bufer ();
            size_t size{};
            status = matrix_p[0].read_b_row_from_file_to_arr (
                b_row_index, size, message, fp);
            if (has_error (status))
              {
                status = execution_status::undefined_error;
                return status;
              }
            send_message (consumer_process_index, size, b_row_index, message);
          }
      }
  else
    for (size_t b_row_index = 0; b_row_index < k; b_row_index++)
      {
        if (has_error (status))
          {
            status = execution_status::undefined_error;
            return status;
          }
        if (b_row_index % p != process_index)
          continue;
        size_t rb_size = matrix_p[0].get_bost_size (b_row_index);
        size_t size = rb_size * m_size;
        double *b_row = matrix_p[0].get_b_row_pointer (b_row_index / p);
        recv_message (0, size, b_row_index, b_row);
      }
  if (process_index == 0)
    fclose (fp);
  return execution_status::success;
}

void
application::norm_calculate ()
{
  size_t k = get_k ();
  double *producer = blocks[0].get_arr ();
  double *consumer = blocks[1].get_arr ();
  for (size_t i = 0; i < k; i++)
    {
      blocks[0].zero_padding ();
      blocks[1].zero_padding ();
      size_t size = matrix_p[0].calculate_norm_part (i, producer);
      reduce_sum_arr (producer, consumer, size);
      norm = std::max (norm, max_from_array (size, consumer));
    }
}

void
application::print_matrix ()
{
  size_t process_index = get_process_index ();
  size_t p = get_active_process ();
  size_t b_size = get_b_size ();
  if (process_index == 0)
    for (size_t row_index = 0; row_index < print_size; row_index++)
      {
        size_t global_b_row_index = row_index / b_size;
        size_t source = global_b_row_index % p;
        double *print_arr = get_bufer ();
        if (source == 0)
          matrix_p[0].get_row_part (row_index, print_size, print_arr);
        else
          recv_message (source, print_size, row_index);
        print_array (print_size, print_arr);
      }
  else
    for (size_t row_index = 0; row_index < print_size; row_index++)
      {
        size_t global_b_row_index = row_index / b_size;
        size_t source = global_b_row_index % p;
        if (source != process_index)
          continue;
        double *bufer = get_bufer ();
        matrix_p[0].get_row_part (row_index, print_size, bufer);
        send_message (0, print_size, row_index, bufer);
      }
  if (process_index == 0)
    printf ("\n\n");
}

void
application::print_transpozition_matrix ()
{
  size_t process_index = get_process_index ();
  size_t p = get_active_process ();
  size_t b_size = get_b_size ();
  if (process_index == 0)
    for (size_t col_index = 0; col_index < print_size; col_index++)
      {
        size_t stop = print_size / b_size + 1;
        double *print_arr = get_bufer ();
        double *buf_arr = print_arr;
        for (size_t b_row_index = 0; b_row_index < stop; b_row_index++)
          {
            size_t source = b_row_index % p;
            if (source == 0)
              matrix_p[0].get_col_part (col_index, b_row_index, buf_arr);
            else
              {
                size_t rb_size = matrix_p[0].get_bost_size (b_row_index);
                recv_message (source, rb_size, col_index, buf_arr);
              }
            buf_arr += b_size;
          }
        print_array (print_size, print_arr);
      }
  else
    for (size_t col_index = 0; col_index < print_size; col_index++)
      {
        size_t stop = print_size / b_size + 1;
        double *bufer = get_bufer ();
        for (size_t b_row_index = 0; b_row_index < stop; b_row_index++)
          {
            size_t source = b_row_index % p;
            if (source != process_index)
              continue;
            matrix_p[0].get_col_part (col_index, b_row_index, bufer);
            size_t rb_size = matrix_p[0].get_bost_size (b_row_index);
            send_message (0, rb_size, col_index, bufer);
          }
      }
}

void
application::init_norm ()
{
  for (size_t i = 0; i < 2; i++)
    matrix_p[i].set_norm (norm);
  for (size_t i = 0; i < 3; i++)
    blocks[i].set_eps (norm);
  rv.set_eps (norm);
}

execution_status
application::cmd_arg_parsing (size_t argc, char *argv[])
{
  default_init_mpi_communicator ();
  if (argc < 5 || argc > 6)
    {
      status = execution_status::cmd_parse_error;
      return status;
    }
  prg_name = argv[0];
  parse_size_t (argv[1], m_size);
  if (get_parse_status () != cmd_arg_parser::cmd_arg_parsing_status::succsess)
    {
      status = execution_status::cmd_parse_error;
      return status;
    }
  parse_size_t (argv[2], b_size);
  if (get_parse_status () != cmd_arg_parser::cmd_arg_parsing_status::succsess)
    {
      status = execution_status::cmd_parse_error;
      return status;
    }
  parse_size_t (argv[3], r);
  if (get_parse_status () != cmd_arg_parser::cmd_arg_parsing_status::succsess)
    {
      status = execution_status::cmd_parse_error;
      return status;
    }
  parse_size_t (argv[4], s);
  if (get_parse_status () != cmd_arg_parser::cmd_arg_parsing_status::succsess)
    {
      status = execution_status::cmd_parse_error;
      return status;
    }
  if (argc > 5)
    file_name = argv[5];
  if (r > m_size)
    r = m_size;
  if (b_size > m_size)
    b_size = m_size;
  if (m_size == 0 || b_size == 0 || s > 4)
    status = execution_status::cmd_parse_error;
  return status;
}

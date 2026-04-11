#include "application.h"
#include "simple_functions.h"
void
application::free_mpi ()
{
  mpi_group::free_mpi ();
}

execution_status
application::init_application ()
{
  init_m_sizes (m_size, b_size);
  size_t k = get_k ();
  size_t process_index = get_process_index ();
  int color = 0;
  if (process_index >= k)
    color = MPI_UNDEFINED;
  split (color);
  if (inactive ())
    {
      status = execution_status::inactive_process;
      return status;
    }
  status = data_memory_allocate (m_size * b_size + b_size * b_size);
  if (status != execution_status::success)
    return status;
  status = group.init_group_view (get_comm (), get_active_process ());
  if (status != execution_status::success)
    return status;

  status = matrix.init_matrix_part (m_size, b_size, get_process_index (),
                                    get_active_process ());
  if (status != execution_status::success)
    return status;
  status = sub_matrix.init_matrix_part (m_size, b_size, get_process_index (),
                                        get_active_process ());
  if (status != execution_status::success)
    return status;

  status = rv.init_reflection_vectors (m_size, b_size, 0);
  if (status != execution_status::success)
    return status;
  rv.set_arr (sub_matrix.get_arr ());
  print_size = r;
  bufer_1 = std::make_unique<double[]> (b_size);
  bufer_2 = std::make_unique<double[]> (b_size);
  for_blocks = std::make_unique<double[]> (b_size * b_size * 2);

  for (size_t i = 0; i < 2; i++)
    strings[i].init_m_sizes (m_size, b_size);
  for (size_t i = 0; i < 3; i++)
    blocks[i].init_m_sizes (m_size, b_size);

  return status;
}

execution_status
application::read_matrix ()
{
  if (s > 0)
    {
      matrix.init_by_formula (s);
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
            double *bufer = matrix.get_b_row_pointer (b_row_index / p);
            size_t size{};
            status = matrix.read_b_row_from_file_to_arr (b_row_index, size,
                                                         bufer, fp);
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
            status = matrix.read_b_row_from_file_to_arr (b_row_index, size,
                                                         message, fp);
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
        size_t rb_size = matrix.get_bost_size (b_row_index);
        size_t size = rb_size * m_size;
        double *b_row = matrix.get_b_row_pointer (b_row_index / p);
        recv_message (0, size, b_row_index, b_row);
      }
  if (process_index == 0)
    fclose (fp);
  return execution_status::success;
}

execution_status
application::read_sub_matrix ()
{
  if (s > 0)
    {
      sub_matrix.init_by_formula (s);
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
            double *bufer = sub_matrix.get_b_row_pointer (b_row_index / p);
            size_t size{};
            status = sub_matrix.read_b_row_from_file_to_arr (b_row_index, size,
                                                             bufer, fp);
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
            status = sub_matrix.read_b_row_from_file_to_arr (b_row_index, size,
                                                             message, fp);
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
        size_t rb_size = sub_matrix.get_bost_size (b_row_index);
        size_t size = rb_size * m_size;
        double *b_row = sub_matrix.get_b_row_pointer (b_row_index / p);
        recv_message (0, size, b_row_index, b_row);
      }
  if (process_index == 0)
    fclose (fp);
  return execution_status::success;
}

void
application::norm_calculate ()
{
  size_t b_size = get_b_size ();
  size_t k = get_k ();
  double *producer = bufer_1.get ();
  double *consumer = bufer_2.get ();
  for (size_t i = 0; i < k; i++)
    {
      arr_set_val (b_size, producer, 0);
      arr_set_val (b_size, consumer, 0);
      size_t size = matrix.calculate_norm_part (i, producer);
      reduce_sum_arr_all (producer, consumer, size);
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
          matrix.get_row_part (row_index, print_size, print_arr);
        else
          recv_message (source, print_size, row_index, print_arr);
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
        matrix.get_row_part (row_index, print_size, bufer);
        send_message (0, print_size, row_index, bufer);
      }
  if (process_index == 0)
    printf ("\n\n");
}

void
application::print_sub_matrix ()
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
          sub_matrix.get_row_part (row_index, print_size, print_arr);
        else
          recv_message (source, print_size, row_index, print_arr);
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
        sub_matrix.get_row_part (row_index, print_size, bufer);
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
              matrix.get_col_part (col_index, b_row_index, buf_arr);
            else
              {
                size_t rb_size = matrix.get_bost_size (b_row_index);
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
            matrix.get_col_part (col_index, b_row_index, bufer);
            size_t rb_size = matrix.get_bost_size (b_row_index);
            send_message (0, rb_size, col_index, bufer);
          }
      }
}

void
application::init_norm ()
{
  matrix.set_norm (norm);
  rv.set_eps (norm);
}

execution_status
application::cmd_arg_parsing (size_t argc, char *argv[])
{
  MPI_Comm world = MPI_COMM_WORLD;
  init_mpi_group (world, 0);
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
  n = m_size;
  m = b_size;
  if (r > m_size)
    r = m_size;
  if (b_size > m_size)
    b_size = m_size;
  if (m_size == 0 || b_size == 0 || s > 4)
    status = execution_status::cmd_parse_error;
  ppppp = get_active_process ();
  return status;
}

void
application::build_triangular_reflection (size_t local_r_index, size_t c_index)
{
  block_view blocks[2];
  matrix.get_block (blocks[0], local_r_index, c_index);
  // blocks[0].print_debug ();
  rv.build_triangular_reflection (blocks[0]);
}

void
application::spread_triangular_reflection (size_t local_r_index,
                                           size_t c_start, size_t c_end)
{
  block_view block;
  for (size_t c_index = c_start; c_index < c_end; c_index++)
    {
      matrix.get_block (block, local_r_index, c_index);
      rv.spread_triangular_reflection (block);
    }
}

void
application::build_reset_reflection (size_t sub_local_r_index,
                                     size_t local_r_index, size_t c_index)
{
  block_view blocks[2];
  matrix.get_block (blocks[0], sub_local_r_index, c_index);
  matrix.get_block (blocks[1], local_r_index, c_index);
  rv.build_reset_reflection (blocks[0], blocks[1]);
}

void
application::spread_reset_reflection (size_t sub_local_r_index,
                                      size_t local_r_index, size_t c_start,
                                      size_t c_end)
{
  block_view blocks[2];
  for (size_t c_index = c_start; c_index < c_end; c_index++)
    {
      matrix.get_block (blocks[0], sub_local_r_index, c_index);
      matrix.get_block (blocks[1], local_r_index, c_index);
      rv.spread_reset_reflection (blocks[0], blocks[1]);
    }
}

size_t
application::calculate_global_b_row_index (size_t local_b_row_index)
{
  return get_process_index () + get_active_process () * local_b_row_index;
}

size_t
application::calculate_local_b_row_index (size_t global_b_row_index)
{
  return global_b_row_index / get_active_process ();
}

double *
application::get_bufer ()
{
  return get_arr ();
}

void
application::find_diapazon (size_t len, size_t p, size_t index, size_t shift)
{
  size_t ttt = len / p;
  size_t ost = len % p;
  start = ttt * index + ((index < ost) ? index : ost);
  end = start + ttt + ((index < ost) ? 1 : 0);
  start += shift;
  end += shift;
}

void
application::print_results ()
{
  printf ("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = %d N = "
          "%d M = %d P = %d\n",
          prg_name, 24, r1, r2, t1, t2, int (s), n, m, ppppp);
}
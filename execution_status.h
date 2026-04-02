#ifndef EXECUTION_STATUS_H
#define EXECUTION_STATUS_H

enum class execution_status
{
  success,
  memory_allocate,
  runtime_error,
  read,
  open,
  inval_arg,
  small_divider,
  inactive_process,
  undefined_error,
  cmd_parse_error
};

#endif // EXECUTION_STATUS_H
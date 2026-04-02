#include "cmd_arg_parser.h"
#include <stdio.h>
#include <string.h>

cmd_arg_parser::cmd_arg_parsing_status
cmd_arg_parser::get_parse_status ()
{
  return parse_status;
}

cmd_arg_parser::cmd_arg_parsing_status
cmd_arg_parser::parse_size_t (char *arg, size_t &res)
{
  size_t len = strlen (arg);
  for (size_t i = 0; i < len; i++)
    {
      if (arg[i] < 48 || arg[i] > 57)
        return cmd_arg_parser::cmd_arg_parsing_status::format_error;
    }
  int buf{};
  if (sscanf (arg, "%d", &buf) != 1)
    return cmd_arg_parser::cmd_arg_parsing_status::read_error;
  res = size_t (buf);
  return cmd_arg_parser::cmd_arg_parsing_status::succsess;
}

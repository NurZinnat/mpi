#ifndef CMD_ARG_PARSER_H
#define CMD_ARG_PARSER_H

#include <stdlib.h>
class cmd_arg_parser
{
public:
  enum class cmd_arg_parsing_status
  {
    succsess,
    read_error,
    num_error,
    format_error
  };
  enum class cmd_arg_type
  {
    cmd_int,
    cmd_size_t,
    cmd_double,
    cmd_string
  };

private:
  cmd_arg_parsing_status parse_status{};

public:
  cmd_arg_parser () = default;
  ~cmd_arg_parser () = default;
  cmd_arg_parser (const cmd_arg_parser &x) = delete;
  cmd_arg_parser (cmd_arg_parser &&x) = delete;
  cmd_arg_parser &operator= (const cmd_arg_parser &x) = delete;
  cmd_arg_parser &operator= (cmd_arg_parser &&x) = delete;
  cmd_arg_parsing_status get_parse_status ();
  cmd_arg_parsing_status parse_size_t (char *arg, size_t &res);
};

#endif // CMD_ARG_PARSER_H
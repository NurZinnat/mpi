#include "application.h"

void
application::multiply_strings (block_string *strings, block_view *blocks)
{
  size_t str_size = strings[0].get_str_size ();
  for (size_t i = 0; i < str_size; i++)
    {
      strings[0].get_block (blocks[1], i);
      strings[1].get_block (blocks[2], i);
      blocks[0].sum_block_multiply (blocks[1], blocks[2]);
    }
}
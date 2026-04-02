#include "precision.h"

double
precision::get_eps () const
{
  return eps;
}

void
precision::set_eps (double val)
{
  eps = val * EPSILON;
}

bool
precision::is_greater_eps (double val) const
{
  return eps < val;
}

bool
precision::is_less_eps (double val) const
{
  return eps > val;
}
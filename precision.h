#ifndef PRECISION_H
#define PRECISION_H
#define EPSILON 1e-16
class precision
{
private:
  double eps{};
public:
  precision () = default;
  ~precision () = default;

  double get_eps () const;
  void set_eps (double val);
  bool is_greater_eps (double val) const;
  bool is_less_eps (double val) const;
};
#endif // PRECISION_H
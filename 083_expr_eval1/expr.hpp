#include <string>

class Expression {
 public:
  virtual std::string toString() const = 0;
  virtual ~Expression(){};
};

class NumExpression : public Expression {
  long num;

 public:
  NumExpression(long num);
  virtual std::string toString() const;
  // virtual ~NumExpression();
};

class OperatorExpression : public Expression {
  Expression * lhs;
  Expression * rhs;
  std::string op;

 public:
  OperatorExpression(Expression * lhs, Expression * rhs, std::string op);
  virtual std::string toString() const;
  virtual ~OperatorExpression();
};

class PlusExpression : public OperatorExpression {
 public:
  PlusExpression(Expression * lhs, Expression * rhs);
};

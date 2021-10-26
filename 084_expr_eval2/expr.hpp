#include <sstream>
#include <string>

class Expression {
 public:
  virtual std::string toString() const = 0;
  virtual ~Expression(){};
};

class NumExpression : public Expression {
  long num;

 public:
  NumExpression(long num) : num(num){};
  virtual std::string toString() const {
    std::stringstream result;
    result << this->num;
    return result.str();
  };
  // virtual ~NumExpression();
};

class OperatorExpression : public Expression {
  Expression * lhs;
  Expression * rhs;
  std::string op;

 public:
  OperatorExpression(Expression * lhs, Expression * rhs, std::string op) :
      lhs(lhs),
      rhs(rhs),
      op(op){};
  virtual std::string toString() const {
    std::stringstream expression;
    expression << '(' << this->lhs->toString() << ' ' << this->op << ' '
               << this->rhs->toString() << ')';
    return expression.str();
  }
  virtual ~OperatorExpression() {
    delete lhs;
    delete rhs;
  };
};

class PlusExpression : public OperatorExpression {
 public:
  PlusExpression(Expression * lhs, Expression * rhs) :
      OperatorExpression(lhs, rhs, "+"){};
};

class MinusExpression : public OperatorExpression {
 public:
  MinusExpression(Expression * lhs, Expression * rhs) :
      OperatorExpression(lhs, rhs, "-"){};
};

class TimesExpression : public OperatorExpression {
 public:
  TimesExpression(Expression * lhs, Expression * rhs) :
      OperatorExpression(lhs, rhs, "*"){};
};

class DivExpression : public OperatorExpression {
 public:
  DivExpression(Expression * lhs, Expression * rhs) : OperatorExpression(lhs, rhs, "/"){};
};

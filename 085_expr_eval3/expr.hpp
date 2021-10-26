#include <sstream>
#include <string>

class Expression {
 public:
  virtual std::string toString() const = 0;
  virtual ~Expression(){};
  virtual long evaluate() const = 0;
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
  virtual long evaluate() const { return this->num; }
};

class OperatorExpression : public Expression {
 protected:
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
  }
};

class PlusExpression : public OperatorExpression {
 public:
  PlusExpression(Expression * lhs, Expression * rhs) :
      OperatorExpression(lhs, rhs, "+"){};
  virtual long evaluate() const { return this->lhs->evaluate() + this->rhs->evaluate(); }
};

class MinusExpression : public OperatorExpression {
 public:
  MinusExpression(Expression * lhs, Expression * rhs) :
      OperatorExpression(lhs, rhs, "-"){};
  virtual long evaluate() const { return this->lhs->evaluate() - this->rhs->evaluate(); }
};

class TimesExpression : public OperatorExpression {
 public:
  TimesExpression(Expression * lhs, Expression * rhs) :
      OperatorExpression(lhs, rhs, "*"){};
  virtual long evaluate() const { return this->lhs->evaluate() * this->rhs->evaluate(); }
};

class DivExpression : public OperatorExpression {
 public:
  DivExpression(Expression * lhs, Expression * rhs) : OperatorExpression(lhs, rhs, "/"){};
  virtual long evaluate() const { return this->lhs->evaluate() / this->rhs->evaluate(); }
};

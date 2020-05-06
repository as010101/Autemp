#include "StringLTEPredicate.H"

StringLTEPredicate::StringLTEPredicate(Expression *left_side, Expression *right_side) 
{
  _ls = left_side;
  _rs = right_side;
}

StringLTEPredicate::~StringLTEPredicate()
{
}

bool StringLTEPredicate::evaluate(char *tuple) 
{
  char* left = _ls->evaluate(tuple);
  char* right = _rs->evaluate(tuple);
  bool b = (strcmp(left, right) < 0) || (strcmp(left, right) == 0);
  delete[] left;
  delete[] right;
  return b;
}

bool StringLTEPredicate::evaluate(char *tuple1, char *tuple2) 
{
  char* left = _ls->evaluate(tuple1, tuple2);
  char* right = _rs->evaluate(tuple1, tuple2);
  bool b = (strcmp(left, right) < 0) || (strcmp(left, right) == 0);
  delete[] left;
  delete[] right;
  return b;
}

StringLTEPredicate::StringLTEPredicate(Expression *expr) 
{
     _ls = expr;
}

void StringLTEPredicate::setExpression(Expression *expr) 
{
  if (!_ls)
    _ls = expr;
  else
    _rs = expr;
}

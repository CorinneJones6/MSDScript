/**
 * \file Expr.cpp
 * \brief Implementation of expression classes for arithmetic operations.
 *
 * Provides the implementation for various expressions including addition, multiplication,
 * numbers, and variables within an expression evaluation context.
 *
 * Created by Corinne Jones on 1/16/24.
 */

#include "Expr.hpp"
#include "Val.hpp"

//====================== Expr ======================//

/**
 * \brief Converts expression to string representation.
 * \return String representation of the expression.
 */
string Expr::to_string(){
    stringstream st("");
    this->print(st);
    return st.str();
}

/**
 * \brief Pretty prints the expression at a given precedence.
 * \param ostream The output stream to print to.
 * \param prec The precedence context in which to print.
 */
void Expr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos){
    print(ostream);
}

/**
 * \brief Pretty prints the expression.
 * \param ostream The output stream.
 */
void Expr::pretty_print(ostream &ostream){
    streampos strmpos = 0;
    pretty_print_at(ostream, prec_none, false, strmpos);
}

/**
 * \brief Converts the expression to a pretty string.
 * \return A pretty string representation of the expression.
 */
string Expr::to_pretty_string(){
    stringstream st("");
    this->pretty_print(st);
    return st.str();
}

//======================  AddExpr  ======================//

/**
 * \brief Constructs an addition expression.
 * \param lhs Left-hand side expression.
 * \param rhs Right-hand side expression.
 */
AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
/**
 * \brief Checks equality of this expression with another expression.
 * \param e The expression to compare with.
 * \return True if equal, false otherwise.
 */
bool AddExpr::equals(PTR(Expr) e) {
    PTR(AddExpr) addPtr = CAST(AddExpr)(e);
    if (addPtr == nullptr) {
        return false;
    }
    return this->lhs->equals(addPtr->lhs) && this->rhs->equals(addPtr->rhs);
}

/**
 * \brief Interprets the addition of expressions.
 * \return The result of the addition.
 */
PTR(Val) AddExpr::interp(PTR(Env) env){
    if(env == nullptr) {
        env = Env::empty;
    }
    return this->lhs->interp(env)->add_to(this->rhs->interp(env));
}

/**
 * \brief Substitutes a variable in the expression with another expression.
 * \param str The variable to substitute.
 * \param e The expression to substitute with.
 * \return The new expression after substitution.
 */
//PTR(Expr) AddExpr::subst(string str, PTR(Expr) e){
//    return NEW(AddExpr) (this->lhs->subst(str, e), this->rhs->subst(str, e));
//}

/**
 * \brief Prints the addition expression.
 * \param ostream The output stream.
 */
void AddExpr::print(ostream &ostream){
    ostream << "(";
    lhs->print(ostream);
    ostream << "+";
    rhs->print(ostream);
    ostream << ")";
}

/**
 * \brief Pretty prints the addition expression with proper precedence.
 * \param ostream The output stream.
 * \param prec The precedence level.
 */
void AddExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos) {
    if(prec >= prec_add){
        ostream << "(";
    }
    lhs->pretty_print_at(ostream, prec_add, true, strmpos);
    
    ostream << " + ";
    
    rhs->pretty_print_at(ostream, prec_none, let_parent, strmpos);
    
    if(prec >= prec_add){
        ostream << ")";
    }
}

//======================  MultExpr  ======================//

/**
 * \brief Constructs a multiplication expression.
 * \param lhs Left-hand side expression.
 * \param rhs Right-hand side expression.
 */
MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs){
  this->lhs = lhs;
  this->rhs = rhs;
}

/**
 * \brief Checks if this expression is equal to another expression.
 * \param e The expression to compare with.
 * \return True if the expressions are equal, false otherwise.
 */
bool MultExpr:: equals (PTR(Expr) e) {
PTR(MultExpr) multPtr = CAST(MultExpr)(e);
if(multPtr==nullptr){
  return false;
}
  return this->lhs->equals(multPtr->lhs) && this->rhs->equals(multPtr->rhs);
}

/**
 * \brief Evaluates the multiplication of the two expressions.
 * \return The integer result of the multiplication.
 */
PTR(Val) MultExpr::interp(PTR(Env) env){
    if(env == nullptr) {
        env = Env::empty;
    }
    return this->lhs->interp(env)->mult_with(this->rhs->interp(env));
}

/**
 * \brief Substitutes a variable within the expression.
 * \param str The variable name to replace.
 * \param e The expression to replace it with.
 * \return A new expression with the substitution made.
 */
//PTR(Expr) MultExpr::subst(string str, PTR(Expr) e){
//    return NEW(MultExpr) (this->lhs->subst(str, e), this->rhs->subst(str, e));
//}

/**
 * \brief Prints the expression to the provided output stream.
 * \param ostream The output stream.
 */
void MultExpr::print (ostream &ostream){
    ostream << "(";
    this->lhs->print(ostream);
    ostream << "*";
    this->rhs->print(ostream);
    ostream << ")";
}

/**
 * \brief Pretty prints the expression with precedence handling.
 * \param ostream The output stream.
 * \param prec The current precedence level.
 */
void MultExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos) {
    bool parent = let_parent;
    if (prec >= prec_mult) {
        ostream << "(";
        parent = false;
    }
    
    this->lhs->pretty_print_at(ostream, prec_mult, true, strmpos);
    
    ostream << " * ";
    
    this->rhs->pretty_print_at(ostream, prec_add, parent, strmpos);
    
    if (prec >= prec_mult) {
        ostream << ")";
    }
}

//======================  NumExpr  ======================//

/**
 * \brief Initializes a numeric constant expression.
 * \param rep The numeric value of the expression.
 */
NumExpr::NumExpr (int rep){
  this->val = rep;
}
/**
 * \brief Checks if this numeric constant is equal to another expression.
 * \param e A pointer to the expression to compare with this numeric constant.
 * \return True if the expressions are equal (i.e., if `e` is also a `Num` with the same value), false otherwise.
 */
bool NumExpr::equals (PTR(Expr) e) {
  PTR(NumExpr) numPtr = CAST(NumExpr)(e); // Check if 'e' is a 'Num' object
  if (numPtr == nullptr) {
      return false; // 'e' is not a 'Num' object
  }
  return this->val == numPtr->val;
}

/**
 * \brief Evaluates to its numeric value.
 * \return The value of the numeric constant.
 */
PTR(Val) NumExpr::interp(PTR(Env) env){
    return NEW(NumVal)(val) ;
}

/**
 * \brief Substitutes a variable within this numeric expression. Since `Num` does not contain variables, it returns itself.
 * \param str The variable name to look for substitution.
 * \param e The expression to substitute in place of the variable.
 * \return A pointer to this numeric constant, as no substitution occurs.
 */
//PTR(Expr) NumExpr::subst(string str, PTR(Expr) e){
//    return THIS;
//}

/**
 * \brief Prints the numeric value to the specified output stream.
 * \param ostream The output stream where the numeric value will be printed.
 */
void NumExpr::print (ostream &ostream){
    ostream<<::to_string(val);
}

//======================  VarExpr  ======================//

/**
 * \brief Constructs a variable expression.
 * \param val The name of the variable.
 */
VarExpr::VarExpr (string val){
  this->val = val;
}

/**
 * \brief Checks if this variable expression is equal to another expression.
 * \param e A pointer to the expression to compare with this variable expression.
 * \return True if `e` is a `Var` object with the same variable name, false otherwise.
 */
bool VarExpr::equals (PTR(Expr) e) {
  PTR(VarExpr) varPtr = CAST(VarExpr)(e); // Check if 'e' is a 'VarExpr' object
  if (varPtr == nullptr) {
      return false; // 'e' is not a 'VarExpr' object
  }
  return this->val == varPtr->val;
}

/**
 * \brief Throws an exception since variables cannot be directly interpreted.
 * \throws std::runtime_error when attempted to interpret a variable.
 */
PTR(Val) VarExpr::interp(PTR(Env) env){
    return env->lookup(this->val);
}

/**
 * \brief Substitutes the variable with another expression if it matches the variable name.
 * \param str The name of the variable to substitute.
 * \param e The expression to substitute in place of the variable.
 * \return The original variable or the substitution.
 */
//PTR(Expr) VarExpr::subst(string str, PTR(Expr) e){
//    if(val==str){
//        return e;
//    }
//    else {
//        return THIS;
//    }
//}

/**
 * \brief Prints the variable's name to the provided output stream.
 * \param ostream The output stream.
 */
void VarExpr::print (ostream &ostream){
    ostream << val;
}

//====================== LetExpr  ======================//

/**
 * \brief Constructor for a Let expression.
 * \param lhs The variable on the left-hand side to bind the value to.
 * \param rhs The expression on the right-hand side whose value will be bound to lhs.
 * \param body The body of the Let expression where lhs may be used.
 */
LetExpr::LetExpr(string lhs, PTR(Expr) rhs, PTR(Expr) body){
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

/**
 * \brief Checks if this Let expression is equal to another expression.
 * \param e The expression to compare with.
 * \return True if both expressions are Let expressions with equal lhs, rhs, and body; otherwise false.
 */
bool LetExpr::equals(PTR(Expr) e){
    PTR(LetExpr) _letPtr = CAST(LetExpr)(e);
    
    if(_letPtr==nullptr){
        return false;
    }
    return this->lhs==(_letPtr->lhs) && this->rhs->equals(_letPtr->rhs) && this->body->equals(_letPtr->body);
}

/**
 * \brief Interprets the Let expression by evaluating rhs, substituting it into body, and then evaluating the result.
 * \return The integer result of interpreting the Let expression.
 */
PTR(Val) LetExpr::interp(PTR(Env) env){
    
    if(env == nullptr) {
        env = Env::empty;
    }
    
    PTR(Val) rhsValue = this->rhs->interp(env);
    
    PTR(Env) new_env = NEW(ExtendedEnv)(lhs, rhsValue, env);
    
    return body->interp(new_env);
}

/**
 * \brief Performs substitution within the Let expression.
 * \param str The variable name to substitute.
 * \param e The expression to replace str with.
 * \return A new Let expression with the substitution made.
 */
//PTR(Expr) LetExpr::subst(string str, PTR(Expr) e){
//    // Check if the variable to substitute is the same as the Let's lhs
//       if (lhs == str) {
//           // If yes, do not substitute within the body, as the Let's variable shadows it
//           return NEW(LetExpr)(lhs, rhs->subst(str, e), body) ;
//       }
//       else {
//           // If not, substitute within both rhs and body
//           return NEW(LetExpr)(lhs, rhs->subst(str, e), body->subst(str, e));
//       }
//}

/**
 * \brief Prints the Let expression to the provided output stream in a specific format.
 * \param ostream The output stream to print to.
 */
void LetExpr::print(ostream &ostream){
    ostream << "(_let " << lhs << "=" << rhs->to_string() << " _in " << body->to_string() << ")";
}

/**
 * \brief Pretty prints the expression with precedence handling.
 * \param ostream The output stream.
 * \param prec The current precedence level.
 */
void LetExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos) {
   
    
    if (let_parent) {
        ostream << "(";
    }
    
    streampos startPosition = ostream.tellp();
    
    streampos depth = startPosition-strmpos;
    
    ostream << "_let " << lhs << " = ";
    
    rhs->pretty_print_at(ostream, prec_none, false, strmpos);
    
    ostream << "\n";
    
    strmpos = ostream.tellp();
    
    ostream << string(depth, ' ') << "_in  ";
   
    body->pretty_print_at(ostream, prec_none, false, strmpos);

    if (let_parent) {
        ostream << ")";
    }
}

//======================  BoolExpr  ======================//

BoolExpr::BoolExpr(bool b){
    this->val = b;
}

bool BoolExpr::equals (PTR(Expr) e){
    PTR(BoolExpr) boolPtr = CAST(BoolExpr)(e);
    
    if (boolPtr == nullptr) {
        return false;
    }
    return this->val == boolPtr->val;
}

PTR(Val) BoolExpr::interp(PTR(Env) env){
    return NEW( BoolVal)(val) ;
}

//PTR(Expr) BoolExpr::subst(string str, PTR(Expr) e){
//    return THIS;
//}

void BoolExpr::print(ostream &ostream){
    if(val){
        ostream << "_true";
    }
    else if (!val){
        ostream << "_false";
    }
}

void BoolExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos){
    
    if(val){
        ostream << "_true";
    }
    else if (!val){
        ostream << "_false";
    }
}

//======================  IfExpr  ======================//

IfExpr::IfExpr(PTR(Expr) if_, PTR(Expr) then_, PTR(Expr) else_){
    this->if_ = if_;
    this->then_ = then_;
    this->else_ = else_;
}

bool IfExpr::equals (PTR(Expr) e){
    PTR(IfExpr) ifPtr = CAST(IfExpr)(e);
    
    if (ifPtr == nullptr) {
        return false;
    }
    return this->if_->equals(ifPtr->if_) && this->then_->equals(ifPtr->then_) && this->else_->equals(ifPtr->else_);
}

PTR(Val) IfExpr::interp(PTR(Env) env){
    if(env == nullptr) {
        env = Env::empty;
    }
    PTR(Val) conditionValue = if_->interp(env);
    PTR(BoolVal) boolCondition = CAST(BoolVal)(conditionValue);
    if (boolCondition != nullptr && boolCondition->is_true()) {
        return then_->interp(env);
    } else {
        return else_->interp(env);
    }
}

//PTR(Expr) IfExpr::subst(string str, PTR(Expr) e){
//    return NEW(IfExpr)(this->if_->subst(str, e),this->then_->subst(str, e), this->else_->subst(str, e)) ;
//}

void IfExpr::print(ostream &ostream){
    ostream << "(" << "_if";
    this->if_->print(ostream);
    ostream << "_then";
    this->then_->print(ostream);
    ostream << "_else";
    this->else_->print(ostream);
    ostream << ")";
}

void IfExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos) {
    
    if (let_parent) {
            ostream << "(";
    }
    
    ostream << "_if ";
    
    if_->pretty_print_at(ostream, prec_none, false, strmpos);
    
    ostream << "\n";
    
    strmpos = ostream.tellp();
    
    ostream << "_then ";
    
    then_->pretty_print_at(ostream, prec_none, false, strmpos);
    
    ostream << "\n";
    
    ostream << "_else ";
    
    strmpos = ostream.tellp();
    
    else_->pretty_print_at(ostream, prec_none, false, strmpos);
    
    if (let_parent) {
        ostream << ")";
    }
    
    ostream << "\n";
    
}

//======================  EqExpr  ======================//

EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals (PTR(Expr) e){
    PTR(EqExpr) eqPtr = CAST(EqExpr)(e);
    
    if (eqPtr == nullptr) {
        return false;
    }
    return this->rhs->equals(eqPtr->rhs) && this->lhs->equals(eqPtr->lhs);
}

PTR(Val) EqExpr::interp(PTR(Env) env){
    if(env == nullptr) {
        env = Env::empty;
    }
   return NEW(BoolVal)(rhs->interp(env)->equals(lhs->interp(env)));
}

//PTR(Expr) EqExpr::subst(string str, PTR(Expr) e){
//    return NEW(EqExpr)(this->rhs->subst(str, e), this->lhs->subst(str, e));
//}

void EqExpr::print(ostream &ostream){
    ostream << "(";
    this->rhs->print(ostream);
    ostream << "==";
    this->lhs->print(ostream);
    ostream << ")";
}

void EqExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos){
    
    if (let_parent) {
            ostream << "(";
    }
    
    lhs->pretty_print_at(ostream, prec_none, false, strmpos);
    
    ostream << "==";
    
    rhs->pretty_print_at(ostream, prec_none, false, strmpos);
    
    if (let_parent) {
        ostream << ")";
    }
}

//======================  FunExpr  ======================//

FunExpr::FunExpr(string formal_arg, PTR(Expr) body){
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals (PTR(Expr)e){
    PTR(FunExpr) funPtr = CAST(FunExpr)(e);
    
    if (funPtr == nullptr) {
        return false;
    }
    return this->formal_arg == funPtr->formal_arg && this->body->equals(funPtr->body);
}

PTR(Val) FunExpr::interp(PTR(Env) env){
    if(env == nullptr) {
        env = Env::empty;
    }
    return NEW( FunVal)(formal_arg, body, env);
}

//PTR(Expr) FunExpr::subst(string str, PTR(Expr) e){
//    if(formal_arg == str){
//        // Return the function as is, no substitution needed
//        return THIS;
//    }
//    else{
//        // Proceed with substitution in the body if the formal argument doesn't match
//        return NEW( FunExpr)(formal_arg, body->subst(str, e));
//    }
//}

void FunExpr::print(ostream &ostream){
    ostream << "_fun (" << this->formal_arg << ") " << this->body->to_string();
}

void FunExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos){
    
}

//======================  CallExpr  ======================//

CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg){
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals (PTR(Expr) e){
    PTR(CallExpr) callPtr = CAST(CallExpr)(e);
    
    if (callPtr == nullptr) {
        return false;
    }
    return this->to_be_called->equals(callPtr->to_be_called) && this->actual_arg->equals(callPtr->actual_arg);
}

PTR(Val) CallExpr::interp(PTR(Env) env){
    if(env == nullptr) {
        env = Env::empty;
    }
    return this->to_be_called->interp(env)->call(this->actual_arg->interp(env));
}

//PTR(Expr) CallExpr::subst(string str, PTR(Expr) e){
//    return NEW(CallExpr)(this->to_be_called->subst(str, e), this->actual_arg->subst(str, e));
//}

void CallExpr::print(ostream &ostream){
    ostream << "(" << this->to_be_called->to_string() << ") (" << this->actual_arg->to_string() << ")";
}

void CallExpr::pretty_print_at(ostream &ostream, precedence_t prec, bool let_parent, streampos &strmpos){
    
}

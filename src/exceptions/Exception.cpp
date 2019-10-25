#include "exceptions/Exception.hpp"
#include <sstream>

Exception::Exception():
_prev(nullptr), _message("Exception"), message(_message){}

Exception::Exception(const Exception &e):
_prev(nullptr), _message(e._message), stacktrace(e.stacktrace), message(_message){
    if(e._prev != nullptr) this->_prev.reset(new Exception(*e._prev));
}

Exception::Exception(String message, const Exception &prev):
_prev(new Exception(prev)), _message(message), message(_message){}

Exception::Exception(String message, const elrond::sizeT skipSt):
_prev(nullptr), _message(message), message(_message){
    Stacktrace::dump(this->stacktrace, skipSt + 1);
}

Exception::Exception(const std::exception &e, const elrond::sizeT skipSt):
_prev(nullptr), _message(e.what()), message(_message){
    Stacktrace::dump(this->stacktrace, skipSt + 1);
}

Exception::~Exception(){}

void Exception::what(std::ostream &oss) const {
    elrond::sizeT i = 0;
    const Exception *le = this;

    oss << std::endl << " ********** EXCEPTION STACK **********" << std::endl;
    Exception::dumpStack(oss, *this, i, le);

    if(!le->stacktrace.empty()){
        oss << std::endl;
        Stacktrace::dump(oss, le->stacktrace);
    }
}

String Exception::what() const {
    std::ostringstream oss;
    this->what(oss);
    return oss.str();
}

void Exception::dumpStack(std::ostream &oss, const Exception &e, elrond::sizeT &i, const Exception *&le){
    if(e._prev != nullptr) Exception::dumpStack(oss, *(e._prev), i, le);
    else le = &e;
    oss << "  #" << i++ << ": " << e.message << std::endl;
}

#include "logs.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace std;

class IException {
private:
  shared_ptr<IException> sub_exceipton;

public:
  IException() = default;
  IException(shared_ptr<IException> sub_exception) {
    this->sub_exceipton = sub_exception;
  };
  operator string() {
    string result = Message();
    if (sub_exceipton) {
      result += ":\n" + (string)*sub_exceipton;
    }
    return result;
  }
  virtual string Message() = 0;
};

class CriticalException : public IException {
private:
  string message;

public:
  CriticalException(string message) {
    this->message = message;
    ERROR(message);
  }
  string Message() { return message; }
};

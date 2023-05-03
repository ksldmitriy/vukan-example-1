#pragma once
#include "../exception.hpp"

using namespace std;

namespace vk {

class CriticalException : public ::CriticalException {
private:
public:
  CriticalException(string message) : ::CriticalException(message){};
};

class MemoryTypeNotFoundException : public IException {
private:
protected:
  string Message() { return "suitable memory type not found"; };

public:
  MemoryTypeNotFoundException() : ::IException(){};
};

class QueueFamilyNotFoundException : public IException {
private:
protected:
  string Message() { return "suitable queue family not found"; };

public:
  QueueFamilyNotFoundException() : ::IException(){};
};

} // namespace vk

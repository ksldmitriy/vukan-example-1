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
public:
  MemoryTypeNotFoundException() : ::IException(){};
  string Message() { return "suitable memory type not found"; };
};

class QueueFamilyNotFoundException : public IException {
private:
public:
  QueueFamilyNotFoundException() : ::IException(){};
  string Message() { return "suitable queue family not found"; };
};

} // namespace vk

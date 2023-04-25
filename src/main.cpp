#include "main.hpp"
#include "logs.hpp"

int main() {
  setup_logs();

  Application application;
  
  try {
    application.Run();
  } catch (IException &e) {
	ERROR("application exit afer unhandled excpetion: {0}", (string)e);
  };

  INFO("aeee");
}

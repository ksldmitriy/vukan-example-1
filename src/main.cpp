#include "main.hpp"
#include "logs.hpp"

int main() {
  setup_logs();

  Application application;
  
  try {
    application.Run();
  } catch (Exception &e) {
    cout << (string)e << endl;
  };

  cout << "aeee" << endl;
}

#ifndef API
#define API

#include "crow.h"

class Api {
public:
  void run();

private:
  crow::SimpleApp app;
};

#endif // !API

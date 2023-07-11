#ifndef API
#define API

#include "crow.h"
#include "json.h"
#include "App.hpp"

class Api {
public:
  void run();

private:
  crow::SimpleApp app;
};

#endif // !API

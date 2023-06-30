#include "Api.hpp"

constexpr std::uint16_t PORT = 8080;

void Api::run() {
  app.loglevel(crow::LogLevel::Info);
  app.port(PORT).multithreaded().run();
}

#include "Api.hpp"

constexpr std::uint16_t PORT = 8080;

void Api::run() {

  // USERS ROUTES
  CROW_ROUTE(app, "/users")
    .methods("GET"_method)
    ([&]() {
      crow::json::wvalue x;
      auto clients = myApp.getClients();
      for (size_t i = 0; i < clients.size(); ++i) {
        x[i]["username"] = clients[i].name;
      }
      return crow::response{x.dump()};
    })
    .methods("PUT"_method)
    ([&](const crow::request& req) {
      auto x = crow::json::load(req.body);
      if (!x)
        return crow::response(400);
      myApp.addClient(x["username"].s(), x["password"].s());
      crow::json::wvalue y;
      y["username"] = x["username"].s();
      return crow::response{y.dump()};
    });

  CROW_ROUTE(app, "/users/<string>")
    .methods("GET"_method)
    ([&](const crow::request& req, std::string username) {
      crow::json::wvalue y;
      if (myApp.clientExists(username, "")) { // No password given, might need to change this
        const auto &client = myApp.getClient(username);
        y["username"] = client.name;
        return crow::response{y.dump()};
      }
      return crow::response(404);
    });

  // AUTH ROUTES
  CROW_ROUTE(app, "/login")
    .methods("POST"_method)
    ([&](const crow::request& req) {
      auto x = crow::json::load(req.body);
      if (!x)
        return crow::response(400);
      if (myApp.clientExists(x["username"].s(), x["password"].s())) {
        crow::json::wvalue y;
        y["username"] = x["username"].s();
        return crow::response{y.dump()};
      } else {
        return crow::response(403);
      }
    });

  CROW_ROUTE(app, "/register")
    .methods("POST"_method)
    ([&](const crow::request& req) {
      auto x = crow::json::load(req.body);
      if (!x)
        return crow::response(400);
      if (myApp.clientExists(x["username"].s(), x["password"].s())) {
        return crow::response(403); // User already exists
      } else {
        myApp.addClient(x["username"].s(), x["password"].s());
        crow::json::wvalue y;
        y["username"] = x["username"].s();
        return crow::response{y.dump()};
      }
    });

  // BLOCKCHAIN ROUTES
  CROW_ROUTE(app, "/blockchain")
    .methods("GET"_method)
    ([&]() {
      crow::json::wvalue x;
      // We'll assume BlockChain has a to_json method
      x = myApp.getBlockChain().to_json();
      return crow::response{x.dump()};
    });

  CROW_ROUTE(app, "/blockchain/transactions")
    .methods("GET"_method)
    ([&]() {
      crow::json::wvalue x;
      const auto& blockchain = myApp.getBlockChain();
      for (size_t i = 0; i < blockchain.size(); ++i) {
        x[i] = blockchain[i].to_json();
      }
      return crow::response{x.dump()};
    });

  CROW_ROUTE(app, "/blockchain/transactions/new")
    .methods("POST"_method)
    ([&](const crow::request& req) {
      auto x = crow::json::load(req.body);
      if (!x)
        return crow::response(400);
      Data newData(x);
      myApp.addBlock(newData);
      return crow::response(200);
    });


  app.loglevel(crow::LogLevel::Info);
  app.port(PORT).multithreaded().run();
}

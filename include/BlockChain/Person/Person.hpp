#ifndef CLIENTS_HPP
#define CLIENTS_HPP

#include <algorithm>
#include <mutex>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

/*
 * Warning: This class might not thread-safe
 * */

// NOLINTNEXTLINE
static std::mt19937 gen{std::random_device{}()};

constexpr std::string_view chars =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()`~"
    "-_=+[{]{|;:'\",<.>/?";
constexpr uint8_t CHARS_LENGHT = chars.size();
constexpr uint8_t ID_LENGTH = 32;

struct Wallet {

  std::string m_wallet_id;
  int m_balance{};

  Wallet() {
    m_wallet_id.reserve(ID_LENGTH);

    // Comented method is beter but slower
    // if used, reduce mt range or use a simpler generator [0, 93]
    // std::generate_n(m_wallet_id.begin(), ID_LENGTH,
    //                 [&]() { return chars[gen() % chars.size()]; });

    // https://stackoverflow.com/questions/63358009/what-is-the-most-efficient-way-to-generate-random-strings-in-c
    while (m_wallet_id.size() < ID_LENGTH) {

      auto rand = gen();

      while (rand > CHARS_LENGHT && m_wallet_id.size() < ID_LENGTH) {

        m_wallet_id += chars[rand % chars.size()];
        rand /= CHARS_LENGHT;
      }
    }
  }
  void regenId() {
    m_wallet_id.clear();
    m_wallet_id.reserve(ID_LENGTH);
    while (m_wallet_id.size() < ID_LENGTH) {

      auto rand = gen();

      while (rand > CHARS_LENGHT && m_wallet_id.size() < ID_LENGTH) {

        m_wallet_id += chars[rand % chars.size()];
        rand /= CHARS_LENGHT;
      }
    }
  }
};

class Client {
public:
  explicit Client(std::string username) : m_username(std::move(username)) {}

  inline void regenWalletId() { m_wallet.regenId(); }

  [[nodiscard]] struct Wallet Wallet() const { return m_wallet; }
  struct Wallet &Wallet() { return m_wallet; }

  void updateBalance(int amount) {

    std::lock_guard<std::mutex> lck(mutex);

    auto newBalance = m_wallet.m_balance + amount;

    if (newBalance >= 0) {
      m_wallet.m_balance = newBalance;
      return;
    }

    if (newBalance < 0) {
      throw std::runtime_error("Insufficient Founds");
    }
  }

private:
  std::string m_username;
  struct Wallet m_wallet;
  inline static std::mutex mutex{};
};

class Clients {

  std::vector<Client> m_clients;

  void add_client(const std::string &name) {
    Client client(name);
    while (
        // if m_wallet_id of client exists creae new wallet id
        std::find_if(m_clients.begin(), m_clients.end(),
                     [client](Client &other_client) {
                       return other_client.Wallet().m_wallet_id ==
                              client.Wallet().m_wallet_id;
                     }) != m_clients.end()) {
      client.regenWalletId();
    }
    m_clients.push_back(std::move(client));
  }
};

#endif // !CLIENTS_HPP

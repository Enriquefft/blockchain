#ifndef CLIENTS_HPP
#define CLIENTS_HPP

#include <algorithm>
#include <mutex>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace Person {

using std::string;

struct Wallet {

  string m_wallet_id;
  int m_balance{};

  Wallet();
  void regenId();

private:
  static inline std::mt19937 getGenerator() {
    return std::mt19937(std::random_device{}());
  }
};

class Client {
public:
  explicit Client(string username) : m_username(std::move(username)) {}

  inline void regenWalletId() { m_wallet.regenId(); }

  [[nodiscard]] struct Wallet wallet() const { return m_wallet; }
  const struct Wallet &wallet() { return m_wallet; }

  void updateBalance(const int &amount);

private:
  string m_username;
  struct Wallet m_wallet;
  inline static std::mutex mutex{};
};

} // namespace Person

#endif // !CLIENTS_HPP

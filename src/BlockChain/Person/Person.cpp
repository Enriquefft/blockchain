#include "Person/Person.hpp"

using Person::Client;
using Person::Wallet;

constexpr std::string_view CHARS =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()`~"
    "-_=+[{]{|;:'\",<.>/?";

constexpr uint8_t CHARS_LENGHT = CHARS.size();
constexpr uint8_t ID_LENGTH = 32;

/* Wallet */
Wallet::Wallet() {

  m_wallet_id.reserve(ID_LENGTH);

  // Comented method is beter but slower
  // if used, reduce mt range or use a simpler generator [0, 93]
  // std::generate_n(m_wallet_id.begin(), ID_LENGTH,
  //                 [&]() { return chars[gen() % chars.size()]; });

  // https://stackoverflow.com/questions/63358009/what-is-the-most-efficient-way-to-generate-random-strings-in-c
  auto gen = getGenerator();
  while (m_wallet_id.size() < ID_LENGTH) {

    auto rand = gen();

    while (rand > CHARS_LENGHT && m_wallet_id.size() < ID_LENGTH) {

      m_wallet_id += CHARS[rand % CHARS.size()];
      rand /= CHARS_LENGHT;
    }
  }
}
void Wallet::regenId() {

  m_wallet_id.clear();
  m_wallet_id.reserve(ID_LENGTH);
  auto gen = getGenerator();
  while (m_wallet_id.size() < ID_LENGTH) {

    auto rand = gen();

    while (rand > CHARS_LENGHT && m_wallet_id.size() < ID_LENGTH) {

      m_wallet_id += CHARS[rand % CHARS.size()];
      rand /= CHARS_LENGHT;
    }
  }
}
void Client::updateBalance(const int &amount) {
  std::lock_guard<std::mutex> lck(mutex);

  auto new_balance = m_wallet.m_balance + amount;

  if (new_balance >= 0) {
    m_wallet.m_balance = new_balance;
    return;
  }

  if (new_balance < 0) {
    throw std::runtime_error("Insufficient Founds");
  }
}

#ifndef DATA_HPP
#define DATA_HPP

#include <chrono>
#include <string>

#include "io1/money.hpp"

struct Data {

  using TIME = std::chrono::time_point<std::chrono::utc_clock>;

  enum class CompareType {
    MONEY_ASCENDING,
    MONEY_DESCENDING,
    TIME_ASCENDING,
    TIME_DESCENDING,
    SENDER_NAME_ASCENDING,
    SENDER_NAME_DESCENDING,
    RECEIVER_NAME_ASCENDING,
    RECEIVER_NAME_DESCENDING
  };

  template <CompareType T> class Compare {

    bool operator()(const Data &data1, const Data &data2) const
      requires(T == CompareType::MONEY_ASCENDING);
    bool operator()(const Data &data1, const Data &data2) const
      requires(T == CompareType::MONEY_DESCENDING);
    bool operator()(const Data &data1, const Data &data2) const
      requires(T == CompareType::TIME_ASCENDING);
    bool operator()(const Data &data1, const Data &data2) const
      requires(T == CompareType::TIME_DESCENDING);
    bool operator()(const Data &data1, const Data &data2) const
      requires(T == CompareType::SENDER_NAME_ASCENDING);
    bool operator()(const Data &data1, const Data &data2) const
      requires(T == CompareType::SENDER_NAME_DESCENDING);
    bool operator()(const Data &data1, const Data &data2) const
      requires(T == CompareType::RECEIVER_NAME_ASCENDING);
    bool operator()(const Data &data1, const Data &data2) const
      requires(T == CompareType::RECEIVER_NAME_DESCENDING);
  };

  Data() = delete;
  Data(const std::string &_sender, const std::string &_receiver,
       const io1::Money &_amount);

  io1::Money amount;
  TIME timestamp;
  std::string sender;
  std::string receiver;

  // Comparison Operators
};

#endif // DATA_HPP

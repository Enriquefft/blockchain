#ifndef DATA_HPP
#define DATA_HPP

#include <chrono>
#include <string>

#include "Credit/Credit.hpp"

struct Data {

  using TIME = std::chrono::time_point<std::chrono::utc_clock>;

  Data() = delete;
  Data(std::string _sender, std::string _receiver, const Credit &_amount);

  Credit amount;
  TIME timestamp;
  std::string sender;
  std::string receiver;

  // cout
  friend std::ostream &operator<<(std::ostream &ost, const Data &data);

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

  template <CompareType T> struct Compare {
    bool operator()(const Data &data1, const Data &data2) const
		requires(T == CompareType::MONEY_ASCENDING){
			return data1.amount > data2.amount;
		}
	/*
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
	  */
  };

};

#endif // DATA_HPP

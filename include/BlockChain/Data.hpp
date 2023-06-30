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

  template <CompareType T>
  [[nodiscard]] Credit get() const
    requires(T == CompareType::MONEY_ASCENDING ||
             T == CompareType::MONEY_DESCENDING);
  template <CompareType T>
  [[nodiscard]] TIME get() const
    requires(T == CompareType::TIME_ASCENDING ||
             T == CompareType::TIME_DESCENDING);
  template <CompareType T>
  [[nodiscard]] std::string get() const
    requires(T == CompareType::SENDER_NAME_ASCENDING ||
             T == CompareType::SENDER_NAME_DESCENDING ||
             T == CompareType::RECEIVER_NAME_ASCENDING ||
             T == CompareType::RECEIVER_NAME_DESCENDING);

  template <CompareType T> class Compare {

  public:
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
};

using CompareType = Data::CompareType;

template <CompareType T> struct GetType;

template <CompareType T>
  requires(T == CompareType::MONEY_ASCENDING ||
           T == CompareType::MONEY_DESCENDING)
struct GetType<T> {
  using type = Credit;
};

template <CompareType T>
  requires(T == CompareType::TIME_ASCENDING ||
           T == CompareType::TIME_DESCENDING)
struct GetType<T> {
  using type = Data::TIME;
};

template <CompareType T>
  requires(T == CompareType::SENDER_NAME_ASCENDING ||
           T == CompareType::SENDER_NAME_DESCENDING ||
           T == CompareType::RECEIVER_NAME_ASCENDING ||
           T == CompareType::RECEIVER_NAME_DESCENDING)
struct GetType<T> {
  using type = std::string;
};

template <CompareType T> using getType = typename GetType<T>::type;

// Extern explicit instantiation
extern template class Data::Compare<Data::CompareType::MONEY_ASCENDING>;
extern template class Data::Compare<Data::CompareType::MONEY_DESCENDING>;
extern template class Data::Compare<Data::CompareType::TIME_ASCENDING>;
extern template class Data::Compare<Data::CompareType::TIME_DESCENDING>;
extern template class Data::Compare<Data::CompareType::SENDER_NAME_ASCENDING>;
extern template class Data::Compare<Data::CompareType::SENDER_NAME_DESCENDING>;
extern template class Data::Compare<Data::CompareType::RECEIVER_NAME_ASCENDING>;
extern template class Data::Compare<
    Data::CompareType::RECEIVER_NAME_DESCENDING>;

extern template Credit Data::get<Data::CompareType::MONEY_ASCENDING>() const;
extern template Credit Data::get<Data::CompareType::MONEY_DESCENDING>() const;
extern template Data::TIME Data::get<Data::CompareType::TIME_ASCENDING>() const;
extern template Data::TIME
Data::get<Data::CompareType::TIME_DESCENDING>() const;
extern template std::string
Data::get<Data::CompareType::SENDER_NAME_ASCENDING>() const;
extern template std::string
Data::get<Data::CompareType::SENDER_NAME_DESCENDING>() const;
extern template std::string
Data::get<Data::CompareType::RECEIVER_NAME_ASCENDING>() const;
extern template std::string
Data::get<Data::CompareType::RECEIVER_NAME_DESCENDING>() const;

#endif // DATA_HPP

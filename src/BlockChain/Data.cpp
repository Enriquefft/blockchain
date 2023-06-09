#include "Data.hpp"

#include <utility>

// Comparator for sorting data
#define comparison_(type)                                                      \
  template <Data::CompareType T>                                               \
  bool Data::Compare<T>::operator()(const Data &data1, const Data &data2)      \
      const                                                                    \
    requires(T == Data::CompareType::type)

comparison_(MONEY_ASCENDING) { return data1.amount > data2.amount; }
comparison_(MONEY_DESCENDING) { return data1.amount < data2.amount; }
comparison_(TIME_ASCENDING) { return data1.timestamp > data2.timestamp; }
comparison_(TIME_DESCENDING) { return data1.timestamp < data2.timestamp; }
comparison_(SENDER_NAME_ASCENDING) { return data1.sender > data2.sender; }
comparison_(SENDER_NAME_DESCENDING) { return data1.sender < data2.sender; }
comparison_(RECEIVER_NAME_ASCENDING) { return data1.receiver > data2.receiver; }
comparison_(RECEIVER_NAME_DESCENDING) {
  return data1.receiver < data2.receiver;
}

Data::Data(std::string _sender, std::string _receiver, const Credit &_amount)
    : amount(_amount), timestamp(std::chrono::utc_clock::now()),
      sender(std::move(_sender)), receiver(std::move(_receiver)) {}

std::ostream &operator<<(std::ostream &ost, const Data &data) {
  ost << "Data(sender=" << data.sender << ", receiver=" << data.receiver
      << ", amount=" << data.amount << ", timestamp=" << data.timestamp << ")";
  return ost;
}

template <CompareType T>
Credit Data::get() const
  requires(T == CompareType::MONEY_ASCENDING ||
           T == CompareType::MONEY_DESCENDING)
{
  return amount;
}
template <CompareType T>
Data::TIME Data::get() const
  requires(T == CompareType::TIME_ASCENDING ||
           T == CompareType::TIME_DESCENDING)
{
  return timestamp;
}
template <CompareType T>
std::string Data::get() const
  requires(T == CompareType::SENDER_NAME_ASCENDING ||
           T == CompareType::SENDER_NAME_DESCENDING ||
           T == CompareType::RECEIVER_NAME_ASCENDING ||
           T == CompareType::RECEIVER_NAME_DESCENDING)
{
  if constexpr (T == CompareType::SENDER_NAME_ASCENDING ||
                T == CompareType::SENDER_NAME_DESCENDING) {
    return sender;
  } else {
    return receiver;
  }
}

// Explicit instantiation
template class Data::Compare<Data::CompareType::MONEY_ASCENDING>;
template class Data::Compare<Data::CompareType::MONEY_DESCENDING>;
template class Data::Compare<Data::CompareType::TIME_ASCENDING>;
template class Data::Compare<Data::CompareType::TIME_DESCENDING>;
template class Data::Compare<Data::CompareType::SENDER_NAME_ASCENDING>;
template class Data::Compare<Data::CompareType::SENDER_NAME_DESCENDING>;
template class Data::Compare<Data::CompareType::RECEIVER_NAME_ASCENDING>;
template class Data::Compare<Data::CompareType::RECEIVER_NAME_DESCENDING>;

template Credit Data::get<Data::CompareType::MONEY_ASCENDING>() const;
template Credit Data::get<Data::CompareType::MONEY_DESCENDING>() const;
template Data::TIME Data::get<Data::CompareType::TIME_ASCENDING>() const;
template Data::TIME Data::get<Data::CompareType::TIME_DESCENDING>() const;
template std::string
Data::get<Data::CompareType::SENDER_NAME_ASCENDING>() const;
template std::string
Data::get<Data::CompareType::SENDER_NAME_DESCENDING>() const;
template std::string
Data::get<Data::CompareType::RECEIVER_NAME_ASCENDING>() const;
template std::string
Data::get<Data::CompareType::RECEIVER_NAME_DESCENDING>() const;

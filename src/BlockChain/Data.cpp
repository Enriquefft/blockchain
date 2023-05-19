#include "Data.hpp"

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

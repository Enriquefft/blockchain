#include "Data.hpp"

using enum Data::CompareType;

using CompareType = Data::CompareType;
template <CompareType T> using Compare = Data::Compare<T>;

template <CompareType T>
bool Compare<T>::operator()(const Data &data1, const Data &data2) const
  requires(T == CompareType::MONEY_ASCENDING)
{
  return data1.m_amount > data2.m_amount;
}
template <CompareType T>
bool Compare<T>::operator()(const Data &data1, const Data &data2) const
  requires(T == CompareType::MONEY_DESCENDING)
{
  return data1.m_amount < data2.m_amount;
}
template <CompareType T>
bool Compare<T>::operator()(const Data &data1, const Data &data2) const
  requires(T == CompareType::TIME_ASCENDING)
{
  return data1.m_timestamp > data2.m_timestamp;
}

template <CompareType T>
bool Compare<T>::operator()(const Data &data1, const Data &data2) const
  requires(T == CompareType::TIME_DESCENDING)
{
  return data1.m_timestamp < data2.m_timestamp;
}

template <CompareType T>
bool Compare<T>::operator()(const Data &data1, const Data &data2) const
  requires(T == CompareType::SENDER_NAME_ASCENDING)
{
  return data1.m_sender > data2.m_sender;
}

template <CompareType T>
bool Compare<T>::operator()(const Data &data1, const Data &data2) const
  requires(T == CompareType::SENDER_NAME_DESCENDING)
{
  return data1.m_sender < data2.m_sender;
}

template <CompareType T>
bool Compare<T>::operator()(const Data &data1, const Data &data2) const
  requires(T == CompareType::RECEIVER_NAME_ASCENDING)
{
  return data1.m_receiver > data2.m_receiver;
}
template <CompareType T>
bool Compare<T>::operator()(const Data &data1, const Data &data2) const
  requires(T == CompareType::RECEIVER_NAME_DESCENDING)
{
  return data1.m_receiver < data2.m_receiver;
}

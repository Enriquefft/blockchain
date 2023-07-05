#ifndef SEARCH_STRUCTURE_BASE_HPP
#define SEARCH_STRUCTURE_BASE_HPP

#include "../Data.hpp"

namespace DataStructures {

using CompareType = Data::CompareType;

// Defines virtual and common methods for search structures
class SearchStructures {
  virtual void insert() = 0;
  virtual void remove() = 0;
  virtual bool search() = 0;
  virtual void display() = 0;
};

// Stores and manages amortization.
// Calls the appropriate structure to perform the calculation.
class SearchStructureController {};

} // namespace DataStructures

#endif // !SEARCH_STRUCTURE_BASE_HPP

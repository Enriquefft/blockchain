#ifndef SEARCH_STRUCTURE_BASE_HPP
#define SEARCH_STRUCTURE_BASE_HPP

#include "Data.hpp"

namespace DataStructures {

using CompareType = Data::CompareType;

// Defines virtual and common methods for search structures
template <CompareType T> class SearchStructureBase {};

// Stores and manages amortization.
// Calls the appropriate structure to perform the calculation.
class SearchStructureController {};

} // namespace DataStructures

#endif // !SEARCH_STRUCTURE_BASE_HPP

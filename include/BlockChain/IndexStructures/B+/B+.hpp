#include "BlockChain/Data.hpp"
#include "Utils/gsl.hpp"
#include <vector>
using namespace std;

namespace IndexStructure{

template <CompareType compare_t> class BPLUS{
public:
	using size_type = std::size_t;
	using index_type = getType<compare_t>;
	using value_type = const Data *;

	BPLUS();
	~BPLUS();

	BPLUS(BPLUS &&) noexcept;
	BPLUS &operator=(BST &&)noexcept;
	BPLUS(const BST &) = delete;
	BPLUS &operator=(const BPLUS &) = delete;

	void insert(value_type data);
	[[nodiscard]] bool search(const index_type &value) const;
	
	[[nodiscard]] vector<value_type> rangeSearch(const index_type &minValue,
			cosnt index_type &maxValue) const;

	[[nodiscard]] vector<value_type> getElements() const;

	[[nodiscard]] size_type height() const;
	[[nodiscard]] size_type size() const;
	void clear();
	[[nodiscard]] bool empty() const;

};



}

#include <algorithm>
#include <cstddef>
#include <format>
#include <iostream>
#include <span>

template <class T, std::size_t N>
[[nodiscard]] constexpr auto Slide(std::span<T, N> s, std::size_t offset,
                                   std::size_t width) {
  return s.subspan(offset, offset + width <= s.size() ? width : 0U);
}

template <class T, std::size_t N, std::size_t M>
constexpr bool StartsWith(std::span<T, N> data, std::span<T, M> prefix) {
  return data.size() >= prefix.size() &&
         std::equal(prefix.begin(), prefix.end(), data.begin());
}

template <class T, std::size_t N, std::size_t M>
constexpr bool EndsWith(std::span<T, N> data, std::span<T, M> suffix) {
  return data.size() >= suffix.size() &&
         std::equal(data.end() - suffix.size(), data.end(),
                    suffix.end() - suffix.size());
}

template <class T, std::size_t N, std::size_t M>
constexpr bool Contains(std::span<T, N> span, std::span<T, M> sub) {
  return std::search(span.begin(), span.end(), sub.begin(), sub.end()) !=
         span.end();
  //  return std::ranges::search(span, sub).begin() != span.end();
}

void Print(const auto &seq) {
  for (const auto &elem : seq) {
    std::cout << elem << ' ';
  }
  std::cout << '\n';
}

int main() {
  constexpr int A[]{0, 1, 2, 3, 4, 5, 6, 7, 8};
  constexpr int B[]{8, 7, 6};

  for (std::size_t offset{};; ++offset) {
    static constexpr std::size_t width{6};
    auto s = Slide(std::span{A}, offset, width);
    if (s.empty())
      break;
    Print(s);
  }

  static_assert(StartsWith(std::span{A}, std::span{A, 4}) and
                StartsWith(std::span{A + 1, 4}, std::span{A + 1, 3}) and
                !StartsWith(std::span{A}, std::span{B}) and
                !StartsWith(std::span{A, 8}, std::span{A + 1, 3}) and
                EndsWith(std::span{A}, std::span{A + 6, 3}) and
                !EndsWith(std::span{A}, std::span{A + 6, 2}) and
                Contains(std::span{A}, std::span{A + 1, 4}) and
                !Contains(std::span{A, 8}, std::span{A, 9}));

  std::cout << "Format:\n";

  std::cout << std::format("{:^6}", "A") << '\n';
}

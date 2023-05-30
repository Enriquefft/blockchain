#include <array>
#include <iomanip>
#include <iostream>
#include <openssl/ts.h>
#include <span>
#include <sstream>
#include <string>
#include <vector>

#include <algorithm>

using std::cout;
using std::endl;

static std::array<uint8_t, SHA256_DIGEST_LENGTH> Sha256(std::string_view str) {

  // Transorm to bytes (unsigned char)
  std::span<const unsigned char> data(
      std::bit_cast<const unsigned char *>(str.data()), str.size());

  std::array<uint8_t, SHA256_DIGEST_LENGTH> result{};
  SHA256(data.data(), data.size(), result.data());

  return result;
}

[[maybe_unused]] std::array<uint8_t, SHA256_DIGEST_LENGTH> static Hash(
    int mon, std::array<uint8_t, SHA256_DIGEST_LENGTH> data) {

  return Sha256(std::string(data.begin(),
                            data.end()) + // Previous hash
                std::to_string(mon));     // Amount
}

int main() {
  auto sha = Sha256("1234567890");
  std::stringstream stream;
  for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    stream << std::setw(2) << std::setfill('0') << std::hex
           << static_cast<int>(sha[i]);
  }

  cout << endl << ss.str() << endl;
}

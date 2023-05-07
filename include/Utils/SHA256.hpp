#ifndef SHA256_HPP
#define SHA256_HPP

#include <iomanip>
#include <openssl/ts.h>
#include <sstream>

namespace Uils {

static std::string sha256(const std::string &str) {

  const auto *str_ptr = std::bit_cast<const unsigned char *>(str.data());

  auto *hash = SHA256(str_ptr, str.size(), nullptr);

  std::stringstream sst;

  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sst << std::hex << std::setw(2) << std::setfill('0')
        << static_cast<int>(hash[i]);
  }

  return sst.str();
}

} // namespace Uils

#endif // !SHA256_HPP

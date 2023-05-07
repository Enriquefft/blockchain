#include <iomanip>
#include <iostream>
#include <openssl/ts.h>
#include <sstream>
#include <string>
#include <vector>

#include <algorithm>
using std::cout;
using std::endl;
using std::string;

static string sha256(const string &str) {

  // initizalize str_ptr with str.c_str()
  unsigned char *str_ptr = (unsigned char *)str.c_str();

  auto *hash = SHA256(str_ptr, str.size(), nullptr);

  std::stringstream sst;

  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sst << std::hex << std::setw(2) << std::setfill('0')
        << static_cast<int>(hash[i]);
  }

  return sst.str();
}

int main() {
  cout << sha256("1234567890_1") << endl;
  // cout << sha256("1234567890_2") << endl;
  // cout << sha256("1234567890_3") << endl;
  // cout << sha256("1234567890_4") << endl;
  return 0;
}

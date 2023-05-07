#include <iomanip>
#include <iostream>
#include <openssl/ts.h>
#include <sstream>
#include <string>

using std::cout;
using std::endl;
using std::string;

static string sha256(const string &str) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  A256_Init(&sha256);
  SHA256_Update(&sha256, str.c_str(), str.size());
  SHA256_Final(hash, &sha256);
  std::stringstream sst;
  for (unsigned char i : hash) {
    sst << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
  }
  return sst.str();
}

int main() {
  cout << sha256("1234567890_1") << endl;
  cout << sha256("1234567890_2") << endl;
  cout << sha256("1234567890_3") << endl;
  cout << sha256("1234567890_4") << endl;
  return 0;
}

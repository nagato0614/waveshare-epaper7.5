
#ifndef NAGATO_CURL_HPP
#define NAGATO_CURL_HPP

#include <curl/curl.h>
#include <string>

namespace nagato
{

class Curl
{
 public:
  Curl();
  ~Curl();

  std::string get(const std::string &url);

 private:

  void clean_up();

  CURL *curl_;
};

}  // namespace nagato

#endif  // NAGATO_CURL_HPP
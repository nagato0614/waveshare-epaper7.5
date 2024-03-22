#include "curl.hpp"

#include <cstdlib>
#include <exception>
#include <stdexcept>

namespace nagato
{

size_t callbackWrite(char *ptr, size_t size, size_t nmemb, std::string *stream)
{
  int dataLength = size * nmemb;
  stream->append(ptr, dataLength);
  return dataLength;
}

Curl::Curl()
{
  curl_ = curl_easy_init();
  if (!curl_)
  {
    fprintf(stderr, "Failed to initialize curl\n");
    std::exit(1);
  }
}

Curl::~Curl()
{
  this->clean_up();
}

void Curl::clean_up()
{
  if (this->curl_)
  {
    curl_easy_cleanup(this->curl_);
  }
}

std::string Curl::get(const std::string &url)
{
  if (!this->curl_)
  {
    throw std::runtime_error("curl is not initialized");
  }

  std::string chunk;
  CURLcode res;
  curl_easy_setopt(this->curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(this->curl_, CURLOPT_WRITEFUNCTION, callbackWrite);
  curl_easy_setopt(this->curl_, CURLOPT_WRITEDATA, &chunk);
  res = curl_easy_perform(this->curl_);
  if (res != CURLE_OK)
  {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }

  return chunk;
}
}  // namespace nagato
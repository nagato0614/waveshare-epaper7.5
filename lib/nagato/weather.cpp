#include "weather.hpp"

#include "nlohmann/json.hpp"

#include <iostream>

namespace nagato
{

Weather::Weather()
{
  // 環境変数を取得
  this->yahoo_client_id_ = std::getenv("YAHOO_CLIENT_ID");

  if (this->yahoo_client_id_.empty())
  {
    std::cerr << "YAHOO_CLIENT_ID is not set." << std::endl;
    exit(1);
  }

  this->curl_ = std::make_unique<Curl>();
}

Weather::~Weather()
{
}

std::string Weather::get_weather_json()
{
  const auto url = this->get_fullurl();

  const std::string json = this->curl_->get(url);

  this->json_string_ = json;
  
  return json;
}

std::string Weather::get_fullurl() const
{
  return this->url_ + this->yahoo_client_id_;
}

}  // namespace nagato

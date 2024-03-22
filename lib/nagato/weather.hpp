#ifndef NAGATO_WEATHER_HPP
#define NAGATO_WEATHER_HPP
#include <memory>
#include <string>

#include "curl.hpp"
#include "nlohmann/json.hpp"

namespace nagato
{
class Weather
{
 public:
  Weather();
  ~Weather();
  std::string get_weather_json();

 private:
  std::string get_fullurl() const;

  nlohmann::json weather_json_;

  std::string json_string_;

  std::string yahoo_client_id_;

  std::string url_ =
      "https://map.yahooapis.jp/weather/V1/"
      "place?coordinates=139.7804029,35.7329001&appid=https://map.yahooapis.jp/"
      "weather/V1/place?coordinates=139.732293,35.663613&output=json&appid=";

  std::unique_ptr<Curl> curl_;
};

}  // namespace nagato

#endif  // NAGATO_WEATHER_HPP
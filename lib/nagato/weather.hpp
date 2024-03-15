#ifndef NAGATO_WEATHER_HPP
#define NAGATO_WEATHER_HPP
#include <string>

namespace nagato {
class Weather {
public:
  Weather();
  ~Weather();

private:
  std::string get_fullurl() const;

  std::string yahoo_client_id_;

  std::string url_ =
      "https://map.yahooapis.jp/weather/V1/"
      "place?coordinates=139.7804029,35.7329001&appid=https://map.yahooapis.jp/"
      "weather/V1/place?coordinates=139.732293,35.663613&output=json&appid=";
};

} // namespace nagato

#endif // NAGATO_WEATHER_HPP
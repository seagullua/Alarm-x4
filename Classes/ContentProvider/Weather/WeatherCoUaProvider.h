#ifndef WEATHERCOUAPROVIDER_H
#define WEATHERCOUAPROVIDER_H
#include "WeatherProvider.h"
namespace Weather
{
class WeatherCoUaProvider : public WeatherProvider
{
public:
    WeatherCoUaProvider();
private:
    void do_getNewForecast(DelegatePtr delegate, const City &city);
    void do_getNewForecast(DelegatePtr delegate,
                           double longintude, double latitude,
                           const City &default_city);
};
}
#endif // WEATHERCOUAPROVIDER_H

#ifndef WEATHERHOLDER_H
#define WEATHERHOLDER_H
#include "WeatherProvider.h"
#include <queue>

namespace Weather
{
class WeatherHolder
{
public:
    class Delegate
    {
    public:
        Delegate()
        {}

        virtual ~Delegate()
        {}

        virtual void onForecastArrived(const Forecast& )
        {}
    };
    typedef std::auto_ptr<Delegate> DelegatePtr;

    static WeatherHolder& getInstance();
    void setCurrentCity(const City&);
    void setDefaultCity(const City&);
    void getForecast(DelegatePtr);
    bool hasCachedForecast();

    const Forecast& getCachedForecast();

    void setProvider(WeatherProviderPtr provider);
private:
    class ProviderUpdater;
    WeatherHolder();

    void loadForecast();
    void saveForecast();
    void updateForecast();

    std::queue<Delegate*> _waiting;

    WeatherProviderPtr _provider;
    Forecast _last_forecast;
    City _default_city;
    std::string _forecast_path;
    static WeatherHolder* _obj;
};
}
#endif // WEATHERHOLDER_H

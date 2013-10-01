#ifndef WEATHERPROVIDER_H
#define WEATHERPROVIDER_H
#include "Forecast.h"
#include "cocos2d.h"
#include <memory>
namespace Weather
{
class WeatherProvider
{
public:
    class Delegate
    {
    public:
        Delegate()
        {}

        enum Error
        {
            NetworkError,
            ParseError
        };

        virtual ~Delegate()
        {}

        virtual void onForecastRecieved(const Forecast&)
        {
        }

        virtual void onError(const Error type,
                             const std::string& description)
        {
            type;
            cocos2d::CCLog(description.c_str());
            description;
        }
    };

    typedef std::auto_ptr<Delegate> DelegatePtr;
    WeatherProvider()
    {}

    void getNewForecast(DelegatePtr delegate, const City& city)
    {
        do_getNewForecast(delegate, city);
    }

    void getNewForecast(DelegatePtr delegate,
                        double longintude, double latitude,
                        const City &default_city)
    {
        do_getNewForecast(delegate, longintude, latitude, default_city);
    }

    virtual ~WeatherProvider()
    {}
private:
    virtual void do_getNewForecast(DelegatePtr delegate, const City& city)=0;
    virtual void do_getNewForecast(DelegatePtr delegate,
                                   double longintude, double latitude,
                                   const City &default_city)
    {
        longintude;
        latitude;
        getNewForecast(delegate, default_city);
    }
};

typedef std::auto_ptr<WeatherProvider> WeatherProviderPtr;
}

#endif // WEATHERPROVIDER_H

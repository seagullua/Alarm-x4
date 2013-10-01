#include "Forecast.h"

namespace Weather
{
Forecast::Forecast()
    : _last_update(0),_is_pobability_defined(false),
      _min(0), _max(0), _state(Undefined), _rain_probability(0)
{
}

void Forecast::setLastUpdate(const time_t last_update)
{
    _last_update = last_update;
}

const time_t Forecast::getLastUpdate() const
{
    return _last_update;
}

void Forecast::setCity(const City& city)
{
    _city = city;
}

const City& Forecast::getCity() const
{
    return _city;
}

void Forecast::setMinTemperature(const Temperature t)
{
    _min = t;
}

const Temperature Forecast::getMinTemperature() const
{
    return _min;
}

void Forecast::setMaxTemperature(const Temperature t)
{
    _max = t;
}

const Temperature Forecast::getMaxTemperature() const
{
    return _max;
}

void Forecast::setCloudState(const CloudState st)
{
    _state = st;
}

const CloudState Forecast::getCloudState() const
{
    return _state;
}

void Forecast::setRainProbability(const RainProbability prob)
{
    _is_pobability_defined = true;
    _rain_probability = prob;
}
bool Forecast::isRainProbabilityDefined() const
{
    return _is_pobability_defined;
}
RainProbability Forecast::getRainProbability() const
{
    return _rain_probability;
}

void Forecast::setForecastDate(const std::string& date)
{
    _forecast_date = date;
}

const std::string& Forecast::getForecastDate() const
{
    return _forecast_date;
}
bool isUmbrellaNeeded(const Forecast& f)
{
    bool needed = false;
    if(f.isRainProbabilityDefined() && f.getRainProbability() != 0)
    {
        //If info about rain probability defined
        CloudState st = f.getCloudState();
        RainProbability prob = f.getRainProbability();

        if(prob >= 25 && st != Snow && st != HeavySnow)
            needed = true;
    }
    else
    {
        //If no probability stupid guess
        CloudState st = f.getCloudState();
        if(st == Rain || st == RainSnow || st == ShortRain || st == Lightning ||
                st == Hail)
            needed = true;
    }

    return needed;
}

}

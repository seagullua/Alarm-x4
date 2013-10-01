#include "WeatherHolder.h"
#include "../LTaskManager.h"
#include "cocos2d.h"
#include "Core/Binary.h"
#include "Core/file_exists.h"
#include "Logic/Location.h"
#include <fstream>
using namespace cocos2d;
namespace Weather
{
WeatherHolder::WeatherHolder()
{
    CCLog("Test");
    std::string wpath = CCFileUtils::sharedFileUtils()->getWriteablePath();
    _forecast_path = wpath + "forecast.ad";
    loadForecast();
    //_city = _last_forecast.getCity();
}

OutputBinaryStream& operator<<(OutputBinaryStream& os, const City& c)
{
    os << uint32_t(c.getID()) << c.getName();
    return os;
}

OutputBinaryStream& operator<<(OutputBinaryStream& os, const Forecast& f)
{
    os << f.getCity() << uint32_t(f.getCloudState()) << f.getForecastDate() <<
          uint64_t(f.getLastUpdate()) << uint32_t(f.getMaxTemperature() + 1000) <<
          uint32_t(f.getMinTemperature() + 1000) << uint32_t(f.getRainProbability());
    return os;
}

InputBinaryStream& operator>>(InputBinaryStream& is, City& c)
{
    uint32_t id = 0;
    std::string name = "";
    is >> id >> name;
    c.setID(id);
    c.setName(name);

    return is;
}
InputBinaryStream& operator>>(InputBinaryStream& is, Forecast& f)
{
    City city;
    uint32_t cloud_state = 0;
    std::string forecast_date = "";
    uint64_t last_update = 0;
    uint32_t max_temperature = 0;
    uint32_t min_temperature = 0;
    uint32_t probability = 0;

    is >> city;
    is >> cloud_state >> forecast_date >>
          last_update >> max_temperature >> min_temperature >> probability;

    f.setCity(city);
    f.setCloudState(static_cast<CloudState>(cloud_state));
    f.setForecastDate(forecast_date);
    f.setLastUpdate(last_update);
    f.setMaxTemperature(int(max_temperature) - 1000);
    f.setMinTemperature(int(min_temperature) - 1000);
    f.setRainProbability(probability);
    return is;
}

void WeatherHolder::loadForecast()
{
    if(fileExists(_forecast_path.c_str()))
    {
        std::ifstream iss(_forecast_path.c_str(), std::ios::in | std::ios::binary);
        InputBinaryStream is(iss);

        is >> _last_forecast;
        //_city = _last_forecast.getCity();
    }
}

void WeatherHolder::saveForecast()
{
    std::ofstream oss(_forecast_path.c_str(), std::ios::out | std::ios::binary);
    OutputBinaryStream os(oss,BinaryStream::MaxProtocolVersion);

    os << _last_forecast;
}

WeatherHolder& WeatherHolder::getInstance()
{
    if(_obj ==  0)
        _obj = new WeatherHolder;
    return *_obj;
}

void WeatherHolder::setCurrentCity(const City& city)
{
    if(city.getID() != _last_forecast.getCity().getID())
    {
        _last_forecast = Forecast();
        _last_forecast.setCity(city);
        //_city = city;

        updateForecast();
    }
}

void WeatherHolder::setDefaultCity(const City & city)
{
    _default_city = city;
}

class WeatherHolder::ProviderUpdater : public LTask
{
public:
    ProviderUpdater(const City& city)
        : _city(city)
    {}

    void do_performTask(LTaskDelegatePtr delegate)
    {
        WeatherHolder& holder = WeatherHolder::getInstance();
        if(holder._provider.get() == 0)
            delegate->onTaskFinished();
        else
        {
            double longitude = Location::getLongitude();
            double latitude = Location::getLatitude();

            if(longitude == 0 && latitude == 0)
            {
                //Cordinates are undefined
                holder._provider->getNewForecast(
                            WeatherProvider::DelegatePtr(new Reciever(delegate)),
                            _city);
            }
            else
            {
                holder._provider->getNewForecast(
                            WeatherProvider::DelegatePtr(new Reciever(delegate)),
                            Location::getLongitude(), Location::getLatitude(),
                            _city);
            }


        }
    }

private:
    class Reciever : public WeatherProvider::Delegate
    {
    public:
        Reciever(LTaskDelegatePtr delegate)
            : _task_delegate(delegate)
        {}
        virtual void onForecastRecieved(const Forecast& fx)
        {
            Forecast f(fx);
            time_t arrival_scan = time(0);
            f.setLastUpdate(arrival_scan);
            WeatherHolder& holder = WeatherHolder::getInstance();
            holder._last_forecast = f;
            holder.saveForecast();
            broadcastRecievedForecast(f);
            _task_delegate->onTaskFinished();

        }
        void onError(const Error,
                               const std::string&)
        {
            WeatherHolder& holder = WeatherHolder::getInstance();
            if(holder.hasCachedForecast())
                broadcastRecievedForecast(holder._last_forecast);
            else
                broadcastRecievedForecast(Forecast());
            _task_delegate->onTaskFinished();
        }
    private:
        void broadcastRecievedForecast(const Forecast& f)
        {
            WeatherHolder& holder = WeatherHolder::getInstance();
            while(!holder._waiting.empty())
            {
                WeatherHolder::Delegate* ptr = holder._waiting.front();
                holder._waiting.pop();

                ptr->onForecastArrived(f);
                delete ptr;
            }
        }

        LTaskDelegatePtr _task_delegate;
    };

    City _city;
};

void WeatherHolder::getForecast(DelegatePtr delegate)
{
    time_t current = time(0);
    time_t diff = current - _last_forecast.getLastUpdate();

    //Last update less than six hours ago
    if(diff < 3600*6)
    {
        if(delegate.get())
            delegate->onForecastArrived(_last_forecast);
    }
    else
    {
        Delegate* ptr = delegate.release();
        if(ptr)
            _waiting.push(ptr);
        updateForecast();
    }

}

void WeatherHolder::updateForecast()
{
    City forecast_city = _default_city;
    if(_last_forecast.getCity().getID() != 0)
        forecast_city = _last_forecast.getCity();

    LTaskManager& manager = LTaskManager::sharedManager();
    manager.addTask(LTaskPtr(new ProviderUpdater(forecast_city)));
}

bool WeatherHolder::hasCachedForecast()
{
    time_t current = time(0);
    time_t diff = current - _last_forecast.getLastUpdate();

    //Last update less than day ago
    return diff < 24*3600;
}

const Forecast& WeatherHolder::getCachedForecast()
{
    return _last_forecast;
}

void WeatherHolder::setProvider(WeatherProviderPtr provider)
{
    _provider = provider;
}
WeatherHolder* WeatherHolder::_obj=0;
}

#include "WeatherCoUaProvider.h"
#include "Extension/CCHttpRequest.h"
#include <sstream>
#include "Logic/Lang.h"
#include "cocos2d.h"
using namespace cocos2d;
namespace Weather
{
typedef WeatherProvider::DelegatePtr DelegatePtr;

WeatherCoUaProvider::WeatherCoUaProvider()
{

}

int stringToInt(const std::string& str)
{
    int res = 0;
    std::stringstream ss(str);
    ss >> res;
    return res;
}

class WeatherCoUaParser
{
public:
    WeatherCoUaParser(const std::string& xml)
        : _xml(xml)
    {


    }
    bool parse()
    {
        CCSAXParser parser;
        Parser p(_forecast);
        parser.setDelegator(&p);
        //replaceStringInPlace(_xml, "&lt;br&gt;", "\n");
        bool res = parser.parse(_xml.c_str(), _xml.size());
        return res;
    }

    const Forecast& getForecast() const
    {
        return _forecast;
    }
    ~WeatherCoUaParser()
    {}
private:
    class Parser : public CCSAXDelegator
    {
    public:
        Parser(Forecast& forecast)
            : _day_section(false),
              _temperature_section(false),
              _max_section(false),
              _min_section(false),
              _cloud_section(false),
              _probability_section(false),
              _finished(false),
              _name_found(false),
              _name_only_once(true),
              _forecast(forecast)
        {

        }


    private:
        Forecast& _forecast;

        bool _day_section;
        bool _temperature_section;
        bool _max_section;
        bool _min_section;
        bool _cloud_section;
        bool _probability_section;
        bool _name_found;
        bool _name_only_once;

        bool _finished;

        std::string _buffer;
        void startElement(void *ctx, const char *name, const char **atts)
        {
            CC_UNUSED_PARAM(ctx);
            CC_UNUSED_PARAM(atts);

            if(!_finished)
            {
                std::string sName((char*)name);
                if(sName == "day")
                {
                    if(atts && atts[0])
                    {
                        for(unsigned int i=0; atts[i]; i+=2)
                        {
                            std::string key((char*)atts[i]);
                            std::string value((char*)atts[i+1]);
                            if(key == "hour")
                            {

                                if(value == "15")
                                    _day_section = true;
                            }
                            else if(key == "date")
                            {
                                _forecast.setForecastDate(value);
                            }
                        }
                    }
                }
                else if(_name_only_once && sName == "name")
                {
                    _name_found = true;
                    _name_only_once = false;
                }
                else if(_day_section)
                {
                    if(sName == "cloud")
                    {
                        _cloud_section = true;
                    }
                    else if(sName == "ppcp")
                    {
                        _probability_section = true;
                    }
                    else if(sName == "t")
                    {
                        _temperature_section = true;
                    }

                    if(_temperature_section)
                    {
                        if(sName == "min")
                        {
                            _min_section = true;
                        }
                        else if(sName == "max")
                        {
                            _max_section = true;
                        }
                    }
                }
            }

        }

        void endElement(void *ctx, const char *name)
        {
            CC_UNUSED_PARAM(ctx);
            std::string sName((char*)name);
            if(_day_section && sName == "day")
            {
                _day_section = false;
                _finished = true;
            }
            else if(_name_found && sName == "name")
            {
                _name_found = false;
            }
            else if(_day_section)
            {
                if(_temperature_section)
                {
                    if(sName == "min")
                    {
                        _min_section = false;
                    }
                    else if(sName == "max")
                    {
                        _max_section = false;
                    }
                }

                if(_cloud_section && sName == "cloud")
                {
                    _cloud_section = false;
                }
                else if(_probability_section && sName == "ppcp")
                {
                    _probability_section = false;
                }
                else if(_temperature_section && sName == "t")
                {
                    _temperature_section = false;
                }


            }


        }

        void textHandler(void *ctx, const char *ch, int len)
        {
            CC_UNUSED_PARAM(ctx);
            if(_day_section || _name_found)
            {
                std::string value((char*)ch,0,len);
                if(_name_found)
                {
                    City c = _forecast.getCity();
                    c.setName(value);
                    _forecast.setCity(c);
                }
                else
                {
                    int num_val = stringToInt(value);
                    if(_cloud_section)
                    {
                        _forecast.setCloudState(static_cast<CloudState>(num_val / 10));
                    }
                    else if(_probability_section)
                    {
                        _forecast.setRainProbability(num_val);
                    }

                    if(_temperature_section)
                    {
                        if(_min_section)
                        {
                            _forecast.setMinTemperature(num_val);
                        }
                        else if(_max_section)
                        {
                            _forecast.setMaxTemperature(num_val);
                        }
                    }
                }
            }
        }


    };

    std::string _xml;
    Forecast _forecast;
};




class HttpObtainer : public CCObject
{
public:
    HttpObtainer(DelegatePtr delegate, const City &city)
        : _delegate(delegate), _city(city)
    {
        this->retain();
    }

    void obtain()
    {
        //http://xml.weather.co.ua/1.2/forecast/23?dayf=1&lang=uk&userid=android_alarm
        std::stringstream ss;
        ss << "http://xml.weather.co.ua/1.2/forecast/" << _city.getID() << "?dayf=1&lang="<< R::tr(R::Weather_LangKey) <<"&userid=android_alarm";
        std::string url = ss.str();

        CCHttpRequest *requestor = CCHttpRequest::sharedHttpRequest();
        requestor->addGetTask(url, this,
                              callfuncND_selector(HttpObtainer::onHttpRequestCompleted));
    }

private:
    void parse(const std::string& xml_data)
    {
        WeatherCoUaParser parser(xml_data);
        //CCLog(xml_data.c_str());
        if(parser.parse())
        {
            Forecast forecast = parser.getForecast();
            City c = forecast.getCity();
            c.setID(_city.getID());
            forecast.setCity(c);
            forecast.setLastUpdate(time(0));
            if(_delegate.get())
                _delegate->onForecastRecieved(forecast);
        }
        else if(_delegate.get())
            _delegate->onError(WeatherProvider::Delegate::ParseError, "");
    }

    void onHttpRequestCompleted(cocos2d::CCObject *, void *data)
    {
        HttpResponsePacket *response = (HttpResponsePacket *)data;

        if (response->request->reqType == kHttpRequestGet)
        {
            if (response->succeed)
            {
                parse(response->responseData);
                //CCLog(response->responseData.c_str());
            }
            else if(_delegate.get())
            {
                _delegate->onError(WeatherProvider::Delegate::NetworkError, response->responseData);
            }
        }
        this->release();
    }

    DelegatePtr _delegate;
    City _city;

};
class CordinatesCity
{
public:
    CordinatesCity()
    {}

    double _lat;
    double _lon;
    CityID _id;

    double relativeDistance(const CordinatesCity& city)
    {
        return angleDiff(_lat, city._lat) + angleDiff(_lon, city._lon);
    }
private:
    double angleDiff(double a, double b)
    {
        double diff = fabs(a-b);
        while(diff > 360.0)
            diff -= 360.0;
        return diff;
    }
};

class CityObtainer : public CCObject
{

public:
    CityObtainer(DelegatePtr delegate, double longitude, double latitude,
                 const City &defualt_city)
        : _delegate(delegate), _default_city(defualt_city), _longitude(longitude),
          _latitude(latitude)
    {
        this->retain();
    }

    void obtain()
    {
        loadCities();
        if(_cities.size() == 0)
            continueWithCity(_default_city);
        else
        {
            CordinatesCity cur;
            cur._lat = _latitude;
            cur._lon = _longitude;

            double min_d = cur.relativeDistance(_cities[0]);
            CityID min_id = _cities[0]._id;
            unsigned int min_i = 0;

            for(unsigned int i=1; i<_cities.size(); ++i)
            {
                double cd = cur.relativeDistance(_cities[i]);
                if(cd < min_d)
                {
                    min_d = cd;
                    min_id = _cities[i]._id;
                    min_i = i;
                }
            }

            //To far
            if(min_d > 30)
                continueWithCity(_default_city);
            else
            {
                //cur.relativeDistance(_cities[min_i]);
                continueWithCity(City("", min_id));
            }
        }


    }
private:
    static bool _cities_loaded;
    static std::vector<CordinatesCity> _cities;

    static void loadCities()
    {
        if(!_cities_loaded)
        {
            std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("cities.txt");
            unsigned long size = 0;
            unsigned char* file_cont = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r", &size);

            CCLog("File readed: %d", size);

            std::stringstream ss(std::ios::in | std::ios::out);
            ss.write((char*)file_cont, size);

            CordinatesCity b;
            while(ss.good())
            {
                ss >> b._id >> b._lat >> b._lon;
                _cities.push_back(b);
            }
            _cities_loaded = true;
        }
    }


    void continueWithCity(const City& city)
    {
        HttpObtainer* obtainer = new HttpObtainer(_delegate, city);
        obtainer->autorelease();
        obtainer->obtain();
        this->release();
    }

    DelegatePtr _delegate;
    City _default_city;
    double _longitude;
    double _latitude;
};

bool CityObtainer::_cities_loaded = false;
std::vector<CordinatesCity> CityObtainer::_cities;

void WeatherCoUaProvider::do_getNewForecast(DelegatePtr delegate, const City &city)
{
    HttpObtainer* obtainer = new HttpObtainer(delegate, city);
    obtainer->autorelease();
    obtainer->obtain();
}

void WeatherCoUaProvider::do_getNewForecast(DelegatePtr delegate,
                                            double longintude, double latitude,
                                            const City &default_city)
{
    CityObtainer* obtainer = new CityObtainer(delegate, longintude, latitude, default_city);
    obtainer->autorelease();
    obtainer->obtain();
}

}

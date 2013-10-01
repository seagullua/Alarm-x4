#include "WeatherWidget.h"
#include "ContentProvider/Weather/WeatherHolder.h"
#include <sstream>
using namespace Weather;
using namespace cocos2d;

class WeatherWidget::WeatherDelegate : public WeatherHolder::Delegate
{
public:
    WeatherDelegate(WeatherWidget* parent)
        : _parent(parent), _cancel(false)
    {}

    void onForecastArrived(const Forecast & f)
    {
        CCLog("Forecast arrived");
        if(!_cancel)
        {
            _parent->_forecast = f;
            _parent->_forecast_arrived = true;
            _parent->showWidget();
            _parent->_delegate = 0;
        }
    }

    void cancel()
    {
        _cancel = true;
    }

private:
    WeatherWidget* _parent;
    bool _cancel;
};

WeatherWidget::WeatherWidget()
    : _drawn(false), _forecast_arrived(false), _ready(false), _delegate(0)
{
    _delegate = new WeatherDelegate(this);
    WeatherHolder::getInstance().getForecast(
                WeatherHolder::DelegatePtr(_delegate));
}


void WeatherWidget::onInit()
{
    _ready = true;
    if(_forecast_arrived)
        showWidget();
}

void temperatureToString(std::ostream& os, const Temperature t)
{
    if(t > 0)
        os << '+';
    os << t;

}

std::string cloudsToString(const CloudState state)
{
    switch(state)
    {
    case Sunny: return R::tr(R::Weather_Sunny);
    case PartialyCloud: return R::tr(R::Weather_PartialyCloud);
    case Cloudy: return R::tr(R::Weather_Cloudy);
    case HeavyCloudy: return R::tr(R::Weather_HeavyCloudy);
    case ShortRain: return R::tr(R::Weather_ShortRain);
    case Rain: return R::tr(R::Weather_Rain);
    case Lightning: return R::tr(R::Weather_Lightning);
    case Hail: return R::tr(R::Weather_Hail);
    case RainSnow: return R::tr(R::Weather_RainSnow);
    case Snow: return R::tr(R::Weather_Snow);
    case HeavySnow: return R::tr(R::Weather_HeavySnow);
    }
    return "";
}

CCSprite* cloudsToImage(const CloudState state)
{
    std::stringstream ss;
    ss << "weather/_" << int(state) << "_w.png";
    return CCSprite::create(ss.str().c_str());
}
void WeatherWidget::onSuspend()
{
    _ready = false;
    if(_delegate)
    {
        _delegate->cancel();
    }
}

std::string WeatherWidget::weatherText()
{
    //bool today = true;
    std::string fr = _forecast.getForecastDate();
    if(fr.size() < 2)
        return "";

    unsigned int d1 = fr[fr.size()-2] - '0';
    unsigned int d2 = fr[fr.size()-1] - '0';

    unsigned int day = d1*10 + d2;

    time_t cur_time = time(0);
    tm* tm_val = localtime(&cur_time);
    unsigned int cur_day = tm_val->tm_mday;

    //TODO: save parsed date in forecast
    if(day == cur_day)
        return R::tr(R::Weather_Today);
    else
        return R::tr(R::Weather_Tomorrow);
}

void WeatherWidget::showWidget()
{

    if(_ready && _forecast_arrived )
    {
        time_t cur = time(0);
        time_t diff = cur - _forecast.getLastUpdate();

        if(diff < 3600 * 24 && _forecast.getCity().getID())
        {
            //If the forecast is new
            MTileBlock wb = getApi()->reserveSpace(Anchor::Right,
                                                   Anchor::Top,
                                                   MTileSize(6,3));

            MTile* weather = MTile::create("blue.png", wb.getPosition(),
                                           wb.getSize());
            Forecast f = _forecast;
            std::stringstream ss;
            temperatureToString(ss, f.getMinTemperature());
            ss << " .. ";
            temperatureToString(ss, f.getMaxTemperature());



            CCLabelTTF* title = LF::label(cloudsToString(f.getCloudState()).c_str(),
                                          LF::MEDIUM_FONT);
            weather->addChild(title, Aligment::Left, Aligment::Top);

            CCSprite* icon = cloudsToImage(f.getCloudState());
            icon->setScale(1.15f);

            float center_margin = icon->getContentSize().width*0.2f;
            weather->addChild(icon, Aligment::Right, Aligment::VCenter, 0, center_margin);


            float temp_size = LF::BIG_FONT*1.25f;
            CCLabelTTF* temperature = LF::label(ss.str().c_str(),temp_size);
            weather->addChild(temperature,
                              Aligment::Left, Aligment::VCenter, 0, center_margin+LF::vfix(temp_size)/2);

            std::stringstream sub_tit;
            sub_tit << f.getCity().getName() << weatherText();

            CCLabelTTF* city = LF::label(sub_tit.str().c_str(), LF::SMALL_FONT);
            weather->addChild(city, Aligment::Left, Aligment::Bottom);

            getApi()->addTile(weather);

            if(isUmbrellaNeeded(f))
                showUmbrellaAlarm();
        }
    }
}

void WeatherWidget::showUmbrellaAlarm()
{
    //If the forecast is new
    MTileBlock wb = getApi()->reserveSpace(Anchor::Right,
                                           Anchor::Top,
                                           MTileSize(6,2));

    MTile* umbr = MTile::create("red.png", wb.getPosition(),
                                   wb.getSize());

    addImageAndLabelToTile(umbr, R::tr(R::Waether_DoNotForgetUmbrella), "umbrella.png");
    getApi()->addTile(umbr);

}

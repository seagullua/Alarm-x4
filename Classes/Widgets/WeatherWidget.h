#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H
#include "ContentProvider/Weather/Forecast.h"
#include "Core/Widget.h"
class WeatherWidget : public Widget
{
public:
    WeatherWidget();
private:
    class WeatherDelegate;
    void onInit();
    void onSuspend();
    void showWidget();
    void showUmbrellaAlarm();
    std::string weatherText();
    bool _forecast_arrived;
    bool _drawn;
    bool _ready;
    Weather::Forecast _forecast;
    WeatherDelegate* _delegate;
};

#endif // WEATHERWIDGET_H

#include "Translations.h"
#include <sstream>
using namespace R;


const std::string dayIdToNameEN(int day_id)
{
    switch(day_id)
    {
    case 0: return "Sunday";
    case 1: return "Monday";
    case 2: return "Tuesday";
    case 3: return "Wednesday";
    case 4: return "Thursday";
    case 5: return "Friday";
    case 6: return "Saturday";
    }
    return "";
}

const std::string monthIdToNameEN(int month_id)
{
    switch(month_id)
    {
    case 0: return "January";
    case 1: return "February";
    case 2: return "March";
    case 3: return "April";
    case 4: return "May";
    case 5: return "June";
    case 6: return "July";
    case 7: return "August";
    case 8: return "Sentember";
    case 9: return "October";
    case 10: return "November";
    case 11: return "December";
    }
    return "";
}

class ENFunc : public LanguageFunc
{
public:
    ENFunc()
    {}

    virtual std::string formatDate(const int year,
                                   const int month,
                                   const int day,
                                   const int week_day)
    {
        std::stringstream ss;
        ss << day << ' ' << monthIdToNameEN(month) << ' '
           << (year + 1900) << ", " << dayIdToNameEN(week_day);
        return ss.str();
    }
};

R::LanguageFuncPtr loadEN(Translation& tr)
{

    tr[AlarmTimeChooser_AlarmIsSet] = "Alarm is set on";
    tr[AlarmTimeChooser_TimeToSleep] = "Remaining time for sleep";
    tr[AlarmTimeChooser_TimeToGetReady] = "Time to get ready";
    tr[AlarmTimeChooser_ExitTime] = "Time to get out";
    tr[AlarmTimeChooser_SetAlarm] = "OK";
    tr[AlarmTimeChooser_Cancel] = "Cancel";

    tr[AlarmWidget_TimeToWakeUp] = "Time to Wake Up";
    tr[AlarmWidget_TurnOff] = "Turn Off";
    tr[AlarmWidget_Snooze] = "Snooze";

    tr[FlowHelperWidget_TurnOffRules] =
            "Connect all circles of the same color and fill all table in order to turn off the alarm.";
    tr[FlowHelperWidget_PlayRules] = "Connect all circles of the same color and fill all table in order to complete the level.";
    tr[FlowHelperWidget_Restart] = "Other puzzle";
    tr[FlowHelperWidget_Exit] = "Back";

    tr[JokesWidget_Joke] = "Joke";

    tr[StartFlowGameWidget_MorningPuzzle] = "Morning puzzle";

    tr[TimeSelector_Hours] = "Hours";
    tr[TimeSelector_Minutes] = "Minutes";

    tr[TurnOnAlarmMenuWidget_AlarmPurpose] = "Set alarm in order to";
    tr[TurnOnAlarmMenuWidget_WakeUp] = "Wake Up";
    tr[TurnOnAlarmMenuWidget_WakeUpAndGoOut] = "Wake up and\nget out on time";

    tr[TurnOnAlarmWidget_AlarmIsSetFor] = "Alarm is set for";
    tr[TurnOnAlarmWidget_GoOutTime] = "Time to get out";
    tr[TurnOnAlarmWidget_TurnOnAlarm] = "Turn On\nAlarm";
    tr[TurnOnAlarmWidget_TurnOffAlarm] = "Turn Off\nAlarm";

    tr[Weather_Sunny] = "Sunny";
    tr[Weather_PartialyCloud] = "Partly cloudy";
    tr[Weather_Cloudy] = "Cloudy";
    tr[Weather_HeavyCloudy] = "Cloudy";
    tr[Weather_ShortRain] = "Short rain";
    tr[Weather_Rain] = "Rain";
    tr[Weather_Lightning] = "Lightning";
    tr[Weather_Hail] = "Hail";
    tr[Weather_RainSnow] = "Snow and Rain";
    tr[Weather_Snow] = "Snow";
    tr[Weather_HeavySnow] = "Heavy snow";
    tr[Weather_Today] = ", forecast for today.";
    tr[Weather_Tomorrow] = ", forecast for tomorrow.";
    tr[Waether_DoNotForgetUmbrella] = "Do not forget umbrella";

    tr[Weather_LangKey] = "en";
    return LanguageFuncPtr(new ENFunc);
}

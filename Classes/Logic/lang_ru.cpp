#include "Translations.h"
#include <sstream>
using namespace R;


const std::string dayIdToNameRu(int day_id)
{
    switch(day_id)
    {
    case 0: return "Воскресенье";
    case 1: return "Понедельник";
    case 2: return "Вторник";
    case 3: return "Среда";
    case 4: return "Четверг";
    case 5: return "Пятница";
    case 6: return "Суббота";
    }
    return "";
}

const std::string monthIdToNameRu(int month_id)
{
    switch(month_id)
    {
    case 0: return "января";
    case 1: return "февраля";
    case 2: return "марта";
    case 3: return "апреля";
    case 4: return "мая";
    case 5: return "июня";
    case 6: return "июля";
    case 7: return "августа";
    case 8: return "сентября";
    case 9: return "октября";
    case 10: return "ноября";
    case 11: return "декабря";
    }
    return "";
}

class RUFunc : public LanguageFunc
{
public:
    RUFunc()
    {}

    virtual std::string formatDate(const int year,
                                   const int month,
                                   const int day,
                                   const int week_day)
    {
        std::stringstream ss;
        ss << day << ' ' << monthIdToNameRu(month) << ' '
           << (year + 1900) << ", " << dayIdToNameRu(week_day);
        return ss.str();
    }
};

R::LanguageFuncPtr loadRU(Translation& tr)
{
    tr[AlarmTimeChooser_AlarmIsSet] = "Звонок будильника";
    tr[AlarmTimeChooser_TimeToSleep] = "Осталось спать";
    tr[AlarmTimeChooser_TimeToGetReady] = "Время на сборы";
    tr[AlarmTimeChooser_ExitTime] = "Время выхода из дома";
    tr[AlarmTimeChooser_SetAlarm] = "Завести";
    tr[AlarmTimeChooser_Cancel] = "Отменить";

    tr[AlarmWidget_TimeToWakeUp] = "Пора просыпаться";
    tr[AlarmWidget_TurnOff] = "Выключить";
    tr[AlarmWidget_Snooze] = "Спать еще";

    tr[FlowHelperWidget_TurnOffRules] = "Для того, чтобы выключить будильник соедините фигуры с одинаковым цветом таким образом, чтобы заполнить все поле";
    tr[FlowHelperWidget_PlayRules] = "Для того, чтобы пройти уровень соедините фигуры с одинаковым цветом таким образом, чтобы заполнить все поле";
    tr[FlowHelperWidget_Restart] = "Другая головоломка";
    tr[FlowHelperWidget_Exit] = "Выйти";

    tr[JokesWidget_Joke] = "Анекдот";

    tr[StartFlowGameWidget_MorningPuzzle] = "Утренняя головоломка";

    tr[TimeSelector_Hours] = "Часы";
    tr[TimeSelector_Minutes] = "Минуты";

    tr[TurnOnAlarmMenuWidget_AlarmPurpose] = "Завести будильник для того, чтобы";
    tr[TurnOnAlarmMenuWidget_WakeUp] = "Проснуться";
    tr[TurnOnAlarmMenuWidget_WakeUpAndGoOut] = "Проснуться и\nвовремя выйти из дому";

    tr[TurnOnAlarmWidget_AlarmIsSetFor] = "Звонок будильника";
    tr[TurnOnAlarmWidget_GoOutTime] = "Время выхода";
    tr[TurnOnAlarmWidget_TurnOnAlarm] = "Завести\nБудильник";
    tr[TurnOnAlarmWidget_TurnOffAlarm] = "Выключить\nБудильник";

    tr[Weather_Sunny] = "Солнечно";
    tr[Weather_PartialyCloud] = "Переменная облачность";
    tr[Weather_Cloudy] = "Облачно";
    tr[Weather_HeavyCloudy] = "Облачно";
    tr[Weather_ShortRain] = "Кратковременные дожди";
    tr[Weather_Rain] = "Дождь";
    tr[Weather_Lightning] = "Гроза";
    tr[Weather_Hail] = "Град";
    tr[Weather_RainSnow] = "Снег с дождем";
    tr[Weather_Snow] = "Снег";
    tr[Weather_HeavySnow] = "Сильный снег";
    tr[Weather_Today] = " погода на сегодня.";
    tr[Weather_Tomorrow] = " погода на завтра.";
    tr[Waether_DoNotForgetUmbrella] = "Не забудьте зонтик";

    tr[Weather_LangKey] = "ru";

    return LanguageFuncPtr(new RUFunc);
}

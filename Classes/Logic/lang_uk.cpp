#include "Translations.h"
#include <sstream>
using namespace R;


const std::string dayIdToName(int day_id)
{
    switch(day_id)
    {
    case 0: return "Неділя";
    case 1: return "Понеділок";
    case 2: return "Вівторок";
    case 3: return "Середа";
    case 4: return "Четвер";
    case 5: return "П’ятниця";
    case 6: return "Субота";
    }
    return "";
}

const std::string monthIdToName(int month_id)
{
    switch(month_id)
    {
    case 0: return "січня";
    case 1: return "лютого";
    case 2: return "березня";
    case 3: return "квітня";
    case 4: return "травня";
    case 5: return "червня";
    case 6: return "липня";
    case 7: return "серпня";
    case 8: return "вересня";
    case 9: return "жовтня";
    case 10: return "листопада";
    case 11: return "грудня";
    }
    return "";
}

class UKFunc : public LanguageFunc
{
public:
    UKFunc()
    {}

    virtual std::string formatDate(const int year,
                                   const int month,
                                   const int day,
                                   const int week_day)
    {
        std::stringstream ss;
        ss << day << ' ' << monthIdToName(month) << ' '
           << (year + 1900) << ", " << dayIdToName(week_day);
        return ss.str();
    }
};

R::LanguageFuncPtr loadUK(Translation& tr)
{
    tr[AlarmTimeChooser_AlarmIsSet] = "Дзвінок будильника";
    tr[AlarmTimeChooser_TimeToSleep] = "Залишилось часу спати";
    tr[AlarmTimeChooser_TimeToGetReady] = "Час на збори";
    tr[AlarmTimeChooser_ExitTime] = "Час виходу з дому";
    tr[AlarmTimeChooser_SetAlarm] = "Завести";
    tr[AlarmTimeChooser_Cancel] = "Скасувати";

    tr[AlarmWidget_TimeToWakeUp] = "Час прокидатись";
    tr[AlarmWidget_TurnOff] = "Вимкнути";
    tr[AlarmWidget_Snooze] = "Спати ще";

    tr[FlowHelperWidget_TurnOffRules] = "Для того, щоб вимкнути будильник з’єднайте "
            "фігури одного кольору таким чином, щоб заповнити "
            "всю дошку";
    tr[FlowHelperWidget_PlayRules] = "Для того, щоб пройти рівень з’єднайте "
            "фігури одного кольору таким чином, щоб заповнити "
            "всю дошку";
    tr[FlowHelperWidget_Restart] = "Інша головоломка";
    tr[FlowHelperWidget_Exit] = "Вийти";

    tr[JokesWidget_Joke] = "Анекдот";

    tr[StartFlowGameWidget_MorningPuzzle] = "Ранкова головоломка";

    tr[TimeSelector_Hours] = "Години";
    tr[TimeSelector_Minutes] = "Хвилини";

    tr[TurnOnAlarmMenuWidget_AlarmPurpose] = "Завести будильник для того, щоб";
    tr[TurnOnAlarmMenuWidget_WakeUp] = "Прокинутись";
    tr[TurnOnAlarmMenuWidget_WakeUpAndGoOut] = "Прокинутись та\nвчасно вийти з дому";

    tr[TurnOnAlarmWidget_AlarmIsSetFor] = "Будильник задзвонить";
    tr[TurnOnAlarmWidget_GoOutTime] = "Час виходу";
    tr[TurnOnAlarmWidget_TurnOnAlarm] = "Завести\nБудильник";
    tr[TurnOnAlarmWidget_TurnOffAlarm] = "Вимкнути\nБудильник";

    tr[Weather_Sunny] = "Сонячно";
    tr[Weather_PartialyCloud] = "Хмарно з проясненнями";
    tr[Weather_Cloudy] = "Хмарно";
    tr[Weather_HeavyCloudy] = "Хмарно";
    tr[Weather_ShortRain] = "Короткочасні дощі";
    tr[Weather_Rain] = "Дощ";
    tr[Weather_Lightning] = "Гроза";
    tr[Weather_Hail] = "Град";
    tr[Weather_RainSnow] = "Сніг з дощем";
    tr[Weather_Snow] = "Сніг";
    tr[Weather_HeavySnow] = "Сильний сніг";
    tr[Weather_Today] = " погода на сьогодні.";
    tr[Weather_Tomorrow] = " погода на завтра.";
    tr[Waether_DoNotForgetUmbrella] = "Не забудьте парасольку";

    tr[Weather_LangKey] = "uk";

    return LanguageFuncPtr(new UKFunc);
}

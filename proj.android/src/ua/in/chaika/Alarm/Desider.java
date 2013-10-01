package ua.in.chaika.Alarm;

public class Desider {
	private static final String LAST_ALARM = "last_alarm";
	private static final String LAST_ALARM_CANCEL = "last_cancel";
	static public void alarmCall()
	{
		App.getSharedPreferencesEditor().putLong(LAST_ALARM, System.currentTimeMillis());
		App.getSharedPreferencesEditor().commit();
	}
	
	static public void alarmCanceled()
	{
		App.getSharedPreferencesEditor().putLong(LAST_ALARM_CANCEL, System.currentTimeMillis());
		App.getSharedPreferencesEditor().commit();
	}
	
	static public boolean isWakeUpTime()
	{
		long last_alarm = App.getSharedPreferences().getLong(LAST_ALARM, 0);
		long last_cancel = App.getSharedPreferences().getLong(LAST_ALARM_CANCEL, 0);
		
		long time = System.currentTimeMillis();
		
		boolean is_wake_up_time = false;
		final long max_delay_after_alarm = 1000;
		
		if(last_cancel >= last_alarm)
			is_wake_up_time = false;
		else if(time - last_alarm > max_delay_after_alarm)
			is_wake_up_time = false;
		else
			is_wake_up_time = true;
		
		return is_wake_up_time;
		
	}
}

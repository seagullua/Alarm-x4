package ua.in.chaika.Alarm;

public class VStorage {
	
	static int getInt(int key)
	{
		return App.getSharedPreferences().getInt(String.valueOf(key), 0);
	}
	
	static void setInt(int key, int value)
	{
		App.getSharedPreferencesEditor().putInt(String.valueOf(key), value);
		App.getSharedPreferencesEditor().commit();
	}
}

package ua.in.chaika.Alarm;

import java.util.Locale;

import android.util.Log;

public class IntentParams {
	public static final int LAUNCH_MODE = 0;
	public static final int WAKEUP_MODE = 1;
	
	public static native void setMode(final int mode);
	public static native int getMode();
	
	public static final int EN = 0;
	public static final int RU = 1;
	public static final int UK = 2;
	
	public static final int getLanguage()
	{
		String lang = Locale.getDefault().getLanguage();
		//Log.d("ccc", lang);
		if("uk".equals(lang))
			return UK;
		if("ru".equals(lang))
			return RU;
		return EN;
	}
	
	static {
        System.loadLibrary("hellocpp");
   }
}

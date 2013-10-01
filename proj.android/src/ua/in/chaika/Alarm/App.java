package ua.in.chaika.Alarm;

import android.app.Application;
import android.content.Context;
import android.content.SharedPreferences;

public class App extends Application {
	private SharedPreferences settings;
    private SharedPreferences.Editor editor;
    private Planer _planner;
    private Context _context;
    private WakeLock _wake_lock;
    private Locator _location;
    private static App _app = null;
    private boolean _pending_reload = false;
    private int _next_launch_mode = IntentParams.LAUNCH_MODE;
    
    public static SharedPreferences getSharedPreferences()
    {
    	if(_app == null)
    		return null;
    	return _app.settings;
    }
    
    public static SharedPreferences.Editor getSharedPreferencesEditor()
    {
    	if(_app == null)
    		return null;
    	return _app.editor;
    }
    public static Planer getSharedPlaner()
    {
    	return _app._planner;
    }
    
    public static Locator getLocation()
    {
    	if(_app._location == null)
    		_app._location = new Locator();
    	return _app._location;
    }
    
    public static Context getAppContext()
    {
    	return _app._context;
    }
    public static WakeLock getWakeLock()
    {
    	return _app._wake_lock;
    }
    public static boolean getPendingReload()
    {
    	return _app._pending_reload;
    }
    public static void setPendingReaload(boolean r)
    {
    	_app._pending_reload = r;
    }
    public static int getLaunchMode()
    {
    	return _app._next_launch_mode;
    }
    public static void setLaunchMode(int mode)
    {
    	_app._next_launch_mode = mode;
    }
    
    public App() {
    	_app = this;
    	
    }
	
    @Override
    public void onCreate()
    {
    	super.onCreate();
    	settings = getSharedPreferences("prefs", 0);
    	editor =  settings.edit();
    	_planner = new Planer();
    	_wake_lock = new WakeLock();
    	_context = getApplicationContext();
    }
    
    
}

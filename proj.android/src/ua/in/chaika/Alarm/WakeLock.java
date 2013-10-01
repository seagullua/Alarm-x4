package ua.in.chaika.Alarm;

import android.content.Context;
import android.os.PowerManager;

public class WakeLock {
	private PowerManager.WakeLock _lock;
	private int _refs_counting = 0;
	private int _last_mode = -1;
	public void acquire(int mode)
	{
		_refs_counting++;
		
		if(_lock == null || _last_mode < mode)
		{
			PowerManager pm = (PowerManager)App.getAppContext()
					.getSystemService(Context.POWER_SERVICE);
		
			_last_mode = mode;
			PowerManager.WakeLock wl = pm.newWakeLock(
					mode, "AlarmLock");
			
			if(_lock != null)
			{
				_lock.release();
			}
			_lock = wl;
			_lock.acquire();
		}
	}
	public void release()
	{
		_refs_counting--;
		if(_refs_counting <= 0 && _lock != null)
		{
			_lock.release();
			_lock = null;
		}
	}
	
	public static void releaseLock()
	{
		App.getWakeLock().release();
	}
	
	public static void acquireCPULock()
	{
		App.getWakeLock().acquire(PowerManager.PARTIAL_WAKE_LOCK);
	}

}

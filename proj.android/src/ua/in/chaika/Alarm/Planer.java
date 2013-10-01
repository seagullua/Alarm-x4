package ua.in.chaika.Alarm;

import java.util.Calendar;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.PowerManager;
import android.provider.Settings;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

public class Planer extends BroadcastReceiver {
	public static final int WAKE_UP_ACTION = 2;
	public static final int INTERNET_FETCH = 4;
	public static final int DISPLAY_LABEL = 5;

	public static final int WAKE_UP_ALARM_FIELD = 13;

	public static final int GO_OUT_BATCH = 1000;
	public static final String ACTION = "action";

	@Override
	public void onReceive(Context context, Intent intent) {

		try {

			// You can do the processing here update the widget/remote views.
			int action_id = intent.getIntExtra(ACTION, 0);
//			Toast.makeText(context, "Action recieved" + action_id,
//					Toast.LENGTH_LONG).show();

			if (action_id == WAKE_UP_ACTION) {
				wakeUpAction(context);
			} else if (action_id >= GO_OUT_BATCH
					&& action_id <= GO_OUT_BATCH + 3) {
				goOutAlarm(action_id % GO_OUT_BATCH);

			} else {
				Toast.makeText(context, "Alarm undefined action " + action_id,
						Toast.LENGTH_LONG).show();
			}
		} catch (Exception e) {

		}
		// Release the lock

	}

	public void setAlarm(Context context, int action, int hours, int minutes) {
		if (action == DISPLAY_LABEL) {

			setStatusBarIcon(context, true);
			saveNextAlarm(context, formatDigit(hours) + ":"
					+ formatDigit(minutes));

		} else {
			AlarmManager am = (AlarmManager) context
					.getSystemService(Context.ALARM_SERVICE);
			Intent intent = new Intent(context, Planer.class);
			intent.putExtra(ACTION, action);
			PendingIntent pi = PendingIntent.getBroadcast(context, action,
					intent, PendingIntent.FLAG_CANCEL_CURRENT);

			long milisecond = getAlarmTime(hours, minutes);
//			milisecond = System.currentTimeMillis()
//					+ (4 - action) * 5000 + 5000;
//			if(action >= GO_OUT_BATCH)
//				milisecond = System.currentTimeMillis()
//						+ (action % GO_OUT_BATCH)*10000 + 5000; 
			
			am.set(AlarmManager.RTC_WAKEUP, milisecond, pi);
			// am.set(AlarmManager.RTC_WAKEUP, getAlarmTime(hours, minutes),
			// pi);

		}
	}

	public String formatDigit(int d) {
		if (d < 10)
			return "0" + String.valueOf(d);
		else
			return String.valueOf(d);
	}

	public void cancelAlarm(Context context, int action) {
		Intent intent = new Intent(context, Planer.class);
		intent.putExtra(ACTION, action);

		PendingIntent sender = PendingIntent.getBroadcast(context, action,
				intent, PendingIntent.FLAG_CANCEL_CURRENT);
		AlarmManager alarmManager = (AlarmManager) context
				.getSystemService(Context.ALARM_SERVICE);
		alarmManager.cancel(sender);

		if (action == WAKE_UP_ACTION) {
			setStatusBarIcon(context, false);
			saveNextAlarm(context, "");
		}
	}

	public static void setAlarm(int action, int hours, int minutes) {
		App.getSharedPlaner().setAlarm(App.getAppContext(), action, hours,
				minutes);
	}

	public static void cancelAlarm(int action) {
		App.getSharedPlaner().cancelAlarm(App.getAppContext(), action);
		Desider.alarmCanceled();
	}

	private long getAlarmTime(int hours, int minutes) {
		Calendar c = Calendar.getInstance();
		int c_hour = c.get(Calendar.HOUR_OF_DAY);
		int c_minute = c.get(Calendar.MINUTE);

		if (c_hour > hours || (c_hour == hours && c_minute > minutes))
			c.add(Calendar.DAY_OF_MONTH, 1);
		c.set(Calendar.HOUR_OF_DAY, hours);
		c.set(Calendar.MINUTE, minutes);

		return c.getTimeInMillis();
	}

	// private static native void internetPrefetchAction();

	private void wakeUpAction(Context c) {
		Intent intent = new Intent();
		intent.setClassName(
				Planer.class.getPackage().getName(),
				Planer.class.getPackage().getName() + "."
						+ TurnOnScreen.class.getSimpleName());

		intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK
				| Intent.FLAG_ACTIVITY_CLEAR_TOP
				| Intent.FLAG_ACTIVITY_SINGLE_TOP);

		Desider.alarmCall();
		// VStorage.setInt(WAKE_UP_ALARM_FIELD, 1);
		// App.setPendingReaload(true);
		// App.setLaunchMode(IntentParams.WAKEUP_MODE);

		c.startActivity(intent);
	}
	private static void goOutAlarm(int times)
	{
		MediaPlayer mp = new MediaPlayer();
		int r_id = R.raw.go_out1;
		if(times == 2)
			r_id = R.raw.go_out2;
		else if(times == 3)
			r_id = R.raw.go_out3;
		AssetFileDescriptor desc = App.getAppContext().getResources().openRawResourceFd(r_id);
		try
		{
			mp.setDataSource(desc.getFileDescriptor(), desc.getStartOffset(), desc.getLength());
		mp.setAudioStreamType(AudioManager.STREAM_ALARM);
		mp.prepare();
		mp.start();
		} catch (Exception e) {
			//Log.e("ccc", e.getMessage());
		}
		if(times == 3)
			goOutAlarmTurnOff();
	}
	private static native void goOutAlarmTurnOff();
	private static void setStatusBarIcon(Context context, boolean enabled) {
		Intent alarmChanged = new Intent("android.intent.action.ALARM_CHANGED");
		alarmChanged.putExtra("alarmSet", enabled);
		context.sendBroadcast(alarmChanged);
	}

	static void saveNextAlarm(final Context context, String timeString) {
		Settings.System.putString(context.getContentResolver(),
				Settings.System.NEXT_ALARM_FORMATTED, timeString);
	}

	static {
		System.loadLibrary("hellocpp");
	}
}

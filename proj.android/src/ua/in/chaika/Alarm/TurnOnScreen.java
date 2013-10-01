package ua.in.chaika.Alarm;

import android.app.Activity;
import android.app.KeyguardManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.PowerManager;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;

public class TurnOnScreen extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		
		PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
		if(pm.isScreenOn())
			openActivity();
		else
		{
			registerReceiver(mScreenOnReceiver,
	                new IntentFilter(Intent.ACTION_SCREEN_ON));
			reciever_registered = true;
			turnScreenOn();
		}
		

	}

	@Override
    public void onDestroy() {
        super.onDestroy();
        if(reciever_registered)
        {
        	unregisterReceiver(mScreenOnReceiver);
        	reciever_registered = false;
        }

    }
	private boolean reciever_registered = false;
	private final BroadcastReceiver mScreenOnReceiver =
            new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                	openActivity();
                }
            };
            
    private void openActivity()
    {
    	Intent intent = new Intent();
		intent.setClassName("ua.in.chaika.Alarm", "ua.in.chaika.Alarm.HelloCpp");
		
		
		intent.setFlags( Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
		Desider.alarmCall();
		startActivity(intent);
		finish();
    }
	
	private void turnScreenOn() {
		final Window win = getWindow();
		win.addFlags(WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED
				| WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD);
		win.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON
				| WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON
				| WindowManager.LayoutParams.FLAG_ALLOW_LOCK_WHILE_SCREEN_ON);
	}
	
}

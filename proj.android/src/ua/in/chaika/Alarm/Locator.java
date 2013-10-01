package ua.in.chaika.Alarm;

import android.annotation.SuppressLint;
import android.content.Context;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;

public class Locator {
	public double getCurrentLatitude() {
		if (_last_location == null)
			return 0;
		return _last_location.getLatitude();
	}

	public double getCurrentLongitude() {
		if (_last_location == null)
			return 0;
		return _last_location.getLongitude();
	}

	public static final double getStaticLongitude() {
		return App.getLocation().getCurrentLongitude();
	}

	public static final double getStaticLatitude() {
		return App.getLocation().getCurrentLatitude();
	}

	public Locator() {
		LocationManager manager = (LocationManager) App.getAppContext()
				.getSystemService(Context.LOCATION_SERVICE);

		saveNewestLocation(manager
				.getLastKnownLocation(LocationManager.PASSIVE_PROVIDER));
		saveNewestLocation(manager
				.getLastKnownLocation(LocationManager.NETWORK_PROVIDER));
		saveNewestLocation(manager
				.getLastKnownLocation(LocationManager.GPS_PROVIDER));

		if (_last_location != null) {
			long last_key_time = _last_location.getTime();
			long current_time = System.currentTimeMillis();

			if (current_time - last_key_time > 1000 * 3600 * 24) {
				_update_is_needed = true;
			}
		} else {
			_update_is_needed = true;
		}
		// _update_is_needed = true;
	}

	public void updateLocation() {
		if (_update_is_needed) {
			// Criteria criteria = new Criteria();
			// // if (android.os.Build.VERSION.SDK_INT >= 9)
			// criteria.setAccuracy(Criteria.ACCURACY_LOW);
			// // else
			// // criteria.setAccuracy(Criteria.ACCURACY_COARSE);
			// criteria.setAltitudeRequired(false);
			// criteria.setBearingRequired(false);
			// //criteria.setCostAllowed(false);
			// criteria.setSpeedRequired(false);
			// //criteria.setPowerRequirement(Criteria.POWER_LOW);

			LocationManager manager = (LocationManager) App.getAppContext()
					.getSystemService(Context.LOCATION_SERVICE);

			// String provider = manager.getBestProvider(criteria, true);
			// Log.d("ccc", "provider"+provider);

			_listener = new LocationListener() {

				@Override
				public void onStatusChanged(String provider, int status,
						Bundle extras) {
					Log.d("ccc", provider);
				}

				@Override
				public void onProviderEnabled(String provider) {
					Log.d("ccc", "Enabled:" + provider);
				}

				@Override
				public void onProviderDisabled(String provider) {
					Log.d("ccc", "Disabled:" + provider);
				}

				@Override
				public void onLocationChanged(Location location) {
					_last_location = location;
					Log.d("ccc", "New location" + location.getLongitude() + ","
							+ location.getLatitude());
					LocationManager manager = (LocationManager) App
							.getAppContext().getSystemService(
									Context.LOCATION_SERVICE);
					manager.removeUpdates(this);

				}
			};
			manager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0,
					_listener);
			manager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 0,
					0, _listener);
			manager.requestLocationUpdates(LocationManager.PASSIVE_PROVIDER, 0,
					0, _listener);
		}

	}

	private void saveNewestLocation(Location n) {
		if (n != null) {
			if (_last_location == null) {
				_last_location = n;
				return;
			}

			if (_last_location.getTime() < n.getTime())
				_last_location = n;

		}
	}

	private LocationListener _listener = null;
	private Location _last_location;
	private boolean _update_is_needed = false;
}

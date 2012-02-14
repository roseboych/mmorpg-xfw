package game.xfw.tester;

import game.xfw.net.NetClient;
import game.xfw.tester.util.ConfigHelper;
import game.xfw.tester.XFWApplication;
import android.app.Activity;
import android.os.Bundle;

public class BaseActivity extends Activity
{
	protected NetClient netclient_ =null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		getNetClient();
	}

	@Override
	protected void onDestroy()
	{
		netclient_ =null;
		
		super.onDestroy();
		
		this.finish();
	}

	@Override
	protected void onPause()
	{
		super.onPause();
	}

	@Override
	protected void onRestart()
	{
		super.onRestart();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
	}
	
	public ConfigHelper getConfigHelper()
	{
		return ((XFWApplication)getApplication()).getConfigHelper();
	}
	
	public NetClient getNetClient()
	{
		if( netclient_ == null)
			netclient_ =((XFWApplication)getApplication()).getNetClient();
		
		return netclient_;
	}
	
}

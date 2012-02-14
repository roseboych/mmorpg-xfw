package game.xfw.tester;

import game.xfw.net.NetClient;
import game.xfw.tester.util.ConfigHelper;
import android.app.Application;
import android.content.res.Configuration;

public class XFWApplication extends Application
{
	private ConfigHelper configure_ =null;
	private NetClient netclient_ =null;

	@Override
	public void onConfigurationChanged(Configuration newConfig)
	{
		super.onConfigurationChanged(newConfig);
	}

	@Override
	public void onCreate()
	{
		super.onCreate();
		
		//load config from res/xml/config.xml file
		getConfigHelper();
		
		//initalize net client, that will be shared all activities
		getNetClient();
	}

	@Override
	public void onTerminate()
	{
		super.onTerminate();
	}
	
	public ConfigHelper getConfigHelper()
	{
		if( configure_ == null)
		{
			configure_ =new ConfigHelper();
			configure_.loadConfig( this.getResources());
		}
		
		return configure_;
	}
	
	public NetClient getNetClient()
	{
		if( netclient_ == null)
		{
			netclient_ =new NetClient();
		}
		
		return netclient_;
	}
}

package game.xfw.tester;

import game.xfw.tester.ui.LoginActivity;
import game.xfw.tester.ui.XFWTesterActivity;
import android.content.Intent;
import android.os.Bundle;

public class XFWLauncher extends BaseActivity
{
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
	}

	@Override
	protected void onStart()
	{
		super.onStart();
		
		Intent ci =new Intent();
		if( this.netclient_.isConnected())
			ci.setClass( this, XFWTesterActivity.class);
		else
			ci.setClass( this, LoginActivity.class);

		this.startActivity( ci);
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		
		Intent ci =new Intent();
		if( this.netclient_.isConnected())
			ci.setClass( this, XFWTesterActivity.class);
		else
			ci.setClass( this, LoginActivity.class);

		this.startActivity( ci);
	}

	@Override
	protected void onPause()
	{
		super.onPause();
	}
	
}

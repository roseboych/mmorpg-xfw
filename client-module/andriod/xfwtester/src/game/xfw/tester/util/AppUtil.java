package game.xfw.tester.util;

import game.xfw.tester.XFWConst;
import game.xfw.tester.XFWLauncher;
import android.app.Activity;
import android.content.Intent;

public class AppUtil
{
	private AppUtil(){}
	
	public static void exitApplication( Activity c)
	{
		Intent ci =new Intent();
		ci.setClass( c, XFWLauncher.class);
		ci.setFlags( Intent.FLAG_ACTIVITY_CLEAR_TOP);
		ci.putExtra( XFWConst.exit_application_, true);

		c.startActivity( ci);
	}
	
	public static boolean isExitApplication( Activity c)
	{
		Intent ci =c.getIntent();
		return ci.getExtras().getBoolean( XFWConst.exit_application_, false);
	}
	
}

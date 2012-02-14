package game.xfw.tester.ui;

import game.xfw.tester.BaseActivity;
import game.xfw.tester.R;
import game.xfw.tester.XFWLauncher;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class LoginActivity extends BaseActivity implements OnClickListener
{

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		this.setContentView( R.layout.login);
		
		Button logbut =(Button)this.findViewById( R.id.login_logbut);
		logbut.setOnClickListener( this);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		return super.onCreateOptionsMenu(menu);
	}

	public void onClick( View v)
	{
		if( v.getId() == R.id.login_logbut)
		{
			EditText ip =(EditText)this.findViewById( R.id.login_ip);
			String strip =ip.getText().toString();
			EditText port =(EditText)this.findViewById(R.id.login_port);
			String sp =port.toString();
			
			netclient_.connect( strip, 1);
			
			gotoXFWTester();
		}
	}
	
	private void gotoXFWTester()
	{
		Intent ci =new Intent();
		ci.setClass( this, XFWLauncher.class);
		ci.setFlags( Intent.FLAG_ACTIVITY_CLEAR_TOP);
		this.startActivity( ci);
	}

	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		
	}
	
	
}

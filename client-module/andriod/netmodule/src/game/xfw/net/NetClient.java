package game.xfw.net;

public final class NetClient
{
	private Integer net_locker_ =new Integer(0);
	
	private boolean is_connected_ =false;
	
	public NetClient()
	{
	}
	
	public void ResetNetClient()
	{
		is_connected_ =false;
	}

	public boolean isConnected()
	{
		synchronized(net_locker_)
		{
			return is_connected_;		
		}
	}
	
	public void connect( String gtsip, int gtsport)
	{
		is_connected_ =true;
	}
	
	public void disconnect()
	{
		
	}
	
	
}

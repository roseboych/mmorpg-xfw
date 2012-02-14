package game.xfw.tester.util;

import game.xfw.tester.R;
import android.content.res.Resources;
import android.content.res.XmlResourceParser;

public class ConfigHelper
{
	private String 	gtsip_ ="127.0.01";
	private int		gtsport_ =18000;
	private String	default_user_ 	="1";
	private String	default_pwd_	="";
	
	public ConfigHelper()
	{
	}
	
	public void loadConfig( Resources rs)
	{
		try
		{
			XmlResourceParser xml =rs.getXml(R.xml.config);
			while( xml.getEventType() != XmlResourceParser.END_DOCUMENT)
			{
				if( xml.getEventType() == XmlResourceParser.START_TAG)
				{
					String name =xml.getName();
					if( name.equals( "gts"))
					{
						int cnt =xml.getAttributeCount();
						for( int ind =0; ind < cnt; ++ind)
						{
							if( "ip".equals( xml.getAttributeName( ind)))
								gtsip_ =xml.getAttributeValue( ind);
							else if( "port".equals( xml.getAttributeName( ind)))
							{
								String ss =xml.getAttributeValue( ind);
								gtsport_ =Integer.parseInt( ss);
							}
						}
					}
					else if( name.equals( "default-user"))
					{
						int cnt =xml.getAttributeCount();
						for( int ind =0; ind < cnt; ++ind)
						{
							if( "uid".equals( xml.getAttributeName( ind)))
								default_user_ =xml.getAttributeValue( ind);
							else if( "pwd".equals( xml.getAttributeName( ind)))
								default_pwd_ =xml.getAttributeValue( ind);
						}
					}
				}
				else if( xml.getEventType() == XmlResourceParser.END_TAG)
					break;
				
				xml.next();
			}
		}
		catch( Exception se)
		{
		}
	}

	public String getGtsip_()
	{
		return gtsip_;
	}

	public void setGtsip_(String gtsip_)
	{
		this.gtsip_ = gtsip_;
	}

	public int getGtsport_()
	{
		return gtsport_;
	}

	public void setGtsport_(int gtsport_)
	{
		this.gtsport_ = gtsport_;
	}

	public String getDefault_user_()
	{
		return default_user_;
	}

	public void setDefault_user_(String default_user_)
	{
		this.default_user_ = default_user_;
	}

	public String getDefault_pwd_()
	{
		return default_pwd_;
	}

	public void setDefault_pwd_(String default_pwd_)
	{
		this.default_pwd_ = default_pwd_;
	}
	
}

# 服务器组部署 #
**服务器组部署配置文件所在位置 `[source/datas/system]` 目录下.**

<h5>大区配置</h5>
```
<!--
region节点定义了整个大区包含的服务器组信息
-->
<region>
	<!--
	userdb节点定义了用户数据库信息.
	dbname:	数据库名字
	ip:		数据库所在的ip地址
	port:	数据库的端口号
	user:	数据库用户名
	pwd:	密码
	-->
	<userdb ip="127.0.0.1" port="3306" user="root" pwd="" dbname="userdb"/>

	<!--
	svrteam节点定义了大区包含的服务器组。可以有多个节点
	sn:			服务器组编号。对应system目录下的"编号.app"配置文件
	maxuser:	服务器组支持的最大玩家数
	-->
	<svrteam sn="1" maxuser="2000"/>
	
	<!--
	lgss节点配置了登陆网关服务器
	-->
	<lgss>
		<!--
		lgs节点定义了lgs服务器的基本配置
		sn:		lgs编号
		ip:		服务器监听地址
		port:	服务器监听端口
		maxuser:支持的最多玩家数
		-->
		<lgs sn="1" ip="127.0.0.1" port="18000" maxuser="3000"/>
	</lgss>
	
	<!--
	rgs节点配置了单个rgs的运行信息
	ip:		服务器监听地址
	port:	服务器监听端口
  controllerport: 管理工具监听端口
	-->
  <rgs ip="127.0.0.1" port="15000" controllerport="1200">
    <!--
    管理工具帐号
    -->
    <controllers>
      <user name="admin" password=""/>
    </controllers>
  </rgs>
	
</region>
```

<h5>大区中服务器组配置</h5>
```
<!--
svrteam节点定义了整个服务器组的部署情况
-->
<svrteam>
	<!--
	cts节点定义了centersvr的配置信息
	ip:			服务器的监听地址
	port:		服务器的监听端口
	-->
	<cts ip="127.0.0.1" port="10000" />
	
	<!--
	gts节点定义了gateway服务器组
	-->
	<gts>
		<!--
		gateway节点定义了每个具体的gateway服务器的配置信息。
		sn:			编号，gateway类型唯一
		desc:		gateway服务器描述
		ip:			服务器的监听地址
		port:		服务器的监听端口
		startuser:	起始的玩家编号（从0开始，多个gateway必须连续不能断开）
		usernum:	支持的玩家数量
		-->
		<gateway sn="1" desc="gateway1" ip="127.0.0.1" port="30000" startuser="0" usernum="500" />
	</gts>

	<!--
	dpx节点定义了每个数据库代理服务器的配置。
	ip: 		服务器监听地址
	port:		服务器监听端口
	-->
	<dpx ip="127.0.0.1" port="40000">
		<gamedb ip="127.0.0.1" port="3306" user="root" pwd="" dbname="gamedb"/>
	</dpx>

	<!--
	css节点定义了所有游戏内容服务器和副本服务器
	-->
	<css>
		<!--
		cs节点定义了游戏服务器配置
		sn:			编号，cs/is类型唯一
		desc:		cs服务器描述
		ip:			服务器的监听地址
		port:		服务器的监听端口
		map:	cs服务器处理的地图。空表示包含所有地图，特别指定的多个地图用','分割(1,2,3)
		-->
		<cs sn="1" desc="cs1" ip="127.0.0.1" port="20000" map="1,2">
			<!--如果是副本服务器-->
			<inss>
				<ins sn="1" nums="2"/>
			</inss>
		</cs>
	</css>
	
</svrteam>
```

<br>
>> IndexMenu <<
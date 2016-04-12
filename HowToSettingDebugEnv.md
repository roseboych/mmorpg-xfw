# 如何设置运行环境 #

## 配置数据库环境 ##
<h5> <b>1.</b> 安装mysql数据库 </h5>
  * 下载mysql5.x http://www.mysql.com/downloads/mirror.php?id=403198
  * 解压缩到c:\根目录
  * 下载mysql odbc驱动 http://www.mysql.com/downloads/mirror.php?id=396600

<h5> <b>2.</b> 启动mysql数据库 </h5>

<h5> <b>3.</b> 创建游戏相关的数据库 </h5>
> >mysql -u root -p <br>
<blockquote>>create database gamedb; <br>
>create database userdb; <br>
>use gamedb; <br>
>source xxxx\document\db\game.sql; <br>
>source xxxx\document\db\game.sql; <br>
>use userdb; <br>
>source xxxx\document\db\user.sql; <br>
>source xxxx\document\db\user.sql; <br></blockquote>

<h5> <b>4.</b> 创建测试数据 </h5>
<blockquote>>mysql -u root -p <br>
>use gamedb; <br>
>call generate_testdata(1000,1000); <br></blockquote>

<h5> <b>5.</b> 创建odbc数据源 </h5>
<blockquote>A. 打开控制面板 -> 管理工具 -> Data Sources(ODBC) <br>
B. 在system dns或者user dns创建2个odbc数据源 <br>
<ul><li>gamedb 关联到gamedb数据库<br>
</li><li>userdb 关联到userdb数据库</li></ul></blockquote>

<h2>配置服务器程序启动环境</h2>

<h5> <b>1.</b> 服务器程序所在的位置是 \run目录，run目录下各个子目录对应了各个功能服务器 </h5>

<h5> <b>2.</b> 执行run\setup_debugenv.bat </h5>

<h5> <b>3.</b> 如有必要请修改 run目录下各个子目录中的svrno文件，该文件是服务器的标识文件 </h5>

<h5> <b>4.</b> 启动各个应用程序 </h5>

<h5> <b>5.</b> 如需在vs2008中调试，请设置工作目录路径到 run\xxx 目录 </h5>

<h2>配置测试程序环境</h2>

<h5> <b>1.</b> 服务器测试程序所在的位置是 \tool-run 目录，包括测试程序服务器和测试程序客户端 </h5>
<ul><li>功能测试只需使用测试程序客户端<br>
</li><li>压力测试需同时使用2个程序</li></ul>

<h5> <b>2.</b> 执行tool-run\setup_debugenv.bat </h5>

<h2><i>注意</i>:必须是中文操作系统</h2>

<br>
>> IndexMenu <<
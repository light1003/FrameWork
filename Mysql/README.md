sudo vim /etc/mysql/mysql.conf.d/mysqld.cnf  -> 注释掉
bind-address = 127.0.0.1
```
3. 登录并赋权限
```
mysql [-h localhost] -uroot -p123456 // 可连接局域网内其他主机mysql
grant all on *.* to root@'%' identified by '123456' with grant option;
flush privileges;
```
4. 重启服务器服务
```
service mysql restart
```
5. 可视化工具
```
sudo apt-get install mysql-workbench
```

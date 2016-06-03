###################################################################
# File name:README.md
# Author:YFzhang
# mail:hnnyzyf2013@163.com
# Created Time:2016年06月03日 星期五 23时20分12秒
# Last Modified:2016年06月03日 星期五 23时38分57秒
####################################################################
OS
#####环境配置
GCC
version 4.4.7
ld
version 2.20.51.0.2-5.36.el6
qemu
version 2.0.0 
######################################################################
qemu安装过程
#此路径下下载
wget http://wiki.qemu-project.org/download/qemu-2.0.0.tar.bz2
#解压
tar xjvf qemu-2.0.0.tar.bz2
#切换指定目录
cd qemu-2.0.0 //如果使用的是git下载的源码，执行cd qemu
#configure
./configure --enable-kvm --enable-debug --enable-vnc --enable-werror --predix=/usr/local/qemu
# 编译
make -j8
#安装
sudo make install

#依赖库
yum install autoconf
yum install automake
yum install libtool


#########################################################################
qemu安装过程中可能会出现glib缺少，添加方法如下

wget  http://ftp.gnome.org/pub/GNOME/sources/glib/2.12/glib-2.12.0.tar.gz
tar zxvf glib-2.12.0.tar.gz
cd glib-2.12.0
./configure
make
make install




#依赖库如下
yum Install gettext
yum install autoconf
yum install automake
yum install libtool


一、目录与文件说明：
1、bin:目录下为Linux(内核版本2.6.32-220.e16.x86_64） 64bit可执行行程，包括驱动和tcmtool应用
2、tcmtool.tar.gz：代码压缩包。
3、test.sh：测试脚本

二、tcm驱动编译说明
1、编译tcm.ko,tcm_tis.ko,代码路径：
\tcmdV1.1

三、tcmtool编译说明
1、编译tddl，代码路径：
\svn\tsm\src\tddl

2、编译tcs，代码路径：
\svn\tsm\src\tcs

3、将编译好的libtddl.a,libtcs.a复制到tcmtool目录（LTcmTool）

4、编译tcmtool

四、tcmtool运行

1、安装驱动：tcm.ko,tcm_tis.ko
2、运行tcmtool


五、版本号修改 

根据需要进行修改
drv:
tcm.c tcm_tis.c  

tcmtool:
LTcmTool.cpp
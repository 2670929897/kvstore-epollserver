c/c++新手，网络是使用了epoll然后用类似reactor的思路封装了以事件驱动的功能，在此基础上增加了kv存储的业务功能，
实现多并发。可以直接clone到linux环境中使用，直接make就行

5.27：
今天加昨天新增了红黑树的存储引擎，然后增加了testcase的测试代码，现在git clone下来就make，然后编译一下testcase，
-m取1就是测试数组的引擎，-m取2就是测试红黑树的引擎，我这里测的是红黑树接近两万了。然后就这么多
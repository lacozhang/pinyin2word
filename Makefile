
test : test.cc options.h options.cc
	g++ -I/cygdrive/f/zhangyu/include -L/cygdrive/f/zhangyu/lib -o test test.cc options.h options.cc -lboost_regex

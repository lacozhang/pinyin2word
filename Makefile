
test : test.cc options.h options.cc input.h input.cc converter.h converter.cc
	g++ -I/cygdrive/f/zhangyu/include -L/cygdrive/f/zhangyu/lib -DDEBUG -o test test.cc options.h options.cc input.h input.cc converter.h converter.cc -lboost_regex

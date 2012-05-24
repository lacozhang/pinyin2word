
test : test.cc options.h options.cc input.h input.cc converter.h converter.cc trie.h trie.cc
	g++ -g -I/cygdrive/f/zhangyu/include -L/cygdrive/f/zhangyu/lib -DDEBUG2 -o test test.cc options.h options.cc input.h input.cc converter.h converter.cc trie.h trie.cc help.h help.cc -lboost_regex

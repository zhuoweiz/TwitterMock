CXXFLAGS = -g -Wall -c 
CXX = g++ -std=c++11

all: twitter heapTest hashTest

heapTest: heapTest.cpp heap.h
	$(CXX) -g -Wall $< -o $@

hashTest: hashTest.cpp hash.o
	$(CXX) -g -Wall $^ -o $@

twitter: twitter.cpp tweet.o user.o datetime.o twiteng.o util.o handler.o cmdhandler.o hash.o
	$(CXX) -g -Wall $^ -o $@

hash.o: hash.cpp hash.h
	$(CXX) $(CXXFLAGS) $< -o $@

tweet.o: tweet.cpp tweet.h
	$(CXX) $(CXXFLAGS) $< -o $@

user.o: user.cpp user.h
	$(CXX) $(CXXFLAGS) $< -o $@

datetime.o: datetime.cpp datetime.h
	$(CXX) $(CXXFLAGS) $< -o $@

twiteng.o: twiteng.cpp twiteng.h
	$(CXX) $(CXXFLAGS) $< -o $@

util.o: util.cpp util.h
	$(CXX) $(CXXFLAGS) $< -o $@

handler.o: handler.cpp handler.h
	$(CXX) $(CXXFLAGS) $< -o $@

cmdhandler.o: cmdhandler.cpp cmdhandler.h
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean

clean: 
	rm twitter
	rm -rf twitter.dSYM
	rm heapTest hashTest
	rm -rf ./*.feed
	rm -rf ./*.o

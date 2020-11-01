CXXFLAGS = -g -Wall -pthread
threadpool_test: threadpool_test.o threadpool.o
	g++ $(CXXFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o threadpool_test
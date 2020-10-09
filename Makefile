all:
	g++ fibheap.cpp hashtagCounter.cpp  -std=gnu++0x -o hashtagcounter	


clean:
	rm -f *.o hashtagcounter



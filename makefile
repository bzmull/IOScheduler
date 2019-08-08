iosched:
	g++ -g main.cpp Scheduler.cpp IO_Request.cpp -o iosched

clean:
	rm -f iosched *~
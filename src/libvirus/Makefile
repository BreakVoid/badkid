all: libvirus.so

libvirus.so: virus.cc
	g++ -o libvirus.so virus.cc -std=c++11 -fPIC -shared -pthread

clean:
	-rm libvirus.so
	-rm libvirus.so.infected
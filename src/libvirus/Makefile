all: libvirus.so

libvirus.so: virus.cc
	g++ -o libvirus.so virus.cc -std=c++11 -fPIC -shared

clean:
	-rm libvirus.so
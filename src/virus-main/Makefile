all: virus

virus: data.cc
	g++ -o virus virus-main.cc data.cc -ldl
data.cc: so2str.cc
	$(MAKE) -C ../libvirus all
	g++ so2str.cc -o so2str
	./so2str

clean:
	$(MAKE) -C ../libvirus clean
	-rm so2str
	-rm virus
	-rm data.cc
	-rm virus.infected
	-rm so2str.infected
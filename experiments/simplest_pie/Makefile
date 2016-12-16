all: simplest_pie

simplest_pie: simplest_pie.c link-script
	gcc simplest_pie.c -o simplest_pie -fPIE -pie -Wl,-T,link-script

clean:
	-rm simplest_pie
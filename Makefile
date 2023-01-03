all: fail good

fail:
	g++ -o fail.o -c demo.cc
	g++ -pie fail.o -o fail -lbcc

good:
	g++ -o good.o -c demo.cc
	g++ -no-pie good.o -o good -lbcc

clean:
	rm -f good good.o
	rm -f fail fail.o

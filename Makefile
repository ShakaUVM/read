tester: main.cc read.h shuf.txt
	g++ -O3 -o tester main.cc

oldway: main.cc read.h shuf.txt
	g++ -O3 -DORIG -o oldway main.cc

shuf.txt:
	seq 1 1000000 | shuf - > shuf.txt 

clean:
	rm -f tester oldway shuf.txt core a.out *.o

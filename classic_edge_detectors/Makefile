C99=c99 # C99 compiler

#change these flags to point to your libpng headers and libraries (only required on some unix flavours different from ubuntu (OSX mainly))
CFLAGS=-g -I/usr/local/include
LDFLAGS=-L/usr/local/lib
FLAGS=$(CFLAGS) $(LDFLAGS)

edges: edges.c io_png.* classic_edge_detectors.*
	${C99} -o edges edges.c io_png.c classic_edge_detectors.c $(FLAGS) -lpng -lm

test: edges
	./edges -r 0.1 -p 0.1 -s 0.1 -m 3 25 0.07 -l 3 29 0.13 -h 0.4 input/windmill.png
	diff out_roberts.png expected_results/windmill_roberts_0.1.png
	diff out_prewitt.png expected_results/windmill_prewitt_0.1.png
	diff out_sobel.png expected_results/windmill_sobel_0.1.png
	diff out_mh.png expected_results/windmill_mh_3_25_0.07.png
	diff out_mhl.png expected_results/windmill_mhl_3_29_0.13.png
	diff out_haralick.png expected_results/windmill_haralick_0.4.png

clean:
	rm -f edges
	rm -f out_*.png

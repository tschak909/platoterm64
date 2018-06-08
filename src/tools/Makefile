
all: mk_scale mk_font

mk_font: mk_font.c
	$(CC) -o mk_font mk_font.c

mk_scale: mk_scale.c
	$(CC) -lm -o mk_scale mk_scale.c

clean:
	rm -f *~ mk_scale mk_font

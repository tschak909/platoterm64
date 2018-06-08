#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) 
{ 
    int i=0;
    double sx;
    double sy;

    if (argc < 3) {
	fprintf(stderr,"usage: mk_scale x y\n");
	return 1;
    }

    sx = strtod(argv[1],NULL) / 512;
    sy = strtod(argv[2],NULL) / 512;

    printf(
	   "/* ! Autogenerated by tools/mk_scale.c */\n\n"
	   "#ifndef SCALE_H\n"
	   "#define SCALE_H\n\n"
	   "unsigned short scalex[]={\n");

    for (i=0;i<512;i++) { 
	printf("%d,",(int)floor(i*sx));
	if( i % 8 == 7 )
	    printf("\n");
    }
    printf("};\n\n");
    
    printf("unsigned short scaley[]={\n");
    for (i=511;i>=0;i-=1) {
	printf("%d,",(int)floor(i*sy));
	if (i % 8 == 0 )
	    printf("\n");
    } 
    printf("};\n"
	   "#endif  /* scale_h */\n"); 
    /*
    printf("unsigned int fontptr[]={"); 
    for (i=0;i<160;i++) { 
	printf("%d,",i*6); 
    } 
    printf("};\n\n");
    */
    return 0;
}

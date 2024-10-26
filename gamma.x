// Generate an LED gamma-correction table for Arduino sketches.
// Written in Processing (www.processing.org), NOT for Arduino!
// Copy-and-paste the program's output into an Arduino sketch.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

float ggamma   = 1.5; // Correction factor
int   max_in  = 255, // Top end of INPUT range
      max_out = 255; // Top end of OUTPUT range

int main(int argc, char *argv[]) {
  printf("const uint8_t gamma8[] = {");
  for(int i=0; i<=max_in; i++) {
    if(i > 0) printf(",");
    if((i & 15) == 0) printf("\n  ");
    printf("%3d",
      255 - (int)(pow((float)(255 - i) / (float)max_in, ggamma) * max_out + 0.5));
  }
  printf(" };\n");
}

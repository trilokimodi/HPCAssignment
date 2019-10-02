#include <stdlib.h>
#include <stdio.h>
 
int main(void)
{
  const int dimx = 100, dimy = 100;
  int i, j;
  FILE *fp = fopen("first.ppm", "wb"); /* b - binary mode */
  (void) fprintf(fp, "P6\n%d %d\n10\n", dimx, dimy);
  for (j = 0; j < dimy; ++j)
  {
    for (i = 0; i < dimx; ++i)
    {
      static unsigned char color[3];
      color[0] = i % 10;  /* red */
      color[1] = j % 10;  /* green */
      color[2] = (i * j) % 10;  /* blue */
      (void) fwrite(color, 1, 3, fp);
    }
  }
  (void) fclose(fp);
  return 1;
}
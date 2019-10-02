#include <stdlib.h>
#include <stdio.h>
 
int main(void)
{
  const int dimx = 800, dimy = 800;
  int i, j;
  FILE *fp = fopen("first.ppm", "wb"); /* b - binary mode */
  (void) fprintf(fp, "P6\n%d %d\n7\n", dimx, dimy);
  for (j = 0; j < dimy; ++j)
  {
    for (i = 0; i < dimx; ++i)
    {
      static unsigned char color[3];
      color[0] = i % 7;  /* red */
      color[1] = j % 7;  /* green */
      color[2] = (i * j) % 7;  /* blue */
      (void) fwrite(color, 1, 3, fp);
    }
  }
  (void) fclose(fp);
  return 1;
}
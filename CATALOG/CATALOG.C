#include <stdio.h>

FILE *disk;
long track,sector;
unsigned char buffer[256];

long address(long track, long sector)
{
     return ((track * 16L) + sector) * 256;
}

void get_sector(long track, long sector, unsigned char *buffer)
{
     fseek(disk,address(track,sector),SEEK_SET);
     fread(buffer,256,1,disk);
}

void show_sector(unsigned char *buffer)
{
     int i,j;
     
     for(i=0;i<16;i++) {
          for(j=0;j<16;j++) {
               printf("%3x",buffer[i*16+j]);
          }
          printf("\n");
     }
}

void showattrib(unsigned char *filedesc) {
          
     if (filedesc[0] == 255)
          printf("D");
     else
          printf(" ");
          
     if (filedesc[2] > 127) {
          printf("*");
          filedesc[2] -= 128;
     }
     else
          printf(" ");
          
     switch (filedesc[2]) {
          case 0 : printf("T");
                   break;
          case 1 : printf("I");
                   break;
          case 2 : printf("A");
                   break;
          case 4 : printf("B");
                   break;
          case 8 : printf("S");
                   break;
          case 10: printf("R");
                   break;
          case 20: printf("a");
                   break;
          case 40: printf("b");
                   break;
          default: printf("?");
                   break;
     }
     
     printf(" %3d ",256*filedesc[34]+filedesc[33]);
}
          
void showname(unsigned char *name) {
     int i;
     for (i=3;i < 33;i++)
          if (name[i] > 127)
               printf("%c",name[i]-128);
     printf("\n");
}

void catalog(void)
{
     long track,sector;
     int byte,done;
     
     get_sector(17L,0L,buffer);
     done = 0;
     while (!done) {
          track = (long) buffer[1];
          sector = (long) buffer[2];
          if ((track > 34) || (sector > 15)) {
               done = 1;
               printf("ERROR: Not a standard DOS 3.3 disk image\n");
          }
          else {
               get_sector(track,sector,buffer);
               for(byte=11;byte < 222;byte += 35) {
                    if (buffer[byte] == 0)
                         done = 1;
                    else {
                         if (buffer[byte] != 255) {
                              showattrib(&buffer[byte]);
                              showname(&buffer[byte]);
                         }
                    }
               }
          }
     }
}         

main(int argc, char *argv[])
{
     if (argc > 1) {
          if ((disk = fopen(argv[1],"rb")) == NULL)
               printf("ERROR: unable to catalog %s\n",argv[1]);
          else {
               catalog();
               fclose(disk);
          }
     }
     else
          printf("Usage: catalog DISKNAME\n");
}


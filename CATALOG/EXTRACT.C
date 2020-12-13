#include <stdio.h>
#include <string.h>

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

int samename(unsigned char *source, unsigned char *buffer)
{
     int i;
     unsigned char name[30];
     for (i=0; i < 30; i++)
          name[i] = buffer[i]-128;
     return !strnicmp(source,name,strlen(source));
}

int wildstrcmp(char *buffer, char *pattern)
{
     int obptr, patptr, done, match, wild, i;
     char object[30];
     obptr = 0;
     patptr = 0;
     done = 0;
     match = 1;
     wild = 0;

     for (i=0; i < 30; i++)
          object[i] = buffer[i]-128;

     i=30;
     while ((object[i-1] == 32) && (i > 0))
          i--;
     object[i] = '\0';

     while (!done) {
          if (wild) {
               if (object[obptr++] == pattern[patptr]) {
                    wild = 0;
                    patptr++;
               } else if (obptr == strlen(object)) {
                    match = 0;
                    done = 1;
               }
          } else if (pattern[patptr] == '*') {
               if (patptr == strlen(pattern)-1)
                    done = 1;
               else {
                         wild = 1;
               }
               while (pattern[patptr] == '*')
                    patptr++;
               
          } else if (pattern[patptr++] != object[obptr++]) {
               match = 0;
               done = 1;
          }
          if (!done && !wild) {
               if (obptr == strlen(object)) {
                    done++;
                    if (done == 1)
                         while (pattern[patptr] == '*')
                              patptr++;
               }
               if (patptr == strlen(pattern)) done++;
               if (done == 1) {
                    match = 0;
               }
          }
     }
     return match;
}

void getfile(int track, int sector, char *rawname)
{
     unsigned char tslist[256], buffer[256], fname[30];
     FILE *fp;
     int spointer, done, i;


     for (i=0; i < 30; i++)
          fname[i] = rawname[i]-128;

     i=30;
     while ((fname[i-1] == ' ') && (i > 0))
          i--;
     fname[i] = '\0';
     
     for (i=0; i < strlen(fname); i++)
          if ((fname[i] == ' ') || (fname[i] == '/') || (fname[i] == '\\'))
               fname[i] = '_';

     get_sector((long)track,(long)sector,tslist);
     
     fp = fopen(fname,"wb");
     spointer = 12;
     done = 0;
     
     while (!done) {
          get_sector((long)tslist[spointer],(long)tslist[spointer+1],buffer);
          fwrite(buffer,256,1,fp);
          spointer++;
          spointer++;
          if (spointer == 256) {
               spointer = 12;
               get_sector((long)tslist[1],(long)tslist[2],tslist);
          }    
          else if (tslist[spointer] == 0 && tslist[spointer+1] == 0)
               done = 1;
     }
     fclose(fp);
}

void extract(unsigned char *source)
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
                              if (wildstrcmp(&buffer[byte+3],source)) {  
                                   showattrib(&buffer[byte]);
                                   showname(&buffer[byte]);
                                   getfile(buffer[byte],buffer[byte+1],&buffer[byte+3]);
                              }
                         }
                    }
               }
          }
     }
}         

main(int argc, unsigned char *argv[])
{
     
     if (argc > 1) {
          if ((disk = fopen(argv[1],"rb")) == NULL)
               printf("ERROR: unable to extract from disk image %s\n",argv[1]);
          else {
               if (argc > 2) {
                    extract(argv[2]);
               }
               else
                    extract("");
               fclose(disk);
          }
     }
     else {
          printf("Usage: extract DISKNAME [file_1] [file_2] [file_n]\n\n");
          printf("       Filenames are CASE SENSITIVE\n");
          printf("         and may contain multiple * wildcards.\n\n");
          printf("Examples:\n");
          printf("       - extract system.dsk H*O\n");
          printf("           extracts all files beginning with H, ending with O\n\n");
          printf("       - extract system.dsk H*L*O\n");
          printf("           extracts all files beginning with H, ending with O\n");
          printf("           and an L in the middle\n");          
     }
}


#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <pthread.h>
#include <time.h>
//#include <sys/time.h>

#include <openssl/md2.h>
#include <openssl/md4.h>
#include <openssl/md5.h>

#include "crc16.c"

#define TARGET "abcdefgh"
#define LIMIT limit
#define START start

#define HASHLEN 16

struct timeval start;
struct timeval end;

double delta;

char hashMode = '5';

char org[1024];

unsigned char HexChar (char c)
{
    if ('0' <= c && c <= '9') return (unsigned char)(c - '0');
    if ('A' <= c && c <= 'F') return (unsigned char)(c - 'A' + 10);
    if ('a' <= c && c <= 'f') return (unsigned char)(c - 'a' + 10);
    return 0xFF;
}

int hex2data(unsigned char *data, const unsigned char *hex, unsigned int len)
{
    unsigned const char *pos = hex;
    char *endptr;
    size_t count = 0;

    if ((hex[0] == '\0') || (strlen(hex) % 2)) {
        //hexstring contains no data
        //or hexstring has an odd length
        return -1;
    }

    for(count = 0; count < len; count++) {
        char buf[5] = {'0', 'x', pos[0], pos[1], 0};
        data[count] = strtol(buf, &endptr, 0);
        pos += 2 * sizeof(char);

        if (endptr[0] != '\0') {
            //non-hexadecimal character encountered
            return -1;
        }
    }

    return 0;
}

char *StringPadRight(char *string, int padded_len, char *pad) {
    int len = (int) strlen(string);
    if (len >= padded_len) {
        return string;
    }
    int i;
    for (i = 0; i < padded_len - len; i++) {
        strcat(string, pad);
    }
    return string;
}

unsigned short _crc(char* data)
{
 //crcInit();
 //return crcFast(data, strlen(data));

 return crc16_ccitt(data, strlen(data));
}

char *compute_hash(const char *str, int len) {
	switch(hashMode) {
		case '2':
			return MD2(str,len,NULL);
			break;
		case '4':
			return MD4(str,len,NULL);
			break;
		case '5':
			return MD5(str,len,NULL);
			break;
		default:
			return MD5(str,len,NULL);
	}

}

void getHash(char* target, unsigned long limit, long start)
{
 printf("\t\tTARGET: ");
 for (int i_2 = 0; i_2 < HASHLEN; i_2++)
 {
   if (i_2 > 0) printf(":");
   printf("%02X", (unsigned char)target[i_2]);
 }

 printf("\n");


 //int hash;
 char* hash = malloc(HASHLEN);
 int found = NULL;
 unsigned long i = START;
 char *buf = malloc(HASHLEN);

 for (i=START; i < LIMIT; i++)
 {
  printf("TRYING: 0x%x", i);
  memcpy(buf, &i, sizeof(i));
  //hash = _crc(buf);
  memcpy( hash, compute_hash( buf, strlen(buf) ), 16  );
  printf("\t - GOT: ");
  for (int i_1 = 0; i_1 < HASHLEN; i_1++)
  {
    if (i_1 > 0) printf(":");
    printf("%02X", (unsigned char)hash[i_1]);
  }
  printf("\t - LENGTH: %d\n", strlen((char*)&i));

  if (!strcmp(hash, target))
  {
   printf("\n\nHASH FOUND - \n\t");

   for (int i_2 = 0; i_2 < HASHLEN; i_2++)
   {
     if (i_2 > 0) printf(":");
     printf("%02X", (unsigned char)hash[i_2]);
   }

   strcpy(org,(char*)&i);
   printf("\n\t%s\n",org);
//   gettimeofday(&end, NULL);

//   delta = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
//   printf("\tFound in %d seconds.  %d hashes/second.\n\n", delta, i/(delta));
   printf("\n\n");

   exit(0);
  }
 }

 free(buf);
 free(hash);

 printf("NOT FOUND: \tTARGET: ");

 for (int i_2 = 0; i_2 < HASHLEN; i_2++)
 {
   if (i_2 > 0) printf(":");
   printf("%02X", (unsigned char)target[i_2]);
 }

 printf("  LIMIT - 0x%x  START - 0x%x\n", LIMIT, START);

}

int main(int argc, char** argv)
{
 if (argc != 5) { printf("ERROR: Usage = ./mdBrute <target> <start> <limit> <hashmode>\n\tThe HASHMODE parameter can be '2', '4' or '5' for MD2, MD4 and MD5 respectivley. Default is MD5\n"); exit(1); }

 char *t = malloc(HASHLEN+1);
 memset( t, 0, HASHLEN);
 strcpy(org, argv[1]);
 hex2data(t, argv[1], strlen(argv[1]));
 StringPadRight(t, HASHLEN, "\0");

 hashMode = argv[4][0];

 if ( atoi(argv[3]) ) {
// gettimeofday(&start, NULL);
 getHash( t , atoi(argv[3]), atoi(argv[2]));
 }
 else
 {
// gettimeofday(&start, NULL);
 getHash( t, 0xFFFFFFFFFFFFFFFF, atoi(argv[2]));
 }

 return 0;
}

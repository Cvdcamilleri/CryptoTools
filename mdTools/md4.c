#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <openssl/md2.h>
#include <openssl/md4.h>
#include <openssl/md5.h>

#define MODE '4'
#define HASHLEN 16

char *compute_hash(const char *str, int len) {
        switch(MODE) {
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

void main(int argc, char **argv)
{
  if (argc != 2) { printf("USAGE: md5 <text>\n"); exit(1); }

  char *hash = malloc(17);
  memcpy(hash, compute_hash(argv[1], strlen(argv[1])), 16);

  for (int i_1 = 0; i_1 < HASHLEN; i_1++)
  {
    if (i_1 > 0) printf("");
    printf("%02x", (unsigned char)hash[i_1]);
  }
  printf("  %s\n", argv[1]);

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <bsd/md5.h>

#define MAX_KEY_LENGTH    8

int main(int argc, const char *argv[]) {
  MD5_CTX md5;
  char hash[33];
  char digest[16];
  char hex[3];
  char guess[MAX_KEY_LENGTH];
  char guess_map[MAX_KEY_LENGTH];
  char key[] = "8d7b356eae43adcd6ad3ee124c3dcf1e";
  char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  unsigned int found, nchars, nkey, i, j;
  unsigned long long int possibilities;

  found = 0;
  nchars = strlen(characters);
  nkey = strlen(key);

  for(possibilities = nchars, i = 1; i < MAX_KEY_LENGTH; ++i) {
    possibilities *= nchars;
  }

  fprintf(stdout, "%llu possibilites to lookup for.\n", possibilities);

  for(i = 1; i < MAX_KEY_LENGTH && found == 0; ++i) {
    for(j = 0; j < i; ++j) {
      guess_map[j] = 0;
    }

    guess[i] = '\0';

    while(possibilities > 0 && found == 0) {
      for(j = 0; j < i; ++j) {
        guess[j] = characters[guess_map[j]];
      }

      MD5Init(&md5);
      MD5Update(&md5, guess, i);
      MD5Final(digest, &md5);

      for(j = 0; j < sizeof digest; ++j) {
        snprintf(hex, sizeof hex, "%2x", digest[j] & 0xFF);
        hash[j * 2] = (hex[0] == ' ') ? '0' : hex[0];
        hash[(j * 2) + 1] = hex[1];
      }

      hash[32] = '\0';

      if(strncmp(hash, key, sizeof key) == 0) {
        found = 1;
      }

      guess_map[0]++;

      for(j = 0; guess_map[j] > nchars - 1; ++j) {
        guess_map[j] = 0;
        guess_map[j + 1]++;
      }

      if(j > i) {
        break;
      }

      --possibilities;
    }
  }

  if(found == 1) {
    fprintf(stdout, "Key is \"%s\"\n", guess);
  } else {
    fprintf(stdout, "Key not found!\n");
  }

  return 0;
}

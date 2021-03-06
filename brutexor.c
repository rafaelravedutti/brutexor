/*
 * brutexor - Brute-force attack and XOR decryption
 *
 * Copyright (C) 2016  Rafael Ravedutti Lucio Machado
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <bsd/md5.h>

#define MAX_KEY_LENGTH    8

int main(int argc, const char *argv[]) {
  MD5_CTX md5;
  char result[64];
  char hash[33];
  unsigned char digest[16];
  char hex[3];
  unsigned char guess[MAX_KEY_LENGTH];
  unsigned char guess_map[MAX_KEY_LENGTH];
  char key[] = "8d7b356eae43adcd6ad3ee124c3dcf1e";
  char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char chain[] = { 0x0b, 0x34, 0x30, 0x20, 0x2f, 0x27, 0x05, 0x22, 0x05, 0x29,
                   0x21, 0x28, 0x22, 0x26, 0x19, 0x34, 0x23, 0x22, 0x27, 0x2d  };
  unsigned int found, nchars, i, j;
  unsigned long long int possibilities;

  found = 0;
  nchars = strlen(characters);

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

  for(i = 0, j = 0; i < 20; ++i, j = (j + 1) % strlen((char *) guess)) {
    result[i] = chain[i] ^ guess[j];
  }

  result[20] = '\0';
  fprintf(stdout, "Result is \"%s\"\n", result);
  return 0;
}

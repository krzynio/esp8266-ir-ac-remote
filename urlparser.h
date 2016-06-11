/**
 * queryString: the string with is to be parsed.
 * WARNING! This function overwrites the content of this string. Pass this function a copy
 * if you need the value preserved.
 * results: place to put the pairs of param name/value.
 * resultsMaxCt: maximum number of results, = sizeof(results)/sizeof(*results)
 * decodeUrl: if this is true, then url escapes will be decoded as per RFC 2616
 */

void percentDecode(char *src);

char *strsep(char **from, const char *delim) {
    char *s, *dp, *ret;

    if ((s = *from) == NULL)
        return NULL;

    ret = s;
    while (*s != '\0') {
        /* loop until the end of s, checking against each delimiting character,
         * if we find a delimiter set **s to '\0' and return our previous token
         * to the user. */
        dp = (char *)delim;
        while (*dp != '\0') {
            if (*s == *dp) {
                *s = '\0';
                *from = s + 1;
                return ret;
            }
            dp++;
        }
        s++;
    }
    /* end of string case */
    *from = NULL;
    return ret;
}
char *
strchrnul (const char *s, int c_in)
{
  char c = c_in;
  while (*s && (*s != c))
    s++;

  return (char *) s;
}

int parseUrlParams(char *queryString, char *results[][2], int resultsMaxCt, boolean decodeUrl) {
  int ct = 0;

  while (queryString && *queryString && ct < resultsMaxCt) {
    results[ct][0] = strsep(&queryString, "&");
    results[ct][1] = strchrnul(results[ct][0], '=');
    if (*results[ct][1]) *results[ct][1]++ = '\0';

    if (decodeUrl) {
      percentDecode(results[ct][0]);
      percentDecode(results[ct][1]);
    }

    ct++;
  }

  return ct;
}

/**
 * Perform URL percent decoding.
 * Decoding is done in-place and will modify the parameter.
 */

void percentDecode(char *src) {
  char *dst = src;

  while (*src) {
    if (*src == '+') {
      src++;
      *dst++ = ' ';
    }
    else if (*src == '%') {
      // handle percent escape

      *dst = '\0';
      src++;

      if (*src >= '0' && *src <= '9') {
        *dst = *src++ - '0';
      }
      else if (*src >= 'A' && *src <= 'F') {
        *dst = 10 + *src++ - 'A';
      }
      else if (*src >= 'a' && *src <= 'f') {
        *dst = 10 + *src++ - 'a';
      }

      // this will cause %4 to be decoded to ascii @, but %4 is invalid
      // and we can't be expected to decode it properly anyway

      *dst <<= 4;

      if (*src >= '0' && *src <= '9') {
        *dst |= *src++ - '0';
      }
      else if (*src >= 'A' && *src <= 'F') {
        *dst |= 10 + *src++ - 'A';
      }
      else if (*src >= 'a' && *src <= 'f') {
        *dst |= 10 + *src++ - 'a';
      }

      dst++;
    }
    else {
      *dst++ = *src++;
    }

  }
  *dst = '\0';
}
#include "sexps.h"
#include "misc/types.h"
#include "misc/misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free(void* ptr) {
   ptr = realloc(ptr, 0); //"ptr = " to shut up gcc
}

void error(const char* str, enum error_type how_bad) {
   printf("error occured: %s\nexiting...", str);
   exit(-1);
}


int* _get_next_quotes(const char* str, size_t len, int i) {
   len = (len == 0) ? strlen(str) : len;
   
   int* to_return = (int*)malloc(sizeof(int) * 2);
   to_return[0] = to_return[1] = -1;

   if (len <= i)
      return to_return;
   
   bool_t have_1st_quote = _false;
   while (i < len) {
      if (str[i] == '"') {
         if (!have_1st_quote) {
            to_return[0] = i;
            have_1st_quote = _true;
         }

         else {
            to_return[1] = i;
            break;  
         }
      }
      i++;
   }

   return to_return;
}

//TODO: optimize
int _increment_counter(const char* str, size_t len, int i, bool_t init) {
   len = (len == 0) ? strlen(str) : len;

   int* quotes;
 
   int to_add = 1;
   if (init)
      to_add = 0;
  
   quotes = _get_next_quotes(str, len, i);
   
   if (quotes[0] == quotes[1]) {
      free(quotes);
      return i + to_add; 
   }
    
   if (i >= (quotes[0] - to_add) && i <= quotes[1]) {
      free(quotes);
      return quotes[1] + 1;
   }
   free(quotes);
   return i + to_add; 
}

bool_t parse_type(struct Sexps* s) {}


struct Sexps* parse_sexps(const char* sexps, size_t len) {
   len = (len == 0) ? strlen(sexps) : len; 

   int begin_paren, end_paren, num_open_paren, num_closed_paren;
   begin_paren = end_paren = -1;
   num_open_paren = num_closed_paren = 0;
  
   struct Sexps* to_ret = (struct Sexps*)malloc(sizeof(struct Sexps)); 
   to_ret->str_val = sexps;
   to_ret->atom = 0;
   if (!parse_type(to_ret)) {
      free(to_ret);
      error("could not deduce the type of sexps", BAD_INPUT);
      return NULL;
   }
   to_ret->sub_sexps = NULL;
   to_ret->sub_sexps_len = to_ret->size_sub_sexps = 0;
   
   if (to_ret->atom)
      return to_ret;
    
   int counter = 0; 
   while (counter < len) { 
      int i; 
      for (i = _increment_counter(sexps, len, counter, _true);
           i < len;
           _increment_counter(sexps, len, i, _false))
      {
         if (sexps[i] == '(') {
            if (num_open_paren == 0)
               begin_paren = i;
           num_open_paren++;
         }

         if (sexps[i] == ')') {
            if (num_open_paren == 0) {
               error("found close parenthesis without matching parenthesis", BAD_INPUT);
               return NULL;
            }
            num_closed_paren++;
            
            if (num_closed_paren == num_open_paren) {
               end_paren = i;
               break;
            }
         }
      }

      if (begin_paren == -1 || end_paren == -1)
        continue;

      if (to_ret->sub_sexps == NULL) {
         to_ret->sub_sexps = (struct Sexps**)malloc(sizeof(struct Sexps*));
         to_ret->sub_sexps_len = to_ret->size_sub_sexps = 1;
      }
   
      //TODO: change to container type
      if (to_ret->sub_sexps_len + 1 == to_ret->size_sub_sexps)
         to_ret = realloc(to_ret, to_ret->size_sub_sexps * 2); 

      to_ret->sub_sexps[to_ret->sub_sexps_len++] = (struct Sexps*)malloc(sizeof(struct Sexps));
    
      char *str_sub_sexps;
      
   }
}




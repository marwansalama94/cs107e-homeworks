#include "strings.h"

void *memset(void *s, int c, size_t n){
    for (size_t i = 0; i < n ; i++){
      *( ((unsigned char*)s) + i) = (unsigned char)c;
    }
    return s;
}

void *memcpy(void *dst, const void *src, size_t n){
    for(size_t i = 0; i < n; i++){
        *( ((unsigned char*)dst) + i ) = *( ((unsigned char*)src) + i );
    }
    return dst;
}

int strlen(const char *s){
    int n = 0;
    while(*s++){
        n++;
    }
    return n;
}

int strcmp(const char *s1, const char *s2){
    int s1_length = strlen(s1);
    int s2_length = strlen(s2);
    if (s1_length != s2_length) return s1_length - s2_length;

    while( (*s1 == *s2) && *s1){
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

static unsigned int strtonum_hex(const char *str,const char **endptr){
    unsigned int num = 0;
    str+= 2;
    while(*str){
        num<<= 4;//a place to get the new 4 bits
        if(*str >= '0' && *str <='9'){
            num += ((*str)-'0');
        }else if (*str >= 'A' && *str <='F'){
            num += 10+((*str)-'A');
        }else if (*str >= 'a' && *str <='f'){
            num += 10+((*str)-'a');
        }else{
            *endptr = str;
            break;
        }
        str++;
    }
    return num;
}

static unsigned int strtonum_decimal(const char *str, const char** endptr){
    unsigned int num = 0;
    while(*str){
        if(*str >= '0' && *str <='9'){
            num*= 10;
            num += *str-'0';
        }else{
            *endptr = str;
            break;
        }
        str++;
    }
    return num;
}

unsigned int strtonum(const char *str, const char **endptr){
    if( str[0] == '0' && (str[1] == 'x' || str[1] == 'X') ){
        return strtonum_hex(str,endptr);
    }else{
        return strtonum_decimal(str,endptr);
    }
}

int strlcat(char *dst, const char *src, int maxsize){
    int end_of_dst = strlen(dst);
    int offset = 0;
    while(*(src+offset) && (maxsize-(end_of_dst+offset+1)) > 0){
        *(dst+ end_of_dst + offset) = *(src+offset);
        offset++;
    }
    *(dst+end_of_dst+offset)='\0';
    return (end_of_dst+offset);
}

#include "printf.h"
#include "string.h"

static unsigned int offset_calc(int digit_count, int bufsize, int min_width){
    if(digit_count < bufsize && digit_count >= min_width){
        return digit_count;
    }else if (min_width > digit_count && min_width < bufsize){
        return min_width;
    }else{
        return bufsize;
    }
}


int unsigned_to_base(char *buf, int bufsize, unsigned int val, int base, int min_width){
    unsigned int ncount = 0;
    int offset;
    int digit_count = digits_count(val);
    if(!bufsize)return 0;
    offset = offset_calc(digit_count, bufsize, min_width);
    for (int i = 1 ; i <= offset; i++){
        *(buf+offset-i) = ((val%base)<10)? '0'+ val % base : 'A'+ (val%base)%10 ;
        val /= base;
        ncount++;
    }
    *(buf+offset) = '\0';
    return ncount;
}

int signed_to_base(char *buf, int bufsize, int val, int base, int min_width){
    if(val < 0){
        *buf = '-';
        return unsigned_to_base(buf+1,bufsize-1, (val*-1),base,min_width-1)+1;
    }
    return unsigned_to_base(buf,bufsize,val,base,min_width);
}

int vsnprintf(char *buf, int bufsize, const char *format, va_list args){
    //TODO:to implement
}

int snprintf(char *buf, int bufsize, const char *format, ...) __attribute__ ((format (printf, 3, 4))){
    //TODO:clean up a little bit and refractor in vsnprintf
    int count = 0;
    int padding = 0;
    va_list ap;
    va_start(ap,format);

    while(((bufsize-1)-count) && *format){
        if (*format != '%'){
            *(buf+count) = *(format);//it is just a character
            format++;
            count++;
        }
        else{//case: placeholder
            format++;
            const char * ptr;
            int offset;
            switch(*format){
                case 'c':
                    *(buf+count) = (char)va_arg(ap,int);
                    format++;
                    count++;
                    break;
                case 's':
                    count+= strlcat(buf+count,va_arg(ap,char*),bufsize-count-1);
                    format++;
                    break;
                default:
                    padding = strtonum(format, &ptr);
                    switch(*ptr){
                        case 'x':
                            offset = strlcat((buf+count), "0x",3) + unsigned_to_base(buf+count+2, bufsize-count-1, va_arg(ap,int), 16, padding-2);
                            count += offset;
                            format += ptr-format+1;
                            break;
                        case 'd':
                            offset = signed_to_base(buf+count, bufsize-count-1, va_arg(ap,int), 10, padding);
                            count+= offset;
                            format+= (ptr-format)+1;
                            break;
                        case 'p':
                            offset = strlcat((buf+count), "0x",3) + unsigned_to_base(buf+count+2, bufsize-count-1, va_arg(ap,long), 16, 12);
                            count += offset;
                            format += ptr-format+1;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }

    }

    va_end(ap);
    *(buf+count) = '\0';
    return count;
}


int printf(const char *format, ...) __attribute__ ((format (printf, 1, 2))){
    //TODO:to implement
}

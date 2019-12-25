#include "printf.h"
#include "strings.h"
#include "uart.h"

static unsigned int offset_calc(int digit_count, int bufsize, int min_width){
    if(digit_count < bufsize && digit_count >= min_width){
        return digit_count;
    }else if (min_width > digit_count && min_width < bufsize){
        return min_width;
    }else{
        return bufsize;
    }
}

static unsigned int digits_count(unsigned int val){
    unsigned int count = 0;
    while(val){
        count++;
        val/= 10;
    }
    return (count > 0 )? count: 1;
}

int unsigned_to_base(char *buf, int bufsize, unsigned long int val, int base, int min_width){
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
    //TODO:clean up a little bit
    int count = 0;
    int padding = 0;

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
                    *(buf+count) = (char)va_arg(args,int);
                    format++;
                    count++;
                    break;
                case 's':
                    count+= strlcat(buf+count,va_arg(args,char*),bufsize-count-1);
                    format++;
                    break;
                case '%':
                    *(buf+count) = *(format);
                    format++;
                    count++;
                    break;    
                default:
                    padding = strtonum(format, &ptr);
                    switch(*ptr){
                        case 'x':
                            offset = strlcat((buf+count), "0x",3) + unsigned_to_base(buf+count+2, bufsize-count-1, va_arg(args,int), 16, padding-2);
                            count += offset;
                            format += ptr-format+1;
                            break;
                        case 'd':
                            offset = signed_to_base(buf+count, bufsize-count-1, va_arg(args,int), 10, padding);
                            count+= offset;
                            format+= (ptr-format)+1;
                            break;
                        case 'p':
                            offset = strlcat((buf+count), "0x",3) + unsigned_to_base(buf+count+2, bufsize-count-1, va_arg(args,long), 16, 12);
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

    *(buf+count) = '\0';
    return count;
}

__attribute__ ((format (printf, 3, 4))) int snprintf(char *buf, int bufsize, const char *format, ...){
    int count = 0;
    va_list ap;
    va_start(ap,format);
    count = vsnprintf(buf, bufsize, format, ap);
    va_end(ap);
    return count;
    
}


__attribute__ ((format (printf, 1, 2))) int printf(const char *format, ...){
    char buf[1024];
    int characters_count = 0;
    va_list ap;
    va_start(ap,format);
    characters_count = vsnprintf(buf, 1024, format, ap);
    va_end(ap);
    int count;
    for(count = 0 ; count <= characters_count; count++){
        uart_putchar(buf[count]);
    }
    return count;
}

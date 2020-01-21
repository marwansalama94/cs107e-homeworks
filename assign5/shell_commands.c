#include "shell_commands.h"
#include "shell.h"
#include "libs/strings.h"
#include "pi.h"

extern const command_t commands[];
extern formatted_fn_t _print_fn;
#define cmds_count 5
#define MANY_ARGS -1
#define FEW_ARGS -2
#define CMD_NOT_FOUND -3
#define BAD_CALL -4

int cmd_echo(int argc, const char *argv[]){
    for(int i = 1; i < argc; i++){
        _print_fn("%s ",argv[i]);
    }
    _print_fn("\n");
    return 0;
}

int cmd_help(int argc, const char *argv[]){
    int flag = 0;
    //_print_fn("args count : %d\n",argc);
    if(argc == 1){
        for(int i = 0; i < cmds_count; i++){
            _print_fn("%s : %s\n",commands[i].name, commands[i].description);
        }
        return flag;
    }else if (argc == 2){
        for (int i = 0; i < cmds_count; i++){
            if(!strcmp(argv[1], commands[i].name)){
                _print_fn("%s : %s\n",argv[1], commands[i].description);
                return flag;
            }
        }
        return BAD_CALL;
    }else{
        flag = BAD_CALL;
    }
    return flag;
}

int cmd_reboot(int argc, const char* argv[]){
    pi_reboot();
    return 0;
}

int cmd_peek(int argc, const char* argv[]){
    int ret = 0;
    if(argc == 1 || argc > 2){
        _print_fn("error: peek expects 1 argument [address]\n");
        ret = -1;
    }else{
        unsigned int address = strtonum(argv[1],&argv[1]);
        if(!address && argv[1][0] != '0'){
            _print_fn("error: peek cannot convert '%s'\n",argv[1]);
            ret = -1;
        }
        else if ( !(address & 0x3) ){
           unsigned int val = *((int*)((long)address));
            //_print_fn("theval:%x\n",val);
            _print_fn("%10x : %d\n",address, val);
            ret = 0;
        }else{
            _print_fn("error: peek address must be 4-byte aligned\n");
            ret = -1;
        }
    }
    return ret;
}

int cmd_poke(int argc, const char* argv[]){
    int ret = 0;
    if(argc == 1 || argc == 2 || argc > 3){
        _print_fn("poke expects 2 arguments [address] [value]\n");
        ret = -1;
    }else{
        unsigned int address = strtonum(argv[1],&argv[1]);
        unsigned int value = strtonum(argv[2],&argv[2]);
        //_print_fn("given address = %08x, given value = %d\n",address,value);
        if(!address && argv[1][0] != '0'){
            _print_fn("error: poke cannot convert '%s'\n",argv[1]);
            ret = -1;
        }else if (!value && argv[2][0] != '0'){
            _print_fn("error: poke cannot convert '%s'\n",argv[2]);
            ret = -1;
        }else if ( !(address & 0x3) ){
            //_print_fn("got here with address:%08x, value:%d\n", address, value);
            *((unsigned int*)((long)address)) = (unsigned int)value;
            ret = 0;
        }else {
             _print_fn("error: peek address must be 4-byte aligned\n");
             ret = -1;
        }
    }
    return ret;
}

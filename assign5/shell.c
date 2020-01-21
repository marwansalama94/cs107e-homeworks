#include "shell.h"
#include "keyboard.h"
#include "shell_commands.h"
#include "libs/strings.h"
#include "libs/malloc.h"

const command_t commands[] = {
  {"help", "[cmd] prints a list of commands or description of cmd", cmd_help},
  {"echo", "<...> echos the user input to the screen", cmd_echo},
  {"reboot","reboot the Raspberry Pi back to the bootloader", cmd_reboot},
  {"peek","Prints the contents (4 bytes) of memory at address.",cmd_peek},
  {"poke","Stores `value` into the memory at `address`.",cmd_poke},
};

formatted_fn_t _print_fn;

void shell_init(formatted_fn_t print_fn){
    _print_fn = print_fn;
}

void shell_readline(char buf[], int bufsize){
    int ch_count = 0;
    char ch = 0;
    while(ch_count < bufsize-1){
        ch = keyboard_read_next();
        if(ch == '\n'){
            _print_fn("\n");
            break;
        }//end of the command
        else if (ch == '\b'){
            if(ch_count){
                _print_fn("\b \b");
                ch_count--;
            }
        }else if(ch){
            //add the character to the buffer
            _print_fn("%c",ch);
            buf[ch_count] = ch;
            ch_count++;
        }
    }
    buf[ch_count]='\0';
}

static char* string_tokenizer_helper(const char* str,int count){
    char* substr = (char*)malloc(count+1);
    if(substr){
        substr = memcpy(substr,str,count);
        substr[count] = '\0';
    }
    return substr;
}

static int string_tokenizer(const char* str, char* tokens[]){
    //tokens = (char**)malloc(sizeof(int));
   // _print_fn("%x\n",tokens);
    int tokens_count = 0;
    const char* base = str;
    int ch_count = 0;
    int begin_offset = 0; 
    while(tokens_count < 10){
       if(*str != ' ' && *str){
            ch_count++;
       }else if(!*str){
           //tokens = realloc(tokens, sizeof(int) * (tokens_count+1));
           //if(!tokens)break;
           tokens[tokens_count] = string_tokenizer_helper(base+begin_offset,ch_count);
           tokens_count++;
           break;
       }
       else{
            //tokens = realloc(tokens, sizeof(int) * (tokens_count+1));
            //if(!tokens)break;
            tokens[tokens_count] = string_tokenizer_helper(base+begin_offset,ch_count);
            begin_offset += ch_count+1;
            ch_count = 0;
            tokens_count++;
        }
       
        str++;

    }
    return tokens_count;
}


int shell_evaluate(const char* line){
    int tokens_count = 0;
    int flag = 0;
    char* tokens[10];
    tokens_count = string_tokenizer(line, tokens);
    for(int i = 0; i < sizeof(commands)/sizeof(command_t); i++){
        if(!strcmp(commands[i].name,tokens[0])){
            commands[i].fn(tokens_count, tokens);
            flag = 1;
            break;
        }
    }
    for(int i = tokens_count-1; i >= 0; i--){
        free(tokens[i]);
    }

    if(!flag){
        _print_fn("command not found\ntype 'help' for the list of available commands\n");
        return -1;
    }
    return 0;
}

void shell_run(void){
    //loop forever
    //display prompt
    //read line of input from user
    //evaluate command (parse and execute)
    char buf[1024];
    while(1){
        _print_fn("Pi>");
        shell_readline(buf,1024);
        shell_evaluate(buf);
    }
}

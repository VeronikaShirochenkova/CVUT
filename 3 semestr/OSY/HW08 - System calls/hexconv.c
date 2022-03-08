#define STDIN_FILENO   0
#define STDOUT_FILENO  1

#define SYSCALL_EXIT   1
#define SYSCALL_READ   3
#define SYSCALL_WRITE  4


int isnum(char ch) {
    return ch >= '0' && ch <= '9';
}

int isspc(char ch) {
    return ch == ' ' || ch == '\n';
}
// exit
static inline long __exit(int arg1) {
    int ret;
    asm volatile ("int $0x80" 
                 : "=a" (ret) 
                 : "a" (SYSCALL_EXIT), "b" (arg1):"memory");
    return ret;
}
// read
static inline int _read( int arg1, char *arg2, int arg3) {
    int ret;
    asm volatile ("int $0x80"
                 : "=a"(ret)
                 : "a"(SYSCALL_READ), "b"(arg1), "c"(arg2), "d"(arg3)
                 : "memory");
    return ret;
}
// write
static inline int _write(int arg1, char *arg2, int arg3) {
    int ret;
    asm volatile ("int $0x80" 
                    : "=a"(ret) 
                    : "a"(SYSCALL_WRITE), "b"(arg1), "c"(arg2), "d"(arg3) 
                    : "memory");
    return ret;
}


static void _print(unsigned num) {
    char buf[20];
    char hex_number[18];

    int i = 0;
    buf[i++] = '0';
    buf[i++] = 'x';
    
    int j = 0;
    if (num == 0) {
        buf[i] = '0';
        i++;
    }else {
        while( num != 0) {
            int temp = 0;
            temp = num % 16;
            if (temp < 10) {
                hex_number[j] = temp + 48;
                j++;
            }
            else {
                hex_number[j] = temp + 87;
                j++;
            }
            num = num / 16;
        }
        j --;
        while (j >= 0) {
            buf[i] = hex_number[j];
            j--;
            i++;
        }
    }

    buf[i++] = '\n';

    int ret = _write(STDOUT_FILENO, buf, i);
    if (ret == -1) {
        __exit(1);
    }
        
}

int _start() {
    char buf[20];
    unsigned num = 0;
    int i = 0;
    int num_digits = 0;
    unsigned chars_in_buffer = 0;

    for (; ; i++, chars_in_buffer--) {
        if (chars_in_buffer == 0) {
            int ret = _read(STDIN_FILENO, buf, sizeof(buf));
            if (ret < 0) {
               __exit(1); 
            }    
            i = 0;
            chars_in_buffer = ret;
        }
        if (num_digits > 0 && (chars_in_buffer == 0 || !isnum(buf[i]))) {
            _print(num);
            num_digits = 0;
            num = 0;
        }
        if (chars_in_buffer == 0 || (!isspc(buf[i]) && !isnum(buf[i]))) {
            __exit(0);
        }
        if (isnum(buf[i])) {
            num = num * 10 + buf[i] - '0';
            num_digits++;
        }
    }
}

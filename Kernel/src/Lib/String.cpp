#include <Lib/String.hpp>
#include <MM/kmalloc.hpp>

u32 strlen(const char* str) {
    u32 len = 0;

    while (str[len]) {
        len++;
    }

    return len;
}

void* memset(void* dest, char val, u32 count) {
    u8* temp = (u8*) dest;
    for (; count != 0; count--) {
        temp[count] = val;
    }

    return dest;
}

void memcpy(void* dest, void* src, u32 size) {
    char* csrc = (char*) src;
    char* cdest = (char*) dest;

    for (u32 i = 0; i < size; i++) {
        cdest[i] = csrc[i];
    }
}

int strcmp(const char* a, const char* b) {
    while (*a) {
        if (*a != *b) {
            break;
        }

        a++;
        b++;
    }

    return *(const unsigned char*) a - *(const unsigned char*) b;
}

char* strdup(const char* src) {
    int len = strlen(src) + 1;
    char* dst = (char*) kmalloc(len);
    memcpy(dst, (void*) src, len);
    return dst;
}

char* strcpy(char* strDest, const char* strSrc) {
    char* temp = strDest;
    while ((*strDest++ = *strSrc++) != '\0') {
    }
    return temp;
}

char* strcat(char* destination, const char* source) {
    strcpy(destination + strlen(destination), source);
    return destination;
}

char* strsep(char** stringp, const char* delim) {
    char* s;
    const char* spanp;
    int c;
    int sc;
    char* tok;

    if ((s = *stringp) == nullptr) {
        return nullptr;
    }

    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0) {
                    s = nullptr;
                } else {
                    s[-1] = 0;
                }
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
}

u32 countDigit(u32 n, u32 base) {
    if (n == 0) {
        return 1;
    }

    int count = 0;
    while (n != 0) {
        n = n / base;
        ++count;
    }
    return count;
}

int abs(u32 n) {
    return (n >= 0) ? n : -n;
}

void swap(char* x, char* y) {
    char t = *x;
    *x = *y;
    *y = t;
}

char* reverse(char* buffer, int i, int j) {
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }

    return buffer;
}

char* itos(u32 value, char* buffer, int base) {
    // Only when heap is uninitialized
    // TODO: write better way with heap initialized
    if (base < 2 || base > 32) {
        return buffer;
    }

    u32 n = abs(value);

    u32 i = 0;
    while (n) {
        u32 r = n % base;

        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        } else {
            buffer[i++] = 48 + r;
        }

        n = n / base;
    }

    if (i == 0) {
        buffer[i++] = '0';
    }

    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    return reverse(buffer, 0, i - 1);
}
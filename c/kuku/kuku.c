#include <stdio.h>

void show_kuku(int beg, int end) {
    for (int i = beg; i <= end; i++) {
        for (int j = beg; j <= end; j++) {
            printf("%c ", 65 + j);
        }
        puts("");
    }
}

int main(void) {
    show_kuku(0, 25);
    return 0;
}

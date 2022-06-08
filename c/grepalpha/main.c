#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

bool is_word(int c) {
    return isalpha(c) || isdigit(c);
}

void grep_words(FILE *fin, FILE *fout) {
    int m = 0;
    
    for (;;) {
        int c = fgetc(fin);
        if (c == EOF) {
            break;
        }

        switch (m) {
        case 0:  // first
            if (is_word(c)) {
                fputc(c, fout);
                m = 10;
            }
            break;
        case 10:  // found word
            if (is_word(c)) {
                fputc(c, fout);
            } else {
                fputc('\n', fout);
                m = 0;
            }
            break;
        }
    }
}

int main(void) {
    grep_words(stdin, stdout);
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

enum {
    DISP_HEIGHT = 30,
    DISP_WIDTH = 60,
    BUF_SIZE = DISP_HEIGHT * DISP_WIDTH,
    SOURCE_TEXT_SIZE = 5000,
    WORD_SIZE = 32,
    WORDS_SIZE = 1000,
};

typedef struct {
    double x, y;
} Vector2f;

typedef struct {
    Vector2f pos;
    char text[WORD_SIZE];
} Word;

int buffer[BUF_SIZE];
char source_text[SOURCE_TEXT_SIZE];
Word words[WORDS_SIZE];
int words_index;

void load(void) {
    FILE *fin = fopen("source.txt", "rb");
    if (!fin) {
        return;
    }

    fread(source_text, sizeof(source_text[0]), SOURCE_TEXT_SIZE, fin);

    fclose(fin);
}

void clear_buffer(void) {
    int size = sizeof(buffer[0]) * BUF_SIZE;
    memset(buffer, ' ', size);
}

void save_word(const char *buf, Vector2f pos) {
    Word *word = &words[words_index++];
    snprintf(word->text, sizeof word->text, "%s", buf);
    word->pos = pos;
}

void init_words(void) {
    int m = 0;
    char buf[WORD_SIZE] = {0};
    int buf_index = 0;
    Vector2f pos = {0};

    for (const char *p = source_text; *p; p += 1) {
        int c = *p;

        if (c == '\n') {
            pos.y += 1;
            pos.x = 0;
        }

        switch (m) {
        case 0:  // first
            if (isalpha(c)) {
                buf[buf_index++] = c;
                m = 10;
            }
            break;
        case 10:  // found alpha
            if (isalpha(c)) {
                buf[buf_index++] = c;
            } else if (c == ' ' ||
                       c == '.') {
                save_word(buf, pos);
                pos.x += strlen(buf);
                memset(buf, 0, sizeof buf);
                buf_index = 0;

                buf[0] = c;
                save_word(buf, pos);
                pos.x += 1;
                memset(buf, 0, sizeof buf);

                m = 0;
            }
            break;
        }
    }
}

void init(void) {
    srand(time(NULL));
    clear_buffer();
    init_words();
}

void copy_words(void) {
    for (int i = 0; i < words_index; i += 1) {
        Word *word = &words[i];
        int x = word->pos.x;
        int y = word->pos.y;
        int bi = y * DISP_WIDTH + x;        
        
        for (int j = 0; j < strlen(word->text); j += 1) {
            int index = bi + j;
            if (index < 0) {
                index = 0;
            } else if (index >= BUF_SIZE) {
                index = BUF_SIZE - 1;
            }
            buffer[index] = word->text[j];
        }
    }
}

void move_words(void) {
    for (int i = 0; i < words_index; i += 1) {
        Word *word = &words[i];
        double ax = -1 + rand() % 3;
        double ay = -1 + rand() % 3;
        ax /= 2;
        ay /= 2;
        word->pos.x += ax;
        word->pos.y += ay;
    }
}

void update(void) {
    usleep(1000 * 30);  // 30ms
    move_words();
}

void draw(void) {
    system("clear");  // cls

    clear_buffer();
    copy_words();

    for (int i = 0; i < BUF_SIZE; i += 1) {
        int c = buffer[i];
        putchar(c);
        if ((i + 1) % DISP_WIDTH == 0) {
            putchar('\n');
        }
    }
}

int main(void) {
    load();
    init();

    for (;;) {
        update();
        draw();
    }

    return 0;
}

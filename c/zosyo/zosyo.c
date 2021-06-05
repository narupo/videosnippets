#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum {
    MAX_NO_SIZE = 1024,
    BOOK_TITLE_SIZE = 100,
    BOOK_CONTENT_SIZE = 400,
};

typedef struct {
    bool is_deleted;
    char title[BOOK_TITLE_SIZE];
    char content[BOOK_CONTENT_SIZE];
} book_t;

static const char *DB_NAME = "zosyo.db";

/**
 * 蔵書の登録
 */
static bool add(void) {
    book_t book = {0};

    printf("Book title > ");
    if (fgets(book.title, sizeof book.title, stdin) == NULL) {
        return false;
    }
    book.title[strlen(book.title) - 1] = '\0';

    printf("Book content > ");
    if (fgets(book.content, sizeof book.content, stdin) == NULL) {
        return false;
    }
    book.content[strlen(book.content) - 1] = '\0';

    FILE *db = fopen(DB_NAME, "ab");
    if (db == NULL) {
        return false;
    }

    fwrite(&book, sizeof(book_t), 1, db);

    fclose(db);
    return true;
}

static void show_book(int32_t no, book_t *book) {
    if (book->is_deleted) {
        return;
    }

    printf("Book %d\n", no);
    printf("  Title: %s\n", book->title);
    printf("  Content: %s\n", book->content);    
}

/**
 * 蔵書の閲覧
 */
static bool view(void) {
    char no[MAX_NO_SIZE];

    printf("Book no > ");
    if (fgets(no, sizeof no, stdin) == NULL) {
        return false;
    }
    int ino = atoi(no);

    FILE *db = fopen(DB_NAME, "r+");
    if (db == NULL) {
        return false;
    }

    int32_t recordsize = sizeof(book_t);
    fseek(db, recordsize * ino, SEEK_SET);

    book_t book = {0};
    fread(&book, sizeof(book_t), 1, db);

    fclose(db);

    show_book(ino, &book);
    return true;
}

static bool list(void) {
    FILE *db = fopen(DB_NAME, "r");
    if (db == NULL) {
        return false;
    }

    for (int no = 0; ; no++) {
        book_t book = {0};
        if (fread(&book, sizeof(book_t), 1, db) == 0) {
            break;
        }

        show_book(no, &book);
    }

    fclose(db);
    return true;
}

static bool delete(void) {
    char no[MAX_NO_SIZE];
    printf("Delete book no > ");
    if (fgets(no, sizeof no, stdin) == NULL) {
        return false;
    }
    int32_t ino = atoi(no);

    FILE *db = fopen(DB_NAME, "r+");
    if (db == NULL) {
        return false;
    }

    int32_t recordsize = sizeof(book_t);
    book_t book = {0};
    int32_t curno = 0;

    for (; fread(&book, sizeof(book_t), 1, db) != 0; curno++) {
        if (curno == ino) {
            if (book.is_deleted) {
                printf("%d was deleted\n", ino);
                return true;
            }
            book.is_deleted = true;
            fseek(db, recordsize * ino, SEEK_SET);
            fwrite(&book, sizeof(book_t), 1, db);
            break;
        }
    }

    fclose(db);
    return true;
}

int main(void) {
    char cmd[100];

    for (;;) {
        printf("command > ");
        if (fgets(cmd, sizeof cmd, stdin) == NULL) {
            break;
        }

        cmd[strlen(cmd) - 1] = '\0';
        if (strcmp(cmd, "add") == 0) {
            if (!add()) {
                fprintf(stderr, "failed to add\n");
                continue;
            }
        } else if (strcmp(cmd, "view") == 0) {
            if (!view()) {
                fprintf(stderr, "failed to view\n");
                continue;
            }
        } else if (strcmp(cmd, "list") == 0) {
            if (!list()) {
                fprintf(stderr, "failed to list\n");
                continue;
            }
        } else if (strcmp(cmd, "delete") == 0) {
            if (!delete()) {
                fprintf(stderr, "failed to delete\n");
                continue;
            }
        } else {
            fprintf(stderr, "invalid command [%s]\n", cmd);
            continue;
        }
    }

    return 0;
}

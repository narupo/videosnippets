#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

void usage(void) {
    printf(
        "Generate random token.\n"
        "\n"
        "Usage:\n"
        "\n"
        "    gentoken [len]\n"
        "\n"
    );
}

char gen_random_char(void) {
    static const char *s = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t len = strlen(s);
    size_t i = rand() % len;
    return s[i];
}

char *gen_token(int32_t len) {
    int32_t byte = sizeof(char);
    int32_t size = byte * len + byte;
    char *token = malloc(size);
    if (!token) {
        return NULL;
    }

    for (int32_t i = 0; i < len; i +=1) {
        char c = gen_random_char();
        token[i] = c;
    }
    token[len] = '\0';

    return token;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        return 0;
    }

    srand(time(NULL));

    int32_t len = atoi(argv[1]);
    char *token = gen_token(len);
    if (!token) {
        perror("gen_token");
        return 1;
    }

    puts(token);
    fflush(stdout);

    free(token);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void usage(void) {
    printf(
        "Read file content\n"
        "\n"
        "Usage:\n"
        "\n"
        "    readfile [file-name]\n"
        "\n"
    );
}

char *read_file(const char *fname) {
    struct stat sbuf;

    if (stat(fname, &sbuf) == -1) {
        perror("stat");
        return NULL;
    }

    long size = sbuf.st_size + sizeof(char);
    char *content = malloc(size);
    if (!content) {
        perror("malloc");
        return NULL;
    }

    FILE *fin = fopen(fname, "rb");
    if (!fin) {
        perror("fopen");
        free(content);
        return NULL;
    }

    fread(content, sizeof(char), sbuf.st_size, fin);
    content[sbuf.st_size] = '\0';

    fclose(fin);
    return content;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        return 0;
    }

    const char *fname = argv[1];

    char *content = read_file(fname);
    if (!content) {
        return 1;
    }

    printf("%s", content);
    fflush(stdout);

    free(content);
    return 0;
}

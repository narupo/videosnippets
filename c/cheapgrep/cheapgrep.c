#include <stdio.h>
#include <string.h>

void usage(void) {
    printf("Usage: cheapgrep [file-name] [keyword]\n");
}

int cheap_grep(const char *fname, const char *keyword) {
    FILE *fin = fopen(fname, "rt");
    if (!fin) {
        perror("ファイルを開くのに失敗しました");
        return 1;
    }

    for (;;) {
        char line[1024];
        if (!fgets(line, sizeof line, fin)) {
            break;
        }

        if (strstr(line, keyword)) {
            printf("%s", line);
        }
    }

    fclose(fin);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        usage();
        return 0;
    }

    const char *fname = argv[1];
    const char *keyword = argv[2];

    return cheap_grep(fname, keyword);
}

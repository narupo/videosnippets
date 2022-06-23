#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>

static void
usage(void) {
    printf(
        "Create empty file.\n"
        "\n"
        "Usage:\n"
        "\n"
        "    trunc [file-name]\n"
        "\n"
    );
}

static bool
is_exists(const char *fname) {
    struct stat buf;
    return stat(fname, &buf) == 0;
}

static int
trunc_(const char *fname) {
    if (is_exists(fname)) {
        fprintf(stderr, "ファイルが存在します。\n");
        return 1;
    }

    FILE *fout = fopen(fname, "wb");
    if (!fout) {
        perror("ファイルを開くのに失敗しました");
        return 1;
    }

    fclose(fout);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        return 0;
    }

    const char *fname = argv[1];
    return trunc_(fname);
}

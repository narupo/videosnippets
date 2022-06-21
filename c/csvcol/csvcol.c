#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

void usage(void) {
    printf(
        "Get CSV column\n"
        "\n"
        "Usage:\n"
        "\n"
        "    csvcol [file-name] [row] [col]\n"
        "\n"
    );
}

int parse_stream(
    FILE *fin,
    int32_t row,
    int32_t col
) {
    int32_t currow = 1;
    int32_t curcol = 1;
    bool do_show = false;

    for (;;) {
        int32_t c = fgetc(fin);
        if (c == EOF) {
            break;
        }

        if (currow == row &&
            curcol == col) {
            do_show = true;
        }

        if (c == ',') {
            curcol += 1;
            if (do_show) {
                goto done;
            }
        } else if (c == '\n') {
            currow += 1;
            curcol = 1;
            if (do_show) {
                goto done;
            }
        } else if (do_show) {
            putchar(c);
        }
    }

done:
    putchar('\n');
    return 0;
}

int show_col(
    const char *fname,
    int32_t row,
    int32_t col
) {
    FILE *fin = fopen(fname, "rb");
    if (!fin) {
        perror("ファイルを開けませんでした。");
        return 1;
    }

    int result = parse_stream(fin, row, col);

    fclose(fin);
    return result;   
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        usage();
        return 0;
    }

    const char *fname = argv[1];
    int32_t row = atoi(argv[2]);
    int32_t col = atoi(argv[3]);

    return show_col(fname, row, col);
}

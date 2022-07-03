#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>

struct AppOption {
    bool is_help;
    bool is_fname;
    char fname[100];
};

bool parse_option(struct AppOption *opt, int argc, char *argv[]) {
    static const struct option longopts[] = {
        {"help", no_argument, 0, 'h'},
        {"fname", required_argument, 0, 'f'},
        {0, 0, 0, 0},
    };
    static const char *optstring = "hf:";
    int longindex = 0;

    for (; ; ) {
        int c = getopt_long(argc, argv, optstring, longopts, &longindex);
        if (c == -1) {
            break;
        }

        switch (c) {
        default: break;
        case 'h': opt->is_help = true; break;
        case 'f':
            opt->is_fname = true;
            snprintf(opt->fname, sizeof opt->fname, "%s", optarg);
            break;
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    struct AppOption opt = {0};

    if (!parse_option(&opt, argc, argv)) {
        fprintf(stderr, "failed to parse option\n");
        return 1;
    }

    if (opt.is_help) {
        printf("Help!\n");
    } else if (opt.is_fname) {
        printf("Fname %s\n", opt.fname);
    }

    for (int i = optind; i < argc; i += 1) {
        printf("%s\n", argv[i]);
    }

    return 0;
}

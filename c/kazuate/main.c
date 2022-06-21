#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

bool input(int *hope) {
    char input_buf[100];

    printf("数を入力してください（0〜10） > ");
    if (!fgets(input_buf, sizeof input_buf, stdin)) {
        return false;
    }

    *hope = atoi(input_buf);

    return true;
}

bool update(void) {
    int hope = 0;
    
    if (!input(&hope)) {
        return false;
    }

    int num = rand() % 11;

    printf("値は%dです。\n", num);
    printf("入力された値は%dです。\n", hope);

    if (num == hope) {
        printf("当たりです！\n");
    } else {
        printf("はずれです。\n");
    }

    return true;
}

int main(void) {
    srand(time(NULL));

    while (update()) {
    }

    return 0;
}

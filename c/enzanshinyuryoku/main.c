#include <stdio.h>
#include <stdlib.h>

static void
die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

static int
calc(int lhs, char op, int rhs) {
    switch (op) {
    default:
        die("invalid operator");
        break;
    case '+': return lhs + rhs; break;
    case '-': return lhs - rhs; break;
    case '*': return lhs * rhs; break;
    case '/':
        if (rhs == 0) {
            die("zero division error");
        }
        return lhs / rhs;
    case '%':
        if (rhs == 0) {
            die("zero division error");
        }
        return lhs % rhs;
    }
    return -1;
}

int main(void) {
    int ans, lhs, rhs;
    char op;

    // 入力
    scanf("%d %c %d", &lhs, &op, &rhs);

    // 計算処理
    ans = calc(lhs, op, rhs);

    // 出力処理
    printf("%d %c %d = %d\n", lhs, op, rhs, ans);
    fflush(stdout);

    return 0;
}

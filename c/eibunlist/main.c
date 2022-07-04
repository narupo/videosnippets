#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListItem {
    struct ListItem *next;
    char str[100];
} ListItem;

void list_item_del(ListItem *self) {
    free(self);
}

ListItem *list_item_new(
    ListItem *next,
    const char *str
) {
    ListItem *self = malloc(sizeof(*self));
    if (!self) {
        return NULL;        
    }

    self->next = next;
    snprintf(self->str, sizeof self->str, "%s", str);

    return self;
}

typedef struct {
    ListItem *root;
} List;

void list_destroy(List *self) {
    for (ListItem *cur = self->root; cur; ) {
        ListItem *del = cur;
        cur = cur->next;
        list_item_del(del);
    }
}

void list_init(List *self) {
    self->root = NULL;
}

void list_show(const List *self) {
    for (const ListItem *cur = self->root;
         cur;
         cur = cur->next) {
        printf("[%s]\n", cur->str);
    }
}

void list_push_back(List *self, ListItem *item) {
    if (!self->root) {
        self->root = item;
        return;
    }

    ListItem *tail = NULL;
    for (ListItem *cur = self->root;
         cur;
         cur = cur->next) {
        if (!cur->next) {
            tail = cur;
            break;
        }
    } 

    if (tail) {
        tail->next = item;
    }
}

void die(const char *s) {
    perror(s);
    exit(1);
}

#define store() { \
        ListItem *item = list_item_new(NULL, buf); \
        if (!item) { \
            die("item is null"); \
        } \
        list_push_back(list, item); \
        buf[0] = '\0'; \
        buf_i = 0; \
    } \

void parse(List *list, const char *src) {
    char buf[100];
    int buf_i = 0;

    for (const char *p = src; *p; p += 1) {
        if (*p == ' ') {
            store();
        } else {
            if (buf_i >= sizeof buf) {
                die("buffer overflow");
            }
            buf[buf_i++] = *p;
            buf[buf_i] = '\0';
        }
    }

    if (buf_i) {
        store();
    }
}

int main(void) {
    const char *src = "abc123 456def ghi";
    List list;    
    list_init(&list);

    parse(&list, src);

    list_show(&list);
    list_destroy(&list);
    return 0;
}

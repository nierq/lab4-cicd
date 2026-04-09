#include <stdio.h>
#include <ctype.h>

void process_string(char *str) {
    int i = 0;
    while (str[i] != '\0' && str[i] != '\n') {
        if (isalpha((unsigned char)str[i])) {
            if (i % 2 == 0) {
                str[i] = toupper((unsigned char)str[i]);
            } else {
                str[i] = tolower((unsigned char)str[i]);
            }
        }
        i++;
    }
    if (str[i] == '\n') str[i] = '\0';
}

int main() {
    int variant = 1;
    printf("%d\n", variant);
    char input[100];
    fgets(input, sizeof(input), stdin);
    process_string(input);
    printf("%s\n", input);
    return 0;
}
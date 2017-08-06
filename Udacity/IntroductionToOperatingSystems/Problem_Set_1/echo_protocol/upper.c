#include <stdio.h>

void upper(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if ((int)str[i] >= (int)'a' && (int)str[i] <= (int)'z') {
            str[i] = (int)str[i] + ((int)'A' - (int)'a');
        }
        ++i;
    }
}

// int main() {
//     char a[] = {"abc ERTd"};
//     printf("%s\n", a);
//     upper(a);
//     printf("%s\n", a);
// }
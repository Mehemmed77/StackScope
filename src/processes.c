#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** parse_fields(char *buffer, int max_tokens) {
    char **arr = malloc((max_tokens + 1) * sizeof(char*));
    int i = 0, j = 0, count = 0;

    for (int k = 0; k < max_tokens; k++)
        arr[k] = malloc(256);

    while (count < max_tokens - 1) {
        while (buffer[i] == ' ') i++;

        j = 0;
        while (buffer[i] != ' ' && buffer[i] != '\0')
            arr[count][j++] = buffer[i++];

        arr[count][j] = '\0';

        while (buffer[i] == ' ') i++;

        count++;
    }

    // last field: copy the rest of the line
    j = 0;
    while (buffer[i] != '\0')
        arr[count][j++] = buffer[i++];

    arr[count][j] = '\0';

    arr[max_tokens] = NULL;
    return arr;
}

int print_result(char pid[]) {
    FILE *fptr;

    char destination[64], buffer[64];
    strcpy(destination, "/proc/");
    strcat(destination, pid);
    strcat(destination, "/comm");

    fptr = fopen(destination, "r");

    if (!fptr) {
        return 0;
    }

    while(fgets(buffer, sizeof(buffer), fptr) != NULL) {
        printf("PID: %s     NAME : %s", pid, buffer);
    }

    fclose(fptr);

    return 0;
}

int main() {
    FILE *fptr,  *ps_aux;
    char buffer[256];
    int count = 0;

    ps_aux = popen("ps -eo user,pid,pcpu,pmem,vsz,rss", "r");

    if (ps_aux == NULL) {
        printf("popen failed");
        return 0;
    }

    while(fgets(buffer, sizeof(buffer), ps_aux) != NULL) {
        if (count == 0) {// to ignore the line of fields
            count++;
            continue;
        } 

        char **arr = parse_fields(buffer, 6);

        print_result(arr[1]);

        for (int i = 0; i < 5; i++) free(arr[i]);
        free(arr);
    }

    pclose(ps_aux);

    return 0;
}
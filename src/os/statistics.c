#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** parse_statistic_fields(char *pid) {
    // extracts process name, state, parent PID, num of threads, virtual memory size
    // 3, 4, 20, 23.

    FILE *f;
    int count = 1, index = 0, i = 0, j = 0;
    char **res = malloc(6 * sizeof(char*));

    for(int i = 0; i < 6; i++) res[i] = malloc(64);

    char buffer[256], destination[64];
    strcpy(destination, "/proc/");
    strcat(destination, pid);
    strcat(destination, "/stat");

    strncpy(res[index], pid, 63);
    res[index][63] = '\0';
    index++;
    f = fopen(destination, "r");

    if(!f) return NULL;

    while(fgets(buffer, sizeof(buffer), f) != NULL) {
        printf("%s", buffer);

        i = 0;
        while(buffer[i] != '\0') {
            j = 0;
            while(buffer[i] != ' ') {
                if(count == 3 || count == 4 || count == 20 || count == 23) {
                    res[index][j++] = buffer[i];
                }

                i++;
            }

            if(j > 0){
                res[index][j] = '\0';
                index++;
            };

            count++;

            while(buffer[i] == ' ') i++;
        }
    }

    fclose(f);

    res[5] = NULL;

    return res;
}

int main() {
    FILE *ps_aux;
    char buffer[16];
    int n = 0;
    ps_aux = popen("ps -eo pid", "r");

    while(fgets(buffer, sizeof(buffer), ps_aux) != NULL) {
        if (n == 0) {
            n++;
            continue;
        }

        char **arr = parse_statistic_fields(buffer);
        for(int i = 0; i < 5; i++) {
            printf("%s", arr[i]);
        }
    }

    pclose(ps_aux);

    return 0;
}

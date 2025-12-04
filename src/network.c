#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

const char *tcp_state_table[256] = {
    
    [0x01] = "ESTABLISHED",
    [0x02] = "SYN_SENT",
    [0x03] = "SYN_RECV",
    [0x04] = "FIN_WAIT1",
    [0x05] = "FIN_WAIT2",
    [0x06] = "TIME_WAIT",
    [0x07] = "CLOSE",
    [0x08] = "CLOSE_WAIT",
    [0x09] = "LAST_ACK",
    [0x0A] = "LISTEN",
    [0x0B] = "CLOSING",
    [0x0C] = "NEW_SYN_RECV"
};

char* convert_to_decimal_ip4(char hex[]) {
    char* out = malloc(16);
    unsigned int bytes[4];

    for (int i = 0; i < 4; i++) {
        sscanf(hex + i*2, "%2x", &bytes[i]);
    }

    snprintf(out, 16, "%u.%u.%u.%u",
             bytes[0], bytes[1], bytes[2], bytes[3]);

    return out;
}

char** parse_fields(char* buffer, int max_tokens) {
    char** arr = malloc((max_tokens + 1) * sizeof(char*));
    int i = 0, j = 0, c = 0;

    for(int k = 0; k < max_tokens; k++) arr[k] = malloc(256);

    while (c + 1 < max_tokens) {
        while (buffer[i] == ' ') i++;

        j = 0;
        while (buffer[i] != ' ' && buffer[i] != '\0') {
            arr[c][j++] = buffer[i++]; 
        }

        arr[c][j] = '\0';
        c++;
    }

    j = 0;
    while(buffer[i] != '\0') {
        arr[c][j++] = buffer[i++];
    }

    arr[c][j] = '\0';
    arr[max_tokens] = '\0';

    return arr;
}

void print_result(char** record) {
    int index = atoi(record[0]);
    char* local_address = convert_to_decimal_ip4(*(record + 1));  
    char* remote_address = convert_to_decimal_ip4(*(record + 2));
    const char* state = tcp_state_table[(int) strtol(*(record + 3), NULL, 16)];

    printf("Index: %d, Local Address: %s, Remote Address: %s, State: %s \n", index, local_address, remote_address, state);
}

int main() {
    FILE *f;
    char buffer[1024];
    int c = 0, max_tokens = 8;

    f = fopen("/proc/net/tcp", "r");
    

    if (f == NULL) {
        printf("Couldn't open the file");
    }

    while(fgets(buffer, sizeof(buffer), f) != NULL) {
        if (c == 0) {
            c++;
            continue;
        }

        char** record = parse_fields(buffer, max_tokens);

        print_result(record);

        for(int i = 0; i < max_tokens - 1; i++) {
            free(record[i]);
        }

        free(record);
    }

    fclose(f);

    return 0;
}

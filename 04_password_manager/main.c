#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 256
#define UPPER_RATE 3
#define SPECIAL_RATE 5

const char* const lengthArg = "--length";
const char* const upperArg  = "--upper";
const char* const digitsArg  = "--digits";
const char* const specialArg = "--special";
const char* const imageArg = "--image";
const char* const nameArg = "--name";

const char* const passwordFile = "password.txt";

const char* const lower = "abcdefghijklmnopqrstuvwxyz";
const char* const upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* const special = ".,!@#$%^&*()<>{}[]/|\?;:'-_";

const int lowerSize = 26;
const int upperSize = 26;
const int specialSize = 27;

const int defaultLength = 10;

typedef enum {
    ARGUMENT_ERROR = 1,
    FILE_ERROR = 2,
} ExitStatus;

// Given some URL and the name of the website store the password
typedef struct { char* URL;
    char* name;
    int length;
} PasswordItem;

typedef struct {
    int length;
    bool upper;
    bool digits;
    bool special;
    bool image;
    char* name;
    char* imageName;
    char* URL;
} Parameters;

Parameters parse_args(int argc, char* argv[]);
bool search_existing_passwords(FILE* file, char* name); 
void to_lower(char* str);
void print_help(void);

/* 
 * should be able to pass the length of the password
 * Whether or not special characters are included
 * Whether it includes digtits.
 * ./passwordmanger [--length <int>] [--image <image>] [--upper] [--digits] [--name <string>] URL...
*/
Parameters parse_args(int argc, char* argv[]) {
    Parameters params = {.length = 0,
        .upper = false,
        .digits = false,
        .image = false,
        .name = NULL,
        .imageName = NULL,
        .URL = NULL
    };
    argc--;
    argv++;
    while (argc > 0 && strncmp(argv[0], "--", 2) == 0) {
        if (!strcmp(argv[0], lengthArg) && argv[1] && !params.length) {
            params.length = atoi(argv[1]);
            argc--;
            argv++;
        } else if (!strcmp(argv[0], imageArg) && argv[1] && !params.image) {
            params.image = true;
            params.imageName = argv[1];
            argc--;
            argv++;
        } else if (!strcmp(argv[0], upperArg) && !params.upper) {
            params.upper = true;
        } else if (!strcmp(argv[0], digitsArg) && !params.digits) {
            params.digits = true;
        } else if (!strcmp(argv[0], specialArg) && !params.special) {
            params.special = true;
        } else if (!strcmp(argv[0], nameArg) && argv[1] && !params.name) {
            params.name = argv[1];
            to_lower(params.name);
            argc--;
            argv++;
        } else {
            fprintf(stderr, "Invalid argument Error!\n");
            exit(ARGUMENT_ERROR);
        }
        argc--;
        argv++;
    }
    if (!argv[0][0]) {
        fprintf(stderr, "Invalid argument Error!");
        exit(ARGUMENT_ERROR);
    }
    params.URL = argv[0];
    return params;
}

/*
 * name: [name]
 * URL: [URL]
 * password: [password]
*/
bool search_existing_passwords(FILE* file, char* name) {
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, name) != NULL) {
            return true;
        };
    }
    return false;
}

void to_lower(char* str) {
    while (*str) {
        *str = tolower((unsigned char) *str);
        str++;
    }
}

void write_password(FILE* file, Parameters* params, char* password) {
    fprintf(file, "name: %s\nURL: %s\npassword: %s\n\n", params->name, params->URL, password);
}

uint32_t generate_hash(Parameters* params) {
    uint32_t hash = 0;
    int urlLength = strlen(params->URL);
    int nameLength = strlen(params->name);
    int length = urlLength + nameLength;

    const char* url = params->URL;
    const char* name = params->name;
    const uint32_t mask = 0xFFFFFFFF;
    const int primeMultiplier = 5;
    for (int i = 0; i < length; i++) {
        hash = (hash * primeMultiplier + url[i % urlLength] + name[i % nameLength]) & mask;
        hash ^= (hash << 5) | (hash >> 27);
    }
    return hash;
}

char* generate_password(Parameters* params, uint32_t hash) {
    int index = 0;
    int length = (params->length != 0) ? params->length : defaultLength;
    char* password = malloc(sizeof(char) * length);

    const uint32_t mask = 0xFFFFFFFF;
    const int primeMultiplier = 7;
    for (int i = 0; i < length; i++) {
        hash = (hash * primeMultiplier + lower[i % lowerSize] + upper[i % upperSize] + special[i % specialSize]) & mask;
        hash ^= (hash << 5) | (hash >> 27);

        if (params->upper && hash % UPPER_RATE == 0) {
            index = hash % upperSize;
            password[i] = upper[index];
            continue;
        } else if (params->special && hash % SPECIAL_RATE == 1) {
            index = hash % specialSize;
            password[i] =special[index];
            continue;
        }

        index = hash % lowerSize;
        password[i] = lower[index];
    }

    password[length] = '\0';
    return password;
}

void debug_parameters(Parameters* params) {
    printf("Website Name: %s\nURL: %s\nImage Name: %s\n"
           "Length: %d\nUpper: %d\nDigits: %d\nSpecial: %d\nImage: %d\n\n"
           , params->name, params->URL, params->imageName, params->length,
           params->upper, params->digits, params->special, params->image);
}


void print_help(void) {
    printf("%s",
           "Usage: CryptyKey [OPTION]...\n"
           "\n"
           "Options:\n"
           "  -l, --length"
           "  -u, --upper"
           "  -d, --digits"
           "  -s, --special"
           "  -i, --image"
           "  -n, --name"
           "  -h, --help"
           );
}

int main(int argc, char* argv[]) {
    Parameters params = parse_args(argc, argv);
    // debug_parameters(&params);

    FILE* file = fopen(passwordFile, "a+");

    if (search_existing_passwords(file, params.name)) {
        char line[BUFFER_SIZE];
        for (int i = 0; i < 2; i++) {

            if (fgets(line, sizeof(line), file) == NULL) {
                perror("EOF reached\n");
                fclose(file);
                exit(FILE_ERROR);
            }
        }
        printf("%s", line);
    } else {
        if (fseek(file, 0, SEEK_END) != 0) {
            perror("Error seeking to end of file");
            fclose(file);
            exit(FILE_ERROR);
        }
        int hash = generate_hash(&params);
        char* password = generate_password(&params, hash);
        write_password(file, &params, password);
        printf("password: %s\n", password);

        // copy password to clipboard assuming using xclip
        char command[100]; // TODO: need to change to dynamically allocate given varying size of password
        sprintf(command, "echo '%s' | xclip -selection clipboard", password);
        system(command);
        printf("Coppied to Clipboard!\n");

        free(password);
    }

    fclose(file);
    return 0;
}





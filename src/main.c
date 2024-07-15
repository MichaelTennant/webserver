#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "project_metadata.h"

// string to unsigned 32 bit integer
// TODO: Move it into it's own file
// NOTE: Function not fully optimized
uint32_t stu32(const char* str, unsigned char* error) {
    uint32_t out = 0;
    int digits = strlen(str);
    int value = 1;

    // Check for leading zeros
    if (str[0] == '0') {
        *error = EDOM;
        return 0;
    }

    // If input string longer then 9 digits long (and no leading zeros), 
    // output will always be out of range.
    if (digits > 9) {
        *error = ERANGE;
        return 0;
    }

    // NOTE: UNDEFINED BEHAVIOUR ON 32 BIT MACHINES FOR INPUTS ["5000000000"-"999999999"]
    for (int i = digits - 1; i >= 0; i--) {
        short digit = str[i] - 48;   // ['0'-'9'] at ascii indices [48-57]
        
        if (digit < 0 || 10 <= digit) {
            *error = EDOM;
            return 0;
        }

        if (UINT32_MAX - value * digit < out) {
            *error = ERANGE;
            return 0;
        }

        out += value * digit;
        value *= 10;
    }

    *error = 0;
    return out;
}


int main(int argc, char* argv[]) {
    bool serve_http = false;
    bool serve_https = false;

    uint32_t http_port = 80;
    uint32_t https_port = 443;

    char* root_path = malloc(2);
    strcpy(root_path, ".");

    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-V") && strcmp(argv[i], "--version")) == 0) {
            printf("%s %s\n", PROJECT_NAME, PROJECT_VERSION);
            return 0;
        }

        if ((strcmp(argv[i], "-h") && strcmp(argv[i], "--help")) == 0) {
            // TODO: Make help look a little prettier. Maybe external file? 
            // TODO: Create man entry for project.
            printf("usage: %s [-V | --version] [-h | --help] [-s | --serve <http | https | both>] "
                    "[-d | --root-path <path>] [--http_port <port_no.>] [--https_port <port_no.>] "
                    "[-v | --verbose]\n", argv[0]);
            return 0;
        }

        if ((strcmp(argv[i], "-s") && strcmp(argv[i], "--serve")) == 0) {
            i++;
            if (argc <= i) {
                // TODO: EXPECTED MORE ARGUMENTS ERROR MESSAGE
                return EINVAL;
            }

            if (strcmp(argv[i], "http") == 0) {
                serve_http = true;
                continue;
            }

            if (strcmp(argv[i], "https") == 0) {
                serve_https = true;
                continue;
            }

            if (strcmp(argv[i], "both") == 0) {
                serve_http = true;
                serve_https = true;
                continue;
            }

            // Raise invalid argument error (err code 22)
            fprintf(stderr, "%s: '%s' expected argument 'http', 'https', or 'both' but got '%s'. "
                   "See '%s --help'.\n", argv[0], argv[i-1], argv[i], argv[0]);
            return EINVAL;
        }

        if ((strcmp(argv[i], "-d") && strcmp(argv[i], "--root-path")) == 0) {
            i++;
            if (argc <= i) {
                // TODO: EXPECTED MORE ARGUMENTS ERROR MESSAGE
                return EINVAL;
            }

            struct stat path_stat;
            if (stat(argv[i], &path_stat)) {
                // TODO: BETTER ERR MESSAGE
                perror(argv[0]);
                return errno;
            }

            if (!S_ISDIR(path_stat.st_mode)) {
                fprintf(stderr, "%s: '%s' expected valid directory but got '%s'." 
                        "See '%s --help.'\n", argv[0], argv[i-1], argv[i]);
            }

            root_path = realloc(root_path, strlen(argv[i])+1);
            strcpy(root_path, argv[i]);

            continue;
        }

        if (strcmp(argv[i], "--http_port") == 0) {
            i++;
            if (argc <= i) {
                // TODO: EXPECTED MORE ARGUMENTS ERROR MESSAGE
                return EINVAL;
            }

            unsigned char error = 0;
            http_port = stu32(argv[i], &error);

            if (error) {
                fprintf(stderr, "%s: '%s' expected positive integer value in range [0,%u] but got "
                        "'%s'. See '%s --help'.\n", 
                        argv[0], argv[i-1], UINT32_MAX, argv[i], argv[0]);
                return EINVAL;
            }

            continue;
        }

        if (strcmp(argv[i], "--https_port") == 0) {
            i++;
            if (argc <= i) {
                // TODO: EXPECTED MORE ARGUMENTS ERROR MESSAGE
                return EINVAL;
            }

            unsigned char error = 0;
            https_port = stu32(argv[i], &error);

            if (error) {
                fprintf(stderr, "%s: '%s' expected positive integer value in range [0,%u] but got "
                        "'%s'. See '%s --help'.\n", 
                        argv[0], argv[i-1], UINT32_MAX, argv[i], argv[0]);
                return EINVAL;
            }

            continue;
        }

        else if ((strcmp(argv[i], "-v") && strcmp(argv[i], "--verbose")) == 0) {
            verbose = true;
            continue;
        }

        // Raise invalid argument error (err code 22)
        fprintf(stderr, "%s: '%s' is not a valid %s argument. See '%s --help'.\n", 
                argv[0], argv[i], argv[0], argv[0]);
        return EINVAL;
    }

    if (!(serve_http || serve_https)) {
        serve_http = true;
        serve_https = true;
    }

    // TEMP CODE BLOCK FOR DEBUGGING START
    printf("serve_http: %s\n", serve_http ? "true" : "false");
    printf("serve_https: %s\n", serve_https ? "true" : "false");

    printf("http_port: %u\n", http_port);
    printf("https_port: %u\n", https_port);
    
    printf("root_path: %s\n", root_path);

    printf("verbose: %s\n", verbose ? "true" : "false");
    // TEMP CODE BLOCK FOR DEBUGGING END

    // TODO: The kinda important part

    free(root_path);
    return 0;
}

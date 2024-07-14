#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "project_metadata.h"


int main(int argc, char* argv[]) {
    bool serve_http = true;
    bool serve_https = true;

    uint32_t http_port = 80;
    uint32_t https_port = 443;

    char* root_path;

    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-V") && strcmp(argv[i], "--version")) == 0) {
            printf("%s %s\n", PROJECT_NAME, PROJECT_VERSION);
            return 0;
        }

        if ((strcmp(argv[i], "-h") && strcmp(argv[i], "--help")) == 0) {
            // TODO: Make help look a little prettier, maybe external file? 
            // TODO: Create man entry for project
            printf("usage: %s [-V | --version] [-h | --help] [-s | --serve <http | https | both>] "
                    "[-d | --root-path <path>] [--http_port <port_no.>] [--https_port <port_no.>] "
                    "[-v | --verbose]\n", argv[0]);
            return 0;
        }

        if ((strcmp(argv[i], "-s") && strcmp(argv[i], "--serve")) == 0) {
            i++;
            if (strcmp(argv[i], "http") == 0) {
                serve_http = true;
                serve_https = false;
                continue;
            }

            if (strcmp(argv[i], "https") == 0) {
                serve_http = false;
                serve_https = true;
                continue;
            }

            if (strcmp(argv[i], "both") == 0) {
                serve_http = true;
                serve_https = true;
                continue;
            }

           fprintf(stderr, "%s: expected argument 'http', 'https', or 'both' but got '%s'. "
                   "See '%s --help'.\n", argv[0], argv[i], argv[0]);
           return 22;
        }

        if ((strcmp(argv[i], "-d") && strcmp(argv[i], "--root-path")) == 0) {
            i++;
            // TODO: Get path at argv[i]
            // TODO: Verify path exists and program has read access
            continue;
        }

        if (strcmp(argv[i], "http_port") == 0) {
            i++;
            // TODO: Verify port number at argv[i] is a non-zero unsigned 32-bit integer
            continue;
        }

        if (strcmp(argv[i], "https_port") == 0) {
            i++;
            // TODO: Verify port number at argv[i] is a non-zero unsigned 32-bit integer
            continue;
        }

        else if ((strcmp(argv[i], "-v") && strcmp(argv[i], "--verbose")) == 0) {
            verbose = true;
            continue;
        }

        fprintf(stderr, "%s: '%s' is not a valid %s argument. See '%s --help'.\n", 
                argv[0], argv[i], argv[0], argv[0]);
        return 22;
    }

    // TODO: The kinda important part
}

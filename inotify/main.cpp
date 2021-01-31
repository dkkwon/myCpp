#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sigInotifier.hpp"


// CONFIG_FILE_PATH is fixed in helm chart
#define CONFIG_FILE_PATH  "../watchedfolder"


int main(int argc, char *argv[])
{
    // if (argc < 2 || strcmp(argv[1], "--help") == 0)
    //     printf("%s pathname...\n", argv[0]);

    int iFD = create_inotifier(CONFIG_FILE_PATH);
    register_siginotifier(iFD);

    while (1) {
      usleep(1000);
    }

    return 0;
}

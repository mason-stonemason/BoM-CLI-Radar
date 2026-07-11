#include <stdio.h>
#include <stdbool.h>
#include "../include/parameters.h"

#define PIXEL '▀'
#define RESOLUTION 64

int ftp(void);
void test_render(void);

int main(int argc, char* argv[])
{
	UserValues cli = set_user_values(argc, argv);
        if (cli.output) {
                file_path(&cli);
        }
	ftp();
}

void test_render(void)
{
	//TODO: testing the unicode in a 64x64 format with ANSI colour encodings changing top and bottom half (via foreground and background).
	printf("TIME: ################\n");
	printf("LOCATION: ############\n");
	printf("RESOLUTION: %dx%d\n", RESOLUTION, RESOLUTION);
	for (int i = 0; i < RESOLUTION / 2; i++) {
		printf("\n");
		for (int a = 0; a < RESOLUTION; a++) {
			printf("▀");
		}
	}
}

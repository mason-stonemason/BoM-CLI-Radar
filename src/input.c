#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/parameters.h"

#define PROGRAM_NAME "bomcli"

//flags
#define OUTPUT_FLAG "--output"
#define OUTPUT_FLAG_SHORT "-o"
#define NO_COLOUR_FLAG "--no-colour"
#define NO_COLOUR_FLAG_SHORT "-nc"
#define HELP_FLAG "--help"
#define HELP_FLAG_SHORT "-h"

//messages
static const char* const usageMessage = "Usage: bomcli ["
				 OUTPUT_FLAG_SHORT " | "
				 OUTPUT_FLAG "[=filePath]] ["
				 NO_COLOUR_FLAG_SHORT " | "
				 NO_COLOUR_FLAG "] location\n";
static const char* const briefMessage = "bomcli - an experimental and ridiculous weather radar viewer through the CLI via unicode and ANSI colour codes.\n"
					"Radar imagery data is public data sourced from the Bureau of Meterology's anonymous "
				        "and public FTP server (see https://www.bom.gov.au/catalogue/anon-ftp.shtml)\n";
static const char* const optionsMessage = "Options:\n" OUTPUT_FLAG_SHORT ", " OUTPUT_FLAG "[=filePath]   the file path to "
					  "send ascii radar history to, otherwise data is appended to \"bomcli.txt\" ("
					  "log file is created in current directory if it does not exist)"
					  "\n" NO_COLOUR_FLAG_SHORT ", " NO_COLOUR_FLAG "          removes ANSI colour encoding\n"
					  "location                  the radar site\n";
//error
const char* const invalidPath = "Invalid filepath.\n";
const char* const invalidLocation = "Invalid location.\n";
const char* const invalidFile = "Couldn't make file (\"bomcli.txt\"), try checking permissions or space.\n";

typedef enum {
	INVALID_USAGE_EXIT = 1,
	INVALID_FILEPATH_EXIT = 2,
	INVALID_LOCATION_EXIT = 3,
	INVALID_FILE_EXIT = 4
} ErrorCodes;

UserValues set_user_values(int argc, char* argv[]);
void file_path(UserValues* cli);

/**
 * @brief exiting function for usage errors
 */
void exit_usage(void)
{
	fprintf(stderr, "%s", usageMessage);
	exit(INVALID_USAGE_EXIT);
}

/**
 * @brief outputs the help msg to stdout, and exits with 0
 */
void help_output(void)
{
	printf("%s\n", briefMessage);	
	printf("%s\n", usageMessage);
	printf("%s", optionsMessage);
	exit(0);
}

/**
 * @brief this function parses the CLI into a UserValues struct
 * @param argc - the argument count
 * @param argv - the argument value
 * @returns the parsed CLI in the form of UserValues
 */
UserValues set_user_values(int argc, char* argv[])
{
	UserValues user = {
		.output = false,
		.filePath = NULL,
		.noColour = false,
		.location = NULL};

	if (argc < 2) {
		exit_usage();
	}
      
        //until i can find a better way for --help, an initial for loop will do	
	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], HELP_FLAG) || !strcmp(argv[i], HELP_FLAG_SHORT)) {
			help_output();
		}
	}

	argv++;
	while (argv[0]) {
	    	if (!strcmp(argv[0], OUTPUT_FLAG_SHORT) || !strcmp(argv[0], OUTPUT_FLAG)) {
			if (user.output) {
				exit_usage(); //for duplicates
			}
			user.output = true;
			argv++;
			if (argv[0] && strncmp(argv[0], "-", 1) != 0) {
				user.filePath = argv[0];
				argv++;
			}
		} else if (!strcmp(argv[0], NO_COLOUR_FLAG_SHORT) || !strcmp(argv[0], NO_COLOUR_FLAG)) {
			if (user.noColour) {
				exit_usage(); //for duplicates
			}
			user.noColour = true;
			argv++;
		} else if (strncmp(argv[0], "-", 1) == 0) {
			fprintf(stderr, "%s: %s invalid option\n", PROGRAM_NAME, argv[0]);
			exit_usage();
		} else {
			if (user.location) {
				exit_usage();
			}
			user.location = argv[0];
			argv++;
		}
	}

	if (!user.location) {
		exit_usage();
	}

	return user;
}

/**
 * @brief exiting function for file errors
 */
void exit_file(void)
{
	fprintf(stderr, "%s", invalidPath);
	exit(INVALID_FILEPATH_EXIT);
}

/**
 * @brief exiting function for file log creation errors.
 */
void exit_file_create(void)
{
	fprintf(stderr, "%s", invalidFile);
	exit(INVALID_FILE_EXIT);
}

/**
 * @brief determines whether file path is valid, and creates one if not specified
 * @param cli - to get the filepath from.
 */
void file_path(UserValues* cli)
{
	FILE* logs;
	if (cli->filePath) {
		logs = fopen(cli->filePath, "r");
		if (logs == NULL) {
			exit_file();
		}
	} else {
		logs = fopen("bomcli.txt", "w");
		if (logs == NULL) {
			exit_file_create();
		}
	}
	fclose(logs);
}

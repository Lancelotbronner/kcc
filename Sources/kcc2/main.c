#include <kcc/args.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Compiler setup and top-level execution
// Copyright (c) 2019 Warren Toomey, GPL3

// Initialise global variables
static void init() {

}

// Print out a usage if started incorrectly
static void usage(char *prog) {
	fprintf(stderr, "Usage: %s infile\n", prog);
	exit(1);
}

// Main program: check arguments and print a usage
// if we don't have an argument. Open up the input
// file and call scanfile() to scan the tokens in it.
int main(int argc, char *argv[]) {
	if (argc != 2)
		usage(argv[0]);

	init();

	// Open up the input file
	FILE *infile = fopen(argv[1], "r");
	if (!infile) {
		fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	kcc_infile(infile);

	// Create the output file
	FILE *outfile = fopen("out.s", "w");
	if (!outfile) {
		fprintf(stderr, "Unable to create out.s: %s\n", strerror(errno));
		exit(1);
	}
	kcc_outfile(outfile);

//	scan(&Token);			// Get the first token from the input
//	genpreamble();		// Output the preamble
//	statements();			// Parse the statements in the input
//	genpostamble();		// Output the postamble
//	fclose(Outfile);		// Close the output file and exit
	
	return 0;
}

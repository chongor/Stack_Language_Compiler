/*
 *  j.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "parser.h"

int main(int argc, char *argv[])
{

	FILE *fin;
	FILE *fout;
	char outname[121];
	int len;

	if (argc < 2)
	{
		printf("Error: Not enough arguments.  Missing input file_name.j\n");
		return 1;
	}

	fin = fopen(argv[1], "rb");

	if (fin == NULL)
	{
		printf("Error:  Couldn't open input file \"%s\"\n", argv[1]);
		return 1;
	}

	fout = fopen(outname, "w");

	if (fout == NULL)
	{
		printf("Error:  Failed to create .asm file.  Execution aborted.");
		return 1;
	}

	strcpy(outname, argv[1]);
	len = strlen(argv[1]);
	initWrite(outname);
	outname[len - 1] = 0;
	strcat(outname, "asm");

	parser(fin, fout);

	fclose(fin);
	fclose(fout);

	return 0;
}
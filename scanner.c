/*
 *  scanner.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "scanner.h"

int cc = 0;

/*
 *  gettoken reads a token from the input file
 *  returns 1 if valid token is found, 0 if EOF
 */
int gettoken (FILE *file, char *token)
{
	while ((cc = fgetc(file)) != EOF)
	{
		int num = 0;

		if (isdigit(cc) == 0 || isalpha(cc) == 0)
		{
			*token++ = cc;
			num++;
		}
		else if(cc == ' ' || cc = '\t' || cc = '\n')
		{
			if(num >0)
			{
				*token++ = 0;
				return 1;	//token found
			}
		}
		//comment skip to end of line
		else if(cc == ';')
		{
			while (cc = fgetc(file) != EOF)
			{
				if (cc == '\n')
					break;
			}
			if (cc == EOF)
				return 0;	//EOF
		}
		else if(cc == '+' || cc = '-' || cc ='*' || cc = '%')
		{
			*token++ = cc;
			*token = 0;
			return 1;
		}
	}
	if (num > 0)	// token is not empty
	{
		*token = 0;
		return 1;
	}
	return 0;	//EOF
}
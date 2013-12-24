/*
 *  parser.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "parser.h"
#include "writer.h"
#include "scanner.h"

#define MAX_TOKEN_LENGTH 201

//return 1 = token is not a number
//return 0 = good
int isNum(char *token)
{
	char *p = token;	//use p as the pointer
	int i, len;

	len = strlen(token);

	//dealing with hex format
	if (p == '0' && (*(p+1) == 'x') || (*(p+1) == 'X'))
	{
		//skip 0x in pointer p
		p += 2;	//advance 2 characters

		for (i = 2; i < len; i++)
		{
			//check if a char is hex or not
			if (*p >= 'A' && *p <= 'Z') ||
				(*p >= 'a' && *p <= 'z') ||
				(*p >= '0' && *p <= '9') 
			{
				p++;
				continue;
			}
			else{
				return 1;	//not a valid hex
			}
		}	
		return 0;	//is a valid hex
	}

	else if(*p == '+' || *p == '-' || (*p >= '0' && *p <= '9'))
	{
		//first char is +, -, or a digit
		p++;
		for (i = 1; i < len; i++)
		{
			if (*p >= '0' && *p <= '9')
			{
				p++;
				continue;
			}
			else
			{
				return 1;	//error, not a valid digit
			}
		}
		return 0;	//valid number found
	}
	return 1;	//not a number found
}

/*
 *  Check if it is a valid function name, return 1 if good name, 0 otherwise
 */
int isFunc(char *token)
{
	char *p = token;	//use p as the pointer
	int i, len;

	len = strlen(token);
	//  Functions must start with an alphabetical character
	if (*p >= 'A' && *p <= 'Z') ||
		(*p >= 'a' && *p <= 'z')
	{
		p++;
		continue;
	}
	else
		return 1;	//  bad function name

	for (i = 1; i < len; i++)
	{
		//  After first character, functions can be composed of alphbetical characters 
		//  and digits only
		if (*p >= 'A' && *p <= 'Z') ||
			(*p >= 'a' && *p <= 'z') || 
			(*p >= '0' && *p <= '9') 
		{
			p++;
			continue;
		}
		else
		{
			return 1;	//  bad function name
		}
	}
	return 0;	//  Valid function name
}

/*
 *  parse the .j file and generate .asm code
 */

void parse (FILE *fin, FILE *fout)
{

	char token[MAX_TOKEN_LENGTH];

	while (gettoken(fin, token))
 	{
  		if (isNum(token)) == 0)
 		{
 			writePush(token, fout);
 		}
 		else if (strcmp(token, "drop") == 0)
 		{
 			writeDrop(fout);
 		}
 		else if (strcmp(token, "dup") == 0)
 		{
 			writeDup(fout);
 		}
 		else if (strcmp(token, "swap") == 0)
 		{
 			writeSwap(fout);
 		}
 		else if (strcmp(token, "rot") == 0)
 		{
 			writeRot(fout);
 		}
 		else if (strcmp(token, "pick") == 0)
 		{
 			writePick(fout);
 		}
 		else if (strcmp(token, "if") == 0)
 		{
 			writeIf(fout);
 		}
 		else if (strcmp(token, "else") == 0)
 		{
 			writeElse(fout);
 		}
 		else if (strcmp(token, "endif") == 0)
 		{
 			writeEndif(fout);
 		}
 		else if ((strcmp(token, "+") == 0) || (strcmp(token, "-") == 0) || 
 			(strcmp(token, "*") == 0) || (strcmp(token, "/") == 0) || (strcmp(token, "%%") == 0))
 		{
 			writeArith(token, fout);
 		}
 		else if ((strcmp(token, "lt") == 0) || (strcmp(token, "le") == 0) || 
 			(strcmp(token, "eq") == 0) || (strcmp(token, "ge") == 0) || (strcmp(token, "gt") == 0))
 		{
 			writeCompare(token, fout);
 		}
 		else if ((strcmp(token, "and") == 0)) || (strcmp(token, "or") == 0) || 
 			(strcmp(token, "not") == 0)
 		{
 			writeLogic(token, fout);
 		}
 		else if (strcmp(token, "defun") == 0)
 		{
 			// define the function, Read the function name
 			if (getToken(fin, token) == 0)	// EOF encountered
 				break;
 			if (isFunc(token) == 0);	//is valid function name?
 				writeDefun(token, fout);
 		}
 		else if (strcmp(token, "return") == 0)
 		{
 			writeFun(token, fout);
 		}
 		else
 		{
 			//if none of above must be a user created function name
 			writeFun(token, fout);
 		}
 	}
}
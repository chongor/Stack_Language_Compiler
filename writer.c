/*
 * writer.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "writer.h"

int labelnum;
int stk = 0;
char stkc[256];
int stkn[256];
int ifnum;
char prgname[121];

int initWrite(char *filename)
{
	ifnum = 0;
	initLabel(filename);
	prgname = toupper(prgname);
}

//  Push new elements onto the stack
int writePush(char *token, FILE *fout)
{
	char str[241];
	sprintf(str, "\tCONST R7, #%s\n", token);
	fputs(str, fout);
	fputs("\tSTR R7, R6, #-1\n", fout);
	fputs("\tADD R6, R6, #-1\n", fout);
	return 0;
}

//  Drop the top element of the stack
void writeDrop(FILE *fout)
{
	fputs("\tADD R6, R6, #1\n", fout);
}

void writeDup(FILE *fout)
{
	fputs("\tLDR R7, R6, #0\n", fout);
	fputs("\tSTR R7, R6, #-1\n", fout);
	fputs("\tADD R6, R6, #-1\n", fout);
}

//  Swap the top two elements of the stack with each other
void writeSwap(FILE *fout)
{
	fputs("\tLDR R7, R6, #0\n", fout);
	fputs("\tLDR R3, R6, #1\n", fout);
	fputs("\tSTR R7, R6, #1\n", fout);
	fputs("\tSTR R3, R6, #0\n", fout);
}

//  Rotate the top three elements.  i.e. 1 2 3 => 2 3 1
void writeRot(FILE *fout)
{
	fputs("\tLDR R7, R6, #0\n", fout);
	fputs("\tLDR R3, R6, #1\n", fout);
	fputs("\tLDR R2, R6, #2\n", fout);
	fputs("\tSTR R3, R6, #2\n", fout);
	fputs("\tSTR R7, R6, #1\n", fout);
	fputs("\tSTR R2, R6, #0\n", fout);
}

//  Find # from top element of stack, pick element # down the stack and replicate at top of stack
void writePick(FILE *fout)
{
	fputs("\tADD R7, R6, #0\n", fout);	//R7 = Address of top of stack
	fputs("\tLDR R3, R6, #0\n", fout);	//R3 = data contained at top of stack
	fputs("\tADD R6, R6, R3\n", fout);	//Move R6 to address at picked element R3 down
	fputs("\tLDR R3, R6, #0\n", fout); 	//Load value at picked address
	fputs("\tSTR R3, R7, #0\n", fout);	//Store the value from picked address into top of stack
	fputs("\tADD R6, R7, #0\n", fout);	//Reset R6 to address at top of stack
}


void initLabel(char *pname)
{
	strcpy(prgname, pname);
}
void getLabel(char *lname)
{
	labelnum++;
	sprintf(lname, "J%d_%s\n", labelnum, prgname);
}

void writeIf(FILE *fout)
{
	char str [242];

	stkc[stk] = 'N';
	stkn[stk] = ifnum;
	stk++;
	stkc[stk] = 'S';
	stkn[stk] = ifnum;
	stk++;

	fputs("\tLDR R7, R6 #0\n", fout);	//load top element of stack into R7
	fputs("\tCONST R3, #0\n", fout);	//Set R3 to 0/false
	sprintf(str,"\tBRz %s_IF%d\n", prgname, ++ifnum);		//If zero branch to else
	fputs(str, fout);
}
void writeElse(FILE *fout)
{
	char str [242];

	stk--;
	if (stkc[stk] == 'S')
	{
		sprintf(str,"\tJMP %s_ENDIF%d\n", prgname, stkn[stk]);
	}

	sprintf(str,"%s_IF%d\n", prgname, ifnum);
}
void writeEndif(FILE *fout)
{
	stk--;
	if (stkc[stk] == 'S')
	{
		sprintf(str,"%s_IF%d\n", prgname, stkn[stk]);
		stk --;;
	}
	else if(stkc[stk] == 'N')
	{
		sprintf(str,"%s_ENDIF%d\n", prgname, stkn[stk]);
		stk--;
	}
}

void writeArith(char *token, FILE *fout)
{

	fputs("\tLDR R7, R6, #0\n", fout);	//R7 has value of top of stack
	fputs("\tLDR R3, R6, #1\n", fout);	//R3 has value of second to top of stack
	fputs("\tADD R6, R6, #1\n", fout);	//R6 addr now at second to top of stack

	//Addition
	if (strcmp(token, "+") == 0)
	{
		fputs("\tADD R7, R7, R3\n", fout);	//Do addition store in R7
		fputs("\tSTR R7, R6, #0\n", fout);	//Store R7 value into R6 address
	}
	//Subtraction
	else if (strcmp(token, "-") == 0)
	{
		fputs("\tSUB R7, R7, R3\n", fout);	//Do subtraction store in R7
		fputs("\tSTR R7, R6, #0\n", fout);	//Store R7 value into R6 address
	}

	//Multiplication
	else if (strcmp(token, "*") == 0)
	{
		fputs("\tMULT R7, R7, R3\n", fout);	//Do multiplication store in R7, pray nothing explodes
		fputs("\tSTR R7, R6, #0\n", fout);	//Store R7 value into R6 address
		break;
	}
	//Division
	else if (strcmp(token, "/") == 0)
	{
		fputs("\tDIV R7, R7, R3\n", fout);	//Do division store in R7
		fputs("\tSTR R7, R6, #0\n", fout);	//Store R7 value into R6 address
		break;
	}
	printf("Error, writeArith given invalid token\n");
}

void writeCompare(char *token, FILE *fout)
{
	char label1[121];
	char label2[121];
	char str[242];

	fputs("\tLDR R7, R6, #0\n", fout);
	fputs("\tLDR R3, R6, #1\n", fout);
	fputs("\tCMP R7, R3\n", fout);
	fputs("\tADD R6, R6, #1\n", fout);

	//less than
	if (strcmp(token, "lt") == 0)
	{
		getLabel(&label1);
		getLabel(&label2);

		str = "\tBRn";
		strcat(str, label1);

		fputs(str, fout);

		//Setting R7 to false
		fputs("\tCONST R7, #0\n", fout);
		fputs("\tSTR R7, R6, #0\n", fout);

		str = "\tJMP";
		strcat (str, label2);

		fputs(str, fout);

		//Setting R7 to true
		strcat(str, label1);
		fputs(str, fout);
		fputs("\tCONST R7, #1\n", fout);

		//storing output
		strcat(str, label2);
		fputs(str, fout);
	}
	//less than equal to
	else if (strcmp(token, "le") == 0)
	{
		getLabel(&label1);
		getLabel(&label2);

		str = "\tBRnz";
		strcat(str, label1);

		fputs(str, fout);

		//Setting R7 to false
		fputs("\tCONST R7, #0\n", fout);
		fputs("\tSTR R7, R6, #0\n", fout);

		str = "\tJMP";
		strcat (str, label2);

		fputs(str, fout);

		//Setting R7 to true
		strcat(str, label1);
		fputs(str, fout);
		fputs("\tCONST R7, #1\n", fout);

		//storing output
		strcat(str, label2);
		fputs(str, fout);
	}
	//equal to
	else if (strcmp(token, "eq") == 0)
	{
		getLabel(&label1);
		getLabel(&label2);

		str = "\tBRz";
		strcat(str, label1);

		fputs(str, fout);

		//Setting R7 to false
		fputs("\tCONST R7, #0\n", fout);
		fputs("\tSTR R7, R6, #0\n", fout);

		str = "\tJMP";
		strcat (str, label2);

		fputs(str, fout);

		//Setting R7 to true
		strcat(str, label1);
		fputs(str, fout);
		fputs("\tCONST R7, #1\n", fout);

		//storing output
		strcat(str, label2);
		fputs(str, fout);
	}
	//greater than equal to
	else if (strcmp(token, "ge") == 0)
	{
		getLabel(&label1);
		getLabel(&label2);

		str = "\tBRzp";
		strcat(str, label1);

		fputs(str, fout);

		//Setting R7 to false
		fputs("\tCONST R7, #0\n", fout);
		fputs("\tSTR R7, R6, #0\n", fout);

		str = "\tJMP";
		strcat (str, label2);

		fputs(str, fout);

		//Setting R7 to true
		strcat(str, label1);
		fputs(str, fout);
		fputs("\tCONST R7, #1\n", fout);

		//storing output
		strcat(str, label2);
		fputs(str, fout);
	}
	//greater than
	else if (strcmp(token, "gt") == 0)
	{
		getLabel(&label1);
		getLabel(&label2);

		str = "\tBRp";
		strcat(str, label1);

		fputs(str, fout);

		//Setting R7 to false
		fputs("\tCONST R7, #0\n", fout);
		fputs("\tSTR R7, R6, #0\n", fout);

		str = "\tJMP";
		strcat (str, label2);

		fputs(str, fout);

		//Setting R7 to true
		strcat(str, label1);
		fputs(str, fout);
		fputs("\tCONST R7, #1\n", fout);

		//storing output
		strcat(str, label2);
		fputs(str, fout);
	}
	fputs("\tSTR R7, R6, #0\n", fout);
}

void writeLogic(char *token, FILE *fout)
{
	fputs("\tLDR R7, R6, #0\n", fout);

	if (strcmp(token, "and") == 0)
	{
		fputs("\tLDR R3, R6, #1\n", fout);
		fputs("\tAND R7, R7, R3\n", fout);
		fputs("\tADD R6, R6, #1\n", fout);

	}
	else if (strcmp(token, "or") == 0)
	{
		fputs("\tLDR R3, R6, #1\n", fout);
		fputs("\tOR R7, R7, R3\n", fout);
		fputs("\tADD R6, R6, #1\n", fout);
	}
	else if (strcmp(token, "not") == 0)
	{
		fputs("\tNOT R7, R7\n", fout);
	}
	fputs("\tSTR R7, R6, #0\n", fout);
}

void writeDefun(char *token, FILE *fout)
{
	char str[242];
	str = token;
	strcat(str, "\n");

	fputs("\t.CODE\n", fout);
	fputs("\t.FALIGN\n", fout);
	fputs(str, fout);

	fputs("\tSTR R7, R6, #-2\n", fout);	//return address R6 stored into RA
	fputs("\tSTR R5, R6, #-3\n", fout);	//frame pointer R5 stored into FP
	fputs("\tADD R6, R6, #-3\n", fout);	//Update stack pointer R6 to top of stack
	fputs("\tADD R5, R6, #0\n", fout);	//Update frame pointer R5 to top of stack
}
void writeRet(char *token, FILE *fout)
{
	fputs("\tLDR R7, R6, #0\n", fout);		//R7 holds return value
	fputs("\tSTR R7, R5, #2\n", fout);		//Store the return value into RV
	fputs("\tADD R6, R5, #0\n", fout);		//R6 points to same addr as R5
	fputs("\tLDR R5, R6, #0\n", fout);		//Load the Frame Pointer into R5
	fputs("\tLDR R7, R6, #1\n", fout);		//Load the Return Address into R7
	fputs("\tADD R6, R6, #3\n", fout);		//Pop off 3 on items on stack pointer
	fputs("\tRET\n", fout);					//Return
}
void writeFun(char *token, FILE *fout)
{
	//JSR into the function
	char str[242];
	str = "\tJSR"
	strcat(str, token);
	strcat(str, "\n");

	fputs(str, fout);

	fputs("\tLDR R7, R6 #0\n", fout);
	fputs("\tSTR R7, R5, #-1\n", fout);
}
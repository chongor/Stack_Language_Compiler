/*
 *  parser.h
 */

int isNum (char *token);
int isFunc(char *token);
void parse (FILE *fin, FILE *fout);
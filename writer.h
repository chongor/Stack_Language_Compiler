/*
 * writer.h
 */

int initWrite(char *filename);

int writePush(char *token, FILE *fout);
void writeDrop(FILE *fout);
void writeDup(FILE *fout);
void writeSwap(FILE *fout);
void writeRot(FILE *fout);
void writePick(FILE *fout);

void initLabel(char *pname);
void getLabel(char *lname);

void writeIf(FILE *fout);
void writeElse(FILE *fout);
void writeEndif(FILE *fout);

void writeArith(char *token, FILE *fout);
void writeCompare(char *token, FILE *fout);
void writeLogic(char *token, FILE *fout);

void writeDefun(char *token, FILE *fout);
void writeRet(char *token, FILE *fout);
void writeFun(char *token, FILE *fout);

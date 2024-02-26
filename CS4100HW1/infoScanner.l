%{
//*****************************************************************************************
// File: inforScan.l
// Purpose: To scan through a file and identify potential P numbers and SSNs.
// Obfuscated numbers (XXX-##-1234) are also detected, as those could be pieced together
// into more complete data sets
//
// build instructions
// flex infoScanner.l
// g++ -o infoScanner lex.yy.x
// ./infoScanner < data_file.txt
// ****************************************************************************************

//declarations

#include <iostream>

using namespace std;

#define TKPNUM 1
#define TKSSNUM 2
#define TKOBPNUM 3
#define TKOBSSNUM 4
#define TKUNMATCHED 255

int linecount = 1;

void Addline(char c)
{
	if(c=='\n')
	{
		linecount++;
	}
}

int line_num(){
	return linecount;
}




%}

D [0-9]
OD [0-9Xx#*$]



%%

P{D}{9}					{	cout << "Found a P number" << endl;
							return TKPNUM;
						}
						
P{OD}{9}				{ return TKOBPNUM; }

{D}{9}						{	return TKSSNUM; }

{OD}{9}						{	return TKOBSSNUM;	}

{D}{3}-{D}{2}-{D}{4}		{	return TKSSNUM;	}

{OD}{3}-{OD}{2}-{OD}{4}		{	return TKOBSSNUM;	}

[ \t\n\f]					{	Addline(yytext[0]); }

.							{ 	//unmatched
								return TKUNMATCHED;
							}



%%

int yywrap(){
	return true;
}

int main() {
	int token = 0;
	token = yylex();
	while(token){
		switch (token) {
			case TKPNUM:
				cout << "Found a P number on line " << line_num() << " : " << yytext << endl;
				break;
			case TKSSNUM:
				cout << "Found a SS number on line " << line_num() << " : " << yytext << endl;
				break;
			case TKOBPNUM:
				cout << "Found an obfuscated P number on line " << line_num() << " : " << yytext << endl;
				break;
			case TKOBSSNUM:
				cout << "Found an obfuscated SS number on line " << line_num() << " : " << yytext << endl;
				break;
			}
	
		token = yylex();
	}
}	
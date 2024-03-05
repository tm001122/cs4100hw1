%{
#include <stdio.h>
%}

%option noyywrap

%{
int in_function = 0; // Flag to track if currently inside a function
%}

%%
"/*"([^*]|[*][^/])*[*]*"/"         ; /* Remove multiline comments */
"//".*                             ; /* Remove single-line comments */
[ \t\n]+                           ; /* Remove whitespace and newlines */
[0-9]+                             { printf("%s", yytext); printf(" ");}
"#include".*                       ; /* Ignore include statements */
"if"|"else"|"while"|"for"|"int"|"char"|"float"|"double"|"return"|"void"|"string"|"bool"|"long" { printf("%s", yytext); printf(" "); }
"=="|"!="|"<"|">"|"<="|">="|"&&"|"||"|"+"|"-"|"*"|"/"|"="|"++"|"--"|"%" { printf("%s", yytext); printf(" "); }
"("                                { printf("( "); }
")"                                { printf(") "); }
"{"                                { 
    printf("{ ");
    // if (in_function) {
    //     printf("fundec ( ");
    //     in_function = 0; // Reset the flag
    // }
}
"}"                                { printf("} "); }
";"                                { 
    printf("; ");
    // if (in_function) {
    //     printf("fundec ( ");
    //     in_function = 0; // Reset the flag
    // }
}
[[:alpha:]_][[:alnum:]_]*          { printf("name "); }
.                                  ; /* Ignore any other characters */

%%

int main() {
    yylex();
    // printf("\n");
    return 0;
}

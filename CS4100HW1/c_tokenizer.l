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
[0-9]+                             { printf("CONSTANT: %s\n", yytext); }
"if"|"else"|"while"|"for"|"int"|"char"|"float"|"double"|"return" { printf("KEYWORD: %s\n", yytext); }
"=="|"!="|"<"|">"|"<="|">="|"&&"|"||"|"+"|"-"|"*"|"/"|"="|"++"|"--"|"%" { printf("OPERATOR: %s\n", yytext); }
"("                                { printf("LEFT_PAREN: %s\n", yytext); }
")"                                { printf("RIGHT_PAREN: %s\n", yytext); }
"{"                                { 
    printf("LEFT_BRACE: %s\n", yytext);
    if (in_function) {
        printf("FUNCTION_DECLARATION: %s\n", yytext);
        in_function = 0; // Reset the flag
    }
}
"}"                                { printf("RIGHT_BRACE: %s\n", yytext); }
";"                                { 
    printf("SEMICOLON: %s\n", yytext);
    if (in_function) {
        printf("FUNCTION_DECLARATION: %s\n", yytext);
        in_function = 0; // Reset the flag
    }
}
[[:alpha:]_][[:alnum:]_]*"("        { 
    // Check if it's a function declaration
    printf("FUNCTION_DECLARATION: %s\n", yytext);
    in_function = 0; // Reset the flag
}
[[:alpha:]_][[:alnum:]_]*          ; /* Ignore variable names */
.                                  ; /* Ignore any other characters */

%%

int main() {
    yylex();
    return 0;
}

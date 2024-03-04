%{
#include <stdio.h>
FILE *output_file;
%}

%option noyywrap

%{
int in_function = 0; // Flag to track if currently inside a function
%}

%%
"/*"([^*]|[*][^/])*[*]*"/"   ; /* Remove multiline comments */
"//".*  ; /* Remove single-line comments */
[ \t\n]+ ; /* Remove whitespace and newlines */
[0-9]+ { fprintf(output_file, "%s\n", yytext); }
"#include".*  ; /* Ignore include statements */
"if"|"else"|"while"|"for"|"int"|"char"|"float"|"double"|"return" { fprintf(output_file, "%s", yytext); }
"=="|"!="|"<"|">"|"<="|">="|"&&"|"||"|"+"|"-"|"*"|"/"|"="|"++"|"--"|"%" { fprintf(output_file, "%s", yytext); }
"(" { fprintf(output_file, "("); }
")" { fprintf(output_file, ")"); }
"{" { 
  fprintf(output_file, "{");
  if (in_function) {
    fprintf(output_file, "fundec(");
    in_function = 0; // Reset the flag
  }
}
"}" { fprintf(output_file, "}"); }
";" { 
  fprintf(output_file, ";");
  if (in_function) {
    fprintf(output_file, "fundec(");
    in_function = 0; // Reset the flag
  }
}
[[:alpha:]_][[:alnum:]_]*"(" { 
  // Check if it's a function declaration
  fprintf(output_file, "fundec(");
  in_function = 0; // Reset the flag
}
[[:alpha:]_][[:alnum:]_]* ; /* Ignore variable names */
. ; /* Ignore any other characters */

%%

int main() {
  output_file = fopen("output.txt", "w"); // Replace "output.txt" with desired filename
  if (output_file == NULL) {
    perror("Error opening file");
    return 1;
  }
  yylex();
  fclose(output_file);
  return 0;
}

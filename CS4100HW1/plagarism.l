%{
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

unordered_set<string> submissionTokens;
unordered_set<string> referenceTokens;
int plagiarismCount = 0;
%}

%%
[a-zA-Z_][a-zA-Z0-9_]*   { submissionTokens.insert(yytext); }
.                        { /* Ignore other characters */ }
%%

int main() {
    // Read the submission code
    string submissionCode;
    while (getline(cin, submissionCode)) {
        yy_scan_string(submissionCode.c_str());
        yylex();
    }

    // Read the reference code
    string referenceCode;
    while (getline(cin, referenceCode)) {
        yy_scan_string(referenceCode.c_str());
        yylex();
    }

    // Compare the tokens
    for (const auto& token : submissionTokens) {
        if (referenceTokens.count(token) > 0) {
            plagiarismCount++;
        }
    }

    // Output the plagiarism count
    cout << "Plagiarism count: " << plagiarismCount << endl;

    return 0;
}

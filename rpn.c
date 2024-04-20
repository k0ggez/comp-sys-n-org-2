#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//wcg9ev@virginia.edu

int main() {
    int* stack = (int*)calloc(4096, sizeof(int));
    int* stackStart = stack;
    int tokenBad = 0;
    int sawNewLine = 1;

    while (tokenBad == 0 && sawNewLine == 1) {//stack is preserved while no extra operations or bad tokens, input is wiped each print
        char* input = (char*)calloc(8192, sizeof(char));
        char* inputStart = input;
        fgets(input, 8191, stdin);

        sawNewLine = 0;
        while (tokenBad == 0 && sawNewLine == 0) {
            while (*input == ' ') {//moves input pointer until non whitespace is found
                input++;
            }

            char* token = (char*)malloc(sizeof(char) * 11);
            int tokenLength = 0;
            while (*input != ' ' && *input != '\0' && *input != '\n') {//puts sequential chars into a token until whitespace is found
                token[tokenLength] = *input;
                tokenLength++;
                input++;
            }

            int tokenAsInt = atoi(token);
            if (tokenAsInt != 0 || *token == '0') {//strings atoi to 0, char comparison captures 0, adds integer to stack
                *stack = tokenAsInt;
                stack++;
            }
            else if (tokenLength == 1 && strchr("+-*/", *token) != NULL) {//if token is 1 char and is an operator, performs operation

                if (stack - stackStart < 2) {
                    tokenBad = 1;
                }
                else {
                    stack--;

                    switch (*token) {
                    case('+'):
                        *(stack - 1) += *stack;
                        break;
                    case('-'):
                        *(stack - 1) -= *stack;
                        break;
                    case('*'):
                        *(stack - 1) *= *stack;
                        break;
                    case('/'):
                        *(stack - 1) /= *stack;
                        break;
                    }

                    *stack = 0;
                }
            }
            else if (tokenLength == 0 && *input == '\n') {
                sawNewLine = 1;
            }
            else {//if token isnt (!=0 || =='0') or (length==1 && contains("+-*/")), must be bad token
                tokenBad = 1;
            }

            free(token);
        }

        char b4 = '[';
        for (int i = 0; i < stack - stackStart; i++) {//prints remaining stack
            printf("%c %d", b4, stackStart[i]);
            b4 = ',';
        }
        if (stack - stackStart != 0) {
            puts(" ]");
        }

        free(inputStart);
    }}
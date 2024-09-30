#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAXOPSTACK 64
#define MAXNUMSTACK 64

char opstack[MAXOPSTACK];
int nopstack = 0;
int numstack[MAXNUMSTACK];
int nnumstack = 0;

// Stack Operations
void push_opstack(char op) {
    if (nopstack > MAXOPSTACK - 1) {
        printf("Error: Operator stack overflow\n");
        exit(4);
    }
    opstack[nopstack++] = op;
}

char pop_opstack() {
    if (!nopstack) {
        printf("Error: Operator stack underflow\n");
        exit(4);
    }
    return opstack[--nopstack];
}

void push_numstack(int num) {
    if (nnumstack > MAXNUMSTACK - 1) {
        printf("Error: Number stack overflow\n");
        exit(4);
    }
    numstack[nnumstack++] = num;
}

int pop_numstack() {
    if (!nnumstack) {
        printf("Error: Number stack underflow\n");
        exit(4);
    }
    return numstack[--nnumstack];
}

// Helper Functions
int exponential(int a1, int a2) {
    return a2 < 0 ? 0 : (a2 == 0 ? 1 : a1 * exponential(a1, a2 - 1));
}

int evaluate_operator(int n1, int n2, char op) {
    switch (op) {
        case '+': return n1 + n2;
        case '-': return n1 - n2;
        case '*': return n1 * n2;
        case '_': return -n1; // Unary negation
        case '^': return exponential(n1, n2);
        case '/': if (!n2) exit(3); return n1 / n2;
        case '%': if (!n2) exit(3); return n1 % n2;
        default: return -1;
    }
}

int get_precedence(char ch) {
    switch (ch) {
        case '_': return 10;
        case '^': return 9;
        case '*':
        case '/':
        case '%': return 8;
        case '+':
        case '-': return 5;
        case '(': 
        case ')': return 0;
        default: return -1;
    }
}

int get_associativity(char ch) {
    switch (ch) {
        case '_': 
        case '^': return 2; // Right-to-left
        case '*':
        case '/':
        case '%':
        case '+':
        case '-': return 1; // Left-to-right
        case '(':
        case ')': return 0; // Not an operator
        default: return -1;
    }
}

int is_unary(char ch) {
    return ch == '_';
}

int is_operator(char ch) {
    return ch == '_' || ch == '^' || ch == '*' || ch == '/' || ch == '%' || 
           ch == '+' || ch == '-' || ch == '(' || ch == ')';
}

// Shunting-yard and RPN Evaluation
void shunt_op(char op) {
    char pop;
    int n1, n2;
    
    if (op == '(') {
        push_opstack(op);
        return;
    } 
    else if (op == ')') {
        while (nopstack > 0 && opstack[nopstack - 1] != '(') {
            pop = pop_opstack();
            n1 = pop_numstack();
            if (is_unary(pop))
                push_numstack(evaluate_operator(n1, 0, pop));
            else {
                n2 = pop_numstack();
                push_numstack(evaluate_operator(n2, n1, pop));
            }
        }
        if (!(pop = pop_opstack()) || pop != '(') {
            printf("Error: Mismatched parentheses\n");
            exit(4);
        }
        return;
    }

    if (get_associativity(op) == 2) { // Right-to-left
        while (nopstack && get_precedence(op) < get_precedence(opstack[nopstack - 1])) {
            pop = pop_opstack();
            n1 = pop_numstack();
            if (is_unary(pop))
                push_numstack(evaluate_operator(n1, 0, pop));
            else {
                n2 = pop_numstack();
                push_numstack(evaluate_operator(n2, n1, pop));
            }
        }
    } else { // Left-to-right
        while (nopstack && get_precedence(op) <= get_precedence(opstack[nopstack - 1])) {
            pop = pop_opstack();
            n1 = pop_numstack();
            if (is_unary(pop))
                push_numstack(evaluate_operator(n1, 0, pop));
            else {
                n2 = pop_numstack();
                push_numstack(evaluate_operator(n2, n1, pop));
            }
        }
    }
    push_opstack(op);
}

void convert_infix_to_rpn(char *input) {
    char *expr;
    char *tstart = NULL;
    char startop = 'X'; // Dummy operator to mark start
    char op;
    char lastop = startop;

    for (expr = input; *expr; ++expr) {
        if (!tstart) {
            if (is_operator(*expr)) {
                op = *expr;
                if (lastop != 'Y' && (lastop == startop || lastop != ')')) {
                    if (op == '-') op = '_'; // Unary negation
                    else if (op != '(') {
                        printf("Error: Unexpected operator\n");
                        exit(2);
                    }
                }
                shunt_op(op);
                lastop = op;
            } 
            else if (isdigit(*expr)) {
                tstart = expr;
            } 
            else if (!isspace(*expr)) {
                printf("Error: Invalid character\n");
                exit(1);
            }
        } 
        else {
            if (isspace(*expr)) {
                push_numstack(atoi(tstart));
                tstart = NULL;
                lastop = 'Y';
            } 
            else if (is_operator(*expr)) {
                op = *expr;
                push_numstack(atoi(tstart));
                tstart = NULL;
                shunt_op(op);
                lastop = op;
            } 
            else if (!isdigit(*expr)) {
                printf("Error: Invalid character\n");
                exit(1);
            }
        }
    }
    if (tstart) push_numstack(atoi(tstart));
}

int evaluate_infix(char *expr) {
    int n1, n2;
    char op;

    convert_infix_to_rpn(expr);

    while (nopstack) {
        op = pop_opstack();
        n1 = pop_numstack();
        if (is_unary(op)) 
            push_numstack(evaluate_operator(n1, 0, op));
        else {
            n2 = pop_numstack();
            push_numstack(evaluate_operator(n2, n1, op));
        }
    }
    if (nnumstack != 1) {
        printf("Error: Invalid expression\n");
        return 4;
    }
    return numstack[0];
}

int evaluate_rpn(char *expr) {
    char *ch;
    int n1, n2;

    for (ch = expr; *ch; ++ch) {
        if (isspace(*ch))
            continue;
        if (isdigit(*ch))
            push_numstack(atoi(ch));
        else if (is_operator(*ch)) {
            n1 = pop_numstack();
            if (is_unary(*ch)) 
                push_numstack(evaluate_operator(n1, 0, *ch));
            else {
                n2 = pop_numstack();
                push_numstack(evaluate_operator(n2, n1, *ch));
            }
        } 
        else {
            printf("Error: Invalid character in RPN expression\n");
            exit(1);
        }
    }
    return numstack[0];
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <expression>\n", argv[0]);
        return 1;
    }

    char *input;
    if (argc == 3) {
        input = argv[2];
        printf("%d\n", evaluate_rpn(input));
    } else {
        input = argv[1];
        printf("%d\n", evaluate_infix(input));
    }

    return 0;
}

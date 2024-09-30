
# Infix and RPN Expression Calculator

This is a C program that evaluates mathematical expressions in both **infix** (e.g., `3 + 4 * 2`) and Reverse Polish Notation (RPN) or postfix format (e.g., `3 4 2 * +`). It supports basic arithmetic operations like addition, subtraction, multiplication, division, modulo, exponentiation, and unary negation.

## Features

- Infix expression evaluation: Supports standard mathematical notation.
- RPN (postfix) expression evaluation: Supports Reverse Polish Notation.
- Supports operators: `+`, `-`, `*`, `/`, `%`, `^` (exponentiation), unary `-`.
- Handles operator precedence and associativity.
- Basic error handling for invalid input or operations (e.g., division by zero).

## Prerequisites

Make sure you have a C compiler installed. We recommend `gcc` (GNU Compiler Collection), which is typically available on Linux, macOS, and can be installed on Windows.

## Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/yourusername/calculator.git
cd calculator
```

### 2. Compile the program

Use `gcc` to compile the C program:

```bash
gcc calculator.c -o calculator
```

### 3. Run the program

You can run the program with either an infix or an RPN (postfix) expression.

#### For infix expressions:

```bash
./calculator "3 + 4 * 2 - 5"
```

#### For RPN (postfix) expressions:

```bash
./calculator rpn "3 4 2 * + 5 -"
```

## Example

### Infix Example:

```bash
./calculator "3 + 4 * 2 - 5"
```

Output:
```
6
```

### RPN Example:

```bash
./calculator rpn "3 4 2 * + 5 -"
```

Output:
```
6
```

## Program Explanation

- The program uses two stacks:
  - **opstack**: A stack for operators.
  - **numstack**: A stack for numbers.
- The `shunt_op` function implements the Shunting Yard algorithm to convert infix expressions into RPN.
- The program supports basic arithmetic, exponentiation, and unary negation.

### Error Handling

The program exits with appropriate error messages for:
- Invalid characters in the input (Error 1).
- Invalid use of operators (Error 2).
- Division or modulo by zero (Error 3).
- Stack overflows or other stack errors (Error 4).

## Supported Operators

- `+` : Addition
- `-` : Subtraction
- `*` : Multiplication
- `/` : Division
- `%` : Modulo
- `^` : Exponentiation
- `_` : Unary minus (used internally for negative numbers)

## License

This project is licensed under the MIT License - see the LICENSE file for details.

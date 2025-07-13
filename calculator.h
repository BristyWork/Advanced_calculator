#pragma once

enum class Operation {
    ADD = 1, SUBTRACT, MULTIPLY, DIVIDE, POWER,
    SQUARE_ROOT, MODULUS, PERCENTAGE, EXIT
};

double calculate(Operation op, double a, double b = 0);
void saveToHistory(Operation op, double a, double b, double result);
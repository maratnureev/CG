#include <complex>
#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <math.h>

typedef std::complex<double> complex;

const double TOLERANCE = 1.0e-8;

inline bool IsZero(complex x)
{
    return (fabs(x.real()) < TOLERANCE) && (fabs(x.imag()) < TOLERANCE);
}

// Returns n=0..numComplexValues, and fills in outArray with the n values from
// inArray that are real-valued (i.e., whose imaginary parts are within TOLERANCE of 0.)
// outArray must be large enough to receive numComplexValues values.
int FilterRealNumbers(int numComplexValues, const complex inArray[], double outArray[])
{
    int numRealValues = 0;
    for (int i = 0; i < numComplexValues; ++i)
    {
        if (fabs(inArray[i].imag()) < TOLERANCE)
        {
            outArray[numRealValues++] = inArray[i].real();
        }
    }
    return numRealValues;
}

// Returns n=0..2, the number of distinct real roots found for the equation
//
//     ax^2 + bx + c = 0
//
// Stores the roots in the first n slots of the array 'roots'.
int SolveQuadraticEquation(complex a, complex b, complex c, complex roots[2])
{
    if (IsZero(a))
    {
        if (IsZero(b))
        {
            // The equation devolves to: c = 0, where the variable x has vanished!
            return 0;   // cannot divide by zero, so there is no solution.
        }
        else
        {
            // Simple linear equation: bx + c = 0, so x = -c/b.
            roots[0] = -c / b;
            return 1;   // there is a single solution.
        }
    }
    else
    {
        const complex radicand = b * b - 4.0 * a * c;
        if (IsZero(radicand))
        {
            // Both roots have the same value: -b / 2a.
            roots[0] = -b / (2.0 * a);
            return 1;
        }
        else
        {
            // There are two distinct real roots.
            const complex r = sqrt(radicand);
            const complex d = 2.0 * a;

            roots[0] = (-b + r) / d;
            roots[1] = (-b - r) / d;
            return 2;
        }
    }
}

complex cbrt(complex a, int n)
{
    /*
        This function returns one of the 3 complex cube roots of the complex number 'a'.
        The value of n=0..2 selects which root is returned.
    */

    const double TWOPI = 2.0 * M_PI;

    double rho = pow(abs(a), 1.0 / 3.0);
    double theta = ((TWOPI * n) + arg(a)) / 3.0;
    return complex(rho * cos(theta), rho * sin(theta));
}

// Returns n=0..3, the number of distinct real roots found for the equation
//
//     ax^3 + bx^2 + cx + d = 0
//
// Stores the roots in the first n slots of the array 'roots'.
int SolveCubicEquation(complex a, complex b, complex c, complex d, complex roots[3])
{
    if (IsZero(a))
    {
        return SolveQuadraticEquation(b, c, d, roots);
    }

    b /= a;
    c /= a;
    d /= a;

    complex S = b / 3.0;
    complex D = c / 3.0 - S * S;
    complex E = S * S * S + (d - S * c) / 2.0;
    complex Froot = sqrt(E * E + D * D * D);
    complex F = -Froot - E;

    if (IsZero(F))
    {
        F = Froot - E;
    }

    for (int i = 0; i < 3; ++i)
    {
        const complex G = cbrt(F, i);
        roots[i] = G - D / G - S;
    }

    return 3;
}

// Returns n=0..4, the number of distinct real roots found for the equation
//
//     ax^4 + bx^3 + cx^2 + dx + e = 0
//
// Stores the roots in the first n slots of the array 'roots'.
int SolveQuarticEquation(complex a, complex b, complex c, complex d, complex e, complex roots[4])
{
    if (IsZero(a))
    {
        return SolveCubicEquation(b, c, d, e, roots);
    }

    // See "Summary of Ferrari's Method" in http://en.wikipedia.org/wiki/Quartic_function

    // Without loss of generality, we can divide through by 'a'.
    // Anywhere 'a' appears in the equations, we can assume a = 1.
    b /= a;
    c /= a;
    d /= a;
    e /= a;

    complex b2 = b * b;
    complex b3 = b * b2;
    complex b4 = b2 * b2;

    complex alpha = (-3.0 / 8.0) * b2 + c;
    complex beta = b3 / 8.0 - b * c / 2.0 + d;
    complex gamma = (-3.0 / 256.0) * b4 + b2 * c / 16.0 - b * d / 4.0 + e;

    complex alpha2 = alpha * alpha;
    complex t = -b / 4.0;

    if (IsZero(beta))
    {
        complex rad = sqrt(alpha2 - 4.0 * gamma);
        complex r1 = sqrt((-alpha + rad) / 2.0);
        complex r2 = sqrt((-alpha - rad) / 2.0);

        roots[0] = t + r1;
        roots[1] = t - r1;
        roots[2] = t + r2;
        roots[3] = t - r2;
    }
    else
    {
        complex alpha3 = alpha * alpha2;
        complex P = -(alpha2 / 12.0 + gamma);
        complex Q = -alpha3 / 108.0 + alpha * gamma / 3.0 - beta * beta / 8.0;
        complex R = -Q / 2.0 + sqrt(Q * Q / 4.0 + P * P * P / 27.0);
        complex U = cbrt(R, 0);
        complex y = (-5.0 / 6.0) * alpha + U;
        if (IsZero(U))
        {
            y -= cbrt(Q, 0);
        }
        else
        {
            y -= P / (3.0 * U);
        }
        complex W = sqrt(alpha + 2.0 * y);

        complex r1 = sqrt(-(3.0 * alpha + 2.0 * y + 2.0 * beta / W));
        complex r2 = sqrt(-(3.0 * alpha + 2.0 * y - 2.0 * beta / W));

        roots[0] = t + (W - r1) / 2.0;
        roots[1] = t + (W + r1) / 2.0;
        roots[2] = t + (-W - r2) / 2.0;
        roots[3] = t + (-W + r2) / 2.0;
    }

    return 4;
}

class SolverException
{
public:
    SolverException(const char* _message)
        : message(_message)
    {
    }

    const char* GetMessage() const { return message; }

private:
    const char* message;
};
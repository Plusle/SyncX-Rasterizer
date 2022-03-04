#define MATRIX_ROW_MAJOR

#include <math/vector.hpp>
#include <math/matrix.hpp>
#include <iostream>

using namespace SyncX;

int main() {
    std::cout << "Vector Test" << std::endl;
    
    Vector2f v2f{1, 2};
    std::cout << "Vector2f:   " << v2f << "Squared Length: " << v2f.SquaredLength() << " \tLength: " << v2f.Length() << "\n";
    std::cout << "Normalized: " << v2f.Normalized() << "Squared Length: " << v2f.SquaredLength() << " \tLength: " << v2f.Length();

    Vector2f v2f2 {3, 4};
    std::cout << std::endl << "(1) + (2) \\w +  " << v2f + v2f2;
    std::cout << "(2) + (1) \\w += " << (v2f2 += v2f);
    std::cout << "(2) /  2  \\w /= " << (v2f2 /= 2);
    std::cout << "(1) *  2  \\w *= " << (v2f2 *= 2);

    std::cout << std::endl;
    // Vector3f v3 {1, 2, 3};
    std::cout << "The size of Vector3 is " << sizeof(Vector3f) << ", which is " << sizeof(Vector3f) / sizeof(float) <<  " times of float" << std::endl;


    std::cout << "\n\nMatrix3 Test" << std::endl;

    Matrix3f m31;

    Matrix3f m3_row(Vector3f(1, 2, 3), Vector3f(1, 2, 3), Vector3f(1, 2, 3));

    Matrix3f m32(m3_row);

    Matrix3f m3_col(Vector3f(1, 2, 3), Vector3f(1, 2, 3), Vector3f(1, 2, 3));
    std::cout << "Show all: \n" << m31 << m32 << m3_row << m3_col << std::endl;

    std::cout << "Addition: " << std::endl;
    std::cout << m3_row + m3_col << std::endl;

    std::cout << "Substraction: " << std::endl;
    std::cout << m3_row - m3_col << std::endl;

    std::cout << "Coeffwise Multiplication: \n";
    std::cout << CoefficentWiseProduct(m3_row, m3_col) << std::endl;

    std::cout << "Matrix Multipication: " << std::endl;
    std::cout << m3_row * m3_col << std::endl;

    std::cout << "Multiplication with scalar: \n";
    std::cout << m3_row * 3 << std::endl;

    std::cout << "Division by 2: \n";
    std::cout << m3_row / 2 << std::endl;

    std::cout << "Access coeff one by one: \n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << m3_row[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "Assignment: \n";
    std::cout << (m3_col = m3_row);

    std::cout << "Comparision: \n";
    std::cout << std::boolalpha;
    std::cout << (m31 == m3_row) << " ";
    std::cout << (m3_col == m3_row);


    std::cout << "\n\nMatrix4 Test" << std::endl;

    Matrix4f m0;
    Matrix4f m4_row(Vector4f(1, 1, 1, 1), Vector4f(2, 2, 2, 2), Vector4f(3, 3, 3, 3), Vector4f(4, 4, 4, 4));
    Matrix4f m4_col(Vector4f(1, 2, 3, 4), Vector4f(1, 2, 3, 4), Vector4f(1, 2, 3, 4), Vector4f(1, 2, 3, 4));
    Matrix4f m_copy(m4_row);

    std::cout << "Show All: \n" << m0 << m_copy << m4_row << m4_col << std::endl;

    std::cout << "Addition: " << std::endl;
    std::cout << m4_col + m4_row << std::endl;

    std::cout << "Substraction: " << std::endl;
    std::cout << m4_col - m4_row << std::endl;

    std::cout << "Coeffwise Multiplication: " << std::endl;
    std::cout << CoefficentWiseProduct(m4_col, m4_row) << std::endl;

    std::cout << "Matrix Multipication: " << std::endl;
    std::cout << m4_row * m4_col << std::endl;

    std::cout << "Multiplication with scalar: " << std::endl;
    std::cout << m4_row * 3 << std::endl;

    std::cout << "Division by 2: " << std::endl;
    std::cout << m4_row / 2 << std::endl;

    std::cout << "Access coeff ony by one: " << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << m4_row[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Assignment: " << std::endl;
    std::cout << (m_copy = m0) << std::endl;

    std::cout << "Comparision: " << std::endl;
    std::cout << (m4_col == m4_row) << " " << (m4_row == m4_row) << std::endl;

    Matrix4f m4xx(Vector4f(1, 2, 3, 4), Vector4f(2, 4, 6, 7), Vector4f(3, 5, 6, 9), Vector4f(4, 6, 8, 9));
    std::cout << m4xx << std::endl;
    std::cout << MakeMatrix3from4(m4xx) << std::endl;
    std::cout << MakeMinor(m4xx, 2, 1) << std::endl;

    std::cout << Determinant(m4xx) << std::endl;
    std::cout << Determinant(MakeMatrix3from4(m4xx)) << std::endl;
    std::cout << MakeInverse(m4xx) << std::endl;

    return 0;
}
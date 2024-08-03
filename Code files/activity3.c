#include <iostream>

namespace Complex {
    class complex {
    private:
        int a, b;

    public:
        // Constructor with zero arguments
        complex() : a(1), b(1) {}

        // Constructor with two arguments
        complex(int x, int y) : a(x), b(y) {}

        int geta() {
            return a;
        }

        int getb() {
            return b;
        }
    };
}

int main() {
    Complex::complex c1; // Declare complex number using default constructor
    Complex::complex c2(5, 10); // Declare complex number using parameterized constructor

    std::cout << "Complex number 1: " << c1.geta() << " + " << c1.getb() << "i" << std::endl;
    std::cout << "Complex number 2: " << c2.geta() << " + " << c2.getb() << "i" << std::endl;

    return 0;
}

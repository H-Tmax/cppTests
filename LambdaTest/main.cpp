#include <iostream>

int main() {


    // [external variables](input argument)

    //Lambda test 1 - giving function input as arguments
    int result = [](int a, int b) -> int { return a * b; }(5, 10);
    std::cout << result << std::endl;

    //Labmda test 2 - giving function input as external variables
    int a = 5;
    int b = 10;
    int result2 = [a, b]() -> int { return a * b; }();
    std::cout << result2 << std::endl;

    //Lambda test 3 - using either & both
    a = 10;
    b = 10;
    int result3 = [a, b](int i, int j) -> int { return a * b; }(5, 10);
    std::cout << result3 << std::endl;

    int result4 = [a, b](int i, int j) -> int { return i * j; }(5, 10);
    std::cout << result4 << std::endl;

    int result5 = [a, b](int i, int j) -> int { return i * j * a * b; }(5, 10);
    std::cout << result5 << std::endl;

    //Lambda test 4 - using abbreviated syntax
    int result6 = [](int a, int b) { return a * b; }(3, 3);
    std::cout << result6 << std::endl;

    a = 5;
    b = 5;
    int result7 = [a, b]() { return a * b; }();
    std::cout << result7 << std::endl;

    /**
     * Lambda abbreviated form's syntax
     *
     * foo() should work the same way with how [](){}() should work
     *
     * for example, let there be a dummy function foo()
     *
     * int foo(int a, int b){
     *  return a * b;
     * }
     *
     * and in anther function, foo got called as following:
     * int x = foo(1,2);
     *
     * What Lambda does is the combination of the above two.
     *
     * 1. The method's parameter goes into "[]()"{}()
     * 2. The method's definition goes into []()"{}"()
     * 3. The method's argument goes into [](){}"()"
     *
     * So in case of the above foo, its definition and function call can be written as the following:
     * int x = [](int one, int two){return one * two;}(1,2);
     *
     * Example CODE:
     * int x = [](int one, int two){return one * two;}(1,2);
     * std::cout << x << std::endl;
     *
     * 4. For input parameter, "[]()"{}(), [] is the special kid that "captures" external variables
     *    [=] captures all the variables in the given scope by value
     *    [&] captures all the variables in the given scope by reference
     *    [var1, &var2] external variables desired to be captured can be specified
     *    [=, var1] does not work since "=" already captures all
     *    [&, &var2] does not work for the same reason above
     *
     *    I think [] exists to maximize the utility of the Lambda function.
     *
     * Example CODE:
     * int aa = 1;
     * int bb = 2;
     * int cc = 3;
     * int dd = 4;
     * int ee = 5;
     * int x = [](int one, int two, int three, int four, int five){return one + two + three + four + five;}(aa, bb, cc, dd, ee);
     * std::cout << x << std::endl;
     *
     *    In the above Lambda function, without using the capture, parameters used in the Lambda function should be
     *    defined in the Lambda function, as well as the variables should be given as arguments.
     *
     * Example CODE:
     * int y = [=](){return aa + bb + cc + dd + ee;}();
     * std::cout << x << std::endl;
     *
     *    By using the capture, the above Lambda function can directly, and eloquantly ha
     */

    return 0;
}
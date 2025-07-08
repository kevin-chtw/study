#include "epoll.h"
#include <iostream>

int main()
{
    int a[3][3] = {{1, 2, 3}, {4, 5}, {6, 7, 8}};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }

    int b[][3] = {1, 2, 3, 4};
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << b[i][j] << " ";
        }
        std::cout << std::endl;
    }
    int c[3][4] = {1, 2, 3};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << c[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return run();
}

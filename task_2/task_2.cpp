#include <iostream>
#include <cmath> // std::fmax

void print_array(double **arr, int size)
{
    std::cout.precision(4);
    for (int i = 0; i < size; i += 1)
    {
        for (int j = 0; j < size; j += 1)
            std::cout << arr[i][j] << "\t";
        printf("\n");
    }
    printf("\n");
}

void initialize_array(double **A, int size)
{
    for (int i = 0; i < size; i++)
        A[i] = new double[size];

    A[0][0] = 10.0;
    A[0][size - 1] = 20.0;
    A[size - 1][size - 1] = 30.0;
    A[size - 1][0] = 20.0;

    double step = 10.0 / (size - 1), step2 = step * 2;

    for (int i = 1; i < size - 1; i++)
    {
        A[0][i] = A[0][i - 1] + step; // horizontal
        A[size - 1][i] = A[size - 1][i - 1] + step; // horizontal
        A[i][0] = A[i - 1][0] + step; // vertical
        A[i][size - 1] = A[i - 1][size - 1] + step; // vertical
        A[i][i] = A[i - 1][i - 1] + step2; // main diagonal
        A[i][size - 1 - i] = 20.0; // side diagonal
    }
}

int main()
{
    // Initialization
    int net_size = 128, iter_max = 1e6;
    double accuracy = 1e-6;
    double **Anew = new double *[net_size],
           **A = new double *[net_size];

    // 10 20 30 20
    initialize_array(A, net_size);
    initialize_array(Anew, net_size);

    // print_array(A, net_size);

    double error;
    int iter = 0;

    std::cout.precision(4);

    do
    {
        error = 0.0;

        for (int j = 1; j < net_size - 1; j++)
            for (int i = 1; i < net_size - 1; i++)
            {
                // Average
                Anew[j][i] = (A[j][i + 1] + A[j][i - 1] + A[j - 1][i] + A[j + 1][i]) * 0.25;
                error = std::fmax(error, std::abs(Anew[j][i] - A[j][i]));
            }

        // copy array
        for (int k = 0; k < net_size; k++)
            for (int j = 0; j < net_size; j++)
                A[k][j] = Anew[k][j];

        if (!(iter % 1000))
            std::cout << "iter=" << iter << ",\terror=" << error << std::endl;

        iter++;
    } while (error > accuracy && iter < iter_max);
    // print_array(A, net_size);
    return 0;
}
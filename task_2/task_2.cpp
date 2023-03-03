#include <iostream>
#include <cmath> // std::fmax

void print_array(double **arr, int size)
{
#pragma acc data copyout(arr[:size][:size])
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
#pragma acc data present(A[:size][:size])
#pragma acc data copyin(size)
    A[0][0] = 10.0;
    A[0][size - 1] = 20.0;
    A[size - 1][size - 1] = 30.0;
    A[size - 1][0] = 20.0;

    double step = 10.0 / (size - 1), step2 = step * 2;
#pragma acc kernels
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
#pragma acc data copyin(net_size)
    double accuracy = 1e-6;
    double **Anew = new double *[net_size],
           **A = new double *[net_size];
    for (int i = 0; i < net_size; i++)
        A[i] = new double[net_size];
    for (int i = 0; i < net_size; i++)
        Anew[i] = new double[net_size];

#pragma acc enter data create(A[:net_size][:net_size], Anew[:net_size][:net_size])

    // 10 20 30 20
    initialize_array(A, net_size);
    initialize_array(Anew, net_size);

    // print_array(A, net_size);

    double error;
    int iter = 0;
#pragma acc data copyin(iter, error)
    std::cout.precision(4);
    do {
        error = 0.0;
#pragma acc parallel loop present(A[:net_size][:net_size], Anew[:net_size][:net_size]) reduction(max:error)
        for (int j = 1; j < net_size - 1; j++)
#pragma acc loop reduction(max:error)
            for (int i = 1; i < net_size - 1; i++)
            {
                // Average
                Anew[j][i] = (A[j][i + 1] + A[j][i - 1] + A[j - 1][i] + A[j + 1][i]) * 0.25;
                error = std::fmax(error, std::abs(Anew[j][i] - A[j][i]));
            }

        // copy array
#pragma acc parallel loop
        for (int k = 0; k < net_size; k++)
#pragma acc loop
            for (int j = 0; j < net_size; j++)
                A[k][j] = Anew[k][j];

        if (!(iter % 1000))
        {
#pragma acc data copyout(iter, error)
            std::cout << "iter=" << iter << ",\terror=" << error << std::endl;
        }

        iter++;
    } while (error > accuracy && iter < iter_max);
    // print_array(A, net_size);
#pragma acc exit data delete(A[:net_size][:net_size], Anew[:net_size][:net_size])
    return 0;
}

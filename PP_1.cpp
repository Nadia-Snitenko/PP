#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include "mpi.h" //+

using namespace std

int main(int argc, char* argv[])
{
    int size = 1000;
    const vector <int> counts = { 100, 150, 200, 250, 300,350, 400, 500, 600, 700, 800 ,900, 1000 };
    const vector <double> times = { 0.009, 0.035, 0.075, 0.133, 0.189,0.355, 0.488, 1.056, 2.281, 4.271, 6.979, 11.133 ,16.508 };

    int* matrix_1 = new int[size * size];
    int* matrix_2 = new int[size * size];
    int* result = new int[size * size];

    int process_rank;

    // количество процессов:
    int comm_sz, local_N;
    int i, j, k;

    double start = 0, finish = 0; // переменные для замера времени:
    int tem;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    local_N = size / comm_sz;  // количество строк, выделенных для каждой матрицы

    // каждому процессу присваивается матрица:
    int* loc_mat_one = new int[local_N * size];


    // в каждом процессе матрица результатов: 
    int* loc_res = new int[local_N * size];


    if (process_rank == 0)
    {
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                matrix_1[i * size + j] = (i + 2) * (j + 1);

        for (j = 0; j < size; j++)
            for (i = 0; i < size; i++)
                matrix_2[i * size + j] = (i + 1) + 2 * (j + 3);

        start = MPI_Wtime();

        // распределение данных
        MPI_Scatter(matrix_1, local_N * size, MPI_INT, loc_mat_one, local_N * size, MPI_INT, 0, MPI_COMM_WORLD);

        // передача данных
        MPI_Bcast(matrix_2, size * size, MPI_INT, 0, MPI_COMM_WORLD);

        // pасчет результататов
        for (i = 0; i < local_N; i++)
            for (j = 0; j < size; j++) {
                tem = 0;
                for (k = 0; k < size; k++)
                    tem += loc_mat_one[i * size + k] * matrix_2[j * size + k];
                loc_res[i * size + j] = tem;
            }

        delete[] loc_mat_one;


        MPI_Gather(loc_res, local_N * size, MPI_INT, result, local_N * size, MPI_INT, 0, MPI_COMM_WORLD);

        // обработка оставшейся строки
        int rest = size % comm_sz;
        if (rest != 0)
            for (i = size - rest - 1; i < size; i++)
                for (j = 0; j < size; j++) {
                    tem = 0;
                    for (k = 0; k < size; k++)
                        tem += matrix_1[i * size + k] * matrix_2[j * size + k];
                    result[i * size + j] = tem;
                }
        finish = MPI_Wtime();
        delete[] matrix_1;
        delete[] matrix_2;
        delete[] loc_res;

        cout << "Elapsed time = " << finish - start << endl;
    }
    else {
        MPI_Scatter(matrix_1, local_N * size, MPI_INT, loc_mat_one, local_N * size, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(matrix_2, size * size, MPI_INT, 0, MPI_COMM_WORLD);
        for (i = 0; i < local_N; i++)
            for (j = 0; j < size; j++) {
                tem = 0;
                for (k = 0; k < size; k++)
                    tem += loc_mat_one[i * size + k] * matrix_2[j * size + k];
                loc_res[i * size + j] = tem;
            }
        MPI_Gather(loc_res, local_N * size, MPI_INT, result, local_N * size, MPI_INT, 0, MPI_COMM_WORLD);

        delete[] loc_mat_one;
        delete[] matrix_2;
        delete[] loc_res;
    }
    MPI_Finalize();
    return 0;
}
 
    
       


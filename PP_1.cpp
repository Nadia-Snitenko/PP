
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <omp.h> //+

using namespace std;

double** read_matrix_from_the_fail(string name_of_fail,size_t & n_, size_t& m_)
{
    ifstream in(name_of_fail);

    if (in.is_open())
    {
        int count = 0;// количество чисел в файле
        int temp;

        while (!in.eof())
        {
            in >> temp;
            count++;
        }

        //кол-во чисел есть, теперь нужно понять сколько чисел в одной строке
        //посчитаем число пробелов до знака перевода на новую строку

        //переведем курсор в потоке в начало файла
        in.seekg(0, ios::beg);
        in.clear();

        int count_space = 0;
        char symbol;
        while (!in.eof())
        {
            //теперь нужно посимвольно считывать данные
            in.get(symbol);//считали текущий символ
            if (symbol == ' ') count_space++;
            if (symbol == '\n') break;//если дошли до конца строки, то выходим из цикла
        }


        // переходим в начало файла
        in.seekg(0, ios::beg);
        in.clear();

        // знаем сколько чисел в файле и сколько пробелов в первой строке.
        //считываем матрицу:

        int n = count / (count_space + 1);//число строк
        int m = count_space + 1;//число столбцов на единицу больше числа пробелов
        double** x;
        x = new double* [n];
        for (int i = 0; i < n; i++) x[i] = new double[m];

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                in >> x[i][j];
        
        //вывод матрицы
       /* for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
                cout << x[i][j] << "\t";
            cout << "\n";
        }
        cout << "\n";*/

        n_ = n;
        m_ = m;
        return x;

        in.close();
    }
    else
    {
        cout << "Файл не найден.";
    }
}

double** matrix_multiplication(size_t col1, size_t row1, size_t col2, size_t row2, double** m, double** n)
{
    double ** result;

    if (col1 != row2)
    {
        cout << "Умножение невозможно!";
        return 0;
    }
    
    result = new double* [row1];
    omp_set_num_threads(8);
    #pragma omp parallel for shared(col1, row1, col2, row2, m,n) default(none)
   
    for (int i = 0; i < row1; i++)
    {
        //cout << omp_get_num_threads();
        result[i] = new double[col2];
        for (int j = 0; j < col2; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < col1; k++)
                result[i][j] += m[i][k] * n[k][j];
        }
    }
    
   /* cout << "Матрица произведения:" << endl;
    for (int i = 0; i < row1; i++)
    {
        for (int j = 0; j < col2; j++)
            cout << result[i][j] << "\t";
        cout << endl;
    }
    cout << '\n';*/

    return result;
}


void write_matrix_to_fail(string name_of_fail,  size_t col,  size_t row,  double** matrix)
{
    ofstream fout(name_of_fail);
    if (!fout.is_open()) 
    { 
        cerr << "Can't open output file!" << endl;
        exit(1); 
    }

    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
            fout << matrix[i][j] << " ";
        fout << "\n";
    }
}

double** matrix_genetation(size_t x, size_t y)
{

    double** a = new double* [x]; // Создаем массив указателей
    for (int i = 0; i < x; i++)
    {
        a[i] = new double[y]; // Создаем элементы
    }
   
    for (size_t i = 0; i < x; i++)
    {
        for (size_t j = 0; j < y; j++)
        {
            a[i][j] = rand() % 10; // Каждый элемент случайному числу от 0 до 9
            //cout << a[i][j] <<"\t"; 
        }
        //cout << endl; 
    }

    return a;
}

void delete_matrix(double**& matrix, size_t& x, size_t& y)
{
    for (int i = 0; i < x; i++) delete[] matrix[i];
    delete[] matrix;
}

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "RUSSIAN");

    double** m, **n, **result;
    size_t m_x, n_x, n_y, m_y;
    

    string name_1 = "matrix_1.txt";
    string name_2 = "matrix_2.txt";
    string name_3 = "matrix_3.txt";
    string name_4 = "time.txt";
    string name_5 = "counts.txt";

        n_x = m_x = n_y = m_y = 1000;
        int number_of_threads = 4;
        m = matrix_genetation(m_x, m_y);
        n = matrix_genetation(n_x, n_y);
        write_matrix_to_fail(name_1, m_x, m_y, m);
        write_matrix_to_fail(name_2, n_x, n_y, n);
        //m = read_matrix_from_the_fail(name_1, m_x, m_y);
        //n = read_matrix_from_the_fail(name_2, n_x, n_y);

        unsigned int start_time = clock(); // начальное время
        result = matrix_multiplication(m_x, m_y, n_x, n_y, m, n);
        unsigned int end_time = clock(); // конечное время

        write_matrix_to_fail(name_3, m_x, n_y, result);

        delete_matrix(m, m_x, m_y);
        delete_matrix(n, n_x, n_y);
        delete_matrix(result, m_x, n_y);


        unsigned int search_time = end_time - start_time; // искомое время

        cout << "время работы: " << search_time / 1000.0 <<endl;
        //cout << "количество потоков: " << number_of_threads;


        const vector <int> counts = { 10, 50, 100, 150, 200, 250, 300,350, 400, 500, 600, 700, 800 ,900, 1000 };
        const vector <double> times = { 0, 0.001, 0.009, 0.035, 0.075, 0.133, 0.189,0.355, 0.488, 1.056, 2.281, 4.271, 6.979, 11.133 ,16.508 };
    
        ofstream fout(name_4);
        if (!fout.is_open())
        {
            cerr << "Can't open output file!" << endl;
            exit(1);
        }

        for (int i = 0; i < counts.size(); i++)
        {
            fout << times[i] << " ";
        }
       
        return 0;
    
}


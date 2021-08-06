/*
*   Headers
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

/*
*   Functions
*/
void print_m(double * matrix,int rows, int cols);
void init_weights(double * weight, int len);
void dot_prod(double * m1, double * m2, int m1_height, int m2_width, int m2_height, double * prod);
double sigmoid(double x, bool derive);
void sigmoid_all(double * matrix, int len, bool derive);
void calc_error(double * m1, double * m2, int len, double * prod);
void calc_change(double * m1, double * m2, int len, double * prod);
void apply_change(double * changed, double * changed_by, int len);
void transpose(double * arr, int width, int height, double * prod);

/*
*   Main
*/
int main()
{
    /*
    *   Variables
    */

    /*
    * The Answer is always the left most column number
    * One Sequence of three
    */
    double training_input[8] = {2,3,4,5,6,7,8,9};

    double transpose_m[8];

    double training_output[8] = {1,1,0,1,0,1,0,0};

    double new_input[8] = {10,11,12,13,14,15,16,17};

    double neural_output[8] = {};

    double weights[3] = {};

    double error[8] = {};

    double delta[8] = {};

    double change[3] = {};

    /*
    *   Seed
    */
    srand(time(0));

    init_weights(weights,3);

    transpose(training_input,8,1,transpose_m);

    for(int i = 0; i < 10000; i++)
    {
        dot_prod(training_input,weights,1,1,3,neural_output);

        sigmoid_all(neural_output,8,false);

        calc_error(training_output,neural_output,8,error);

        if(i % 2000 == 0)
        {
            double error_avg = 0;
            for(int j = 0; j < 8; j++)
            {

                error_avg += error[j];
                //printf("%f, ", error[j]);
            }
            printf("\nIteration %4d \nError average: %f\n",i,error_avg/8);
        }

        calc_change(neural_output,error,8,delta);

        dot_prod(transpose_m,delta,3,1,8,change);

        apply_change(weights,change,3);
    }

    printf("Trying new input:\n");

    dot_prod(new_input,weights,8,1,3,neural_output);

    sigmoid_all(neural_output,8,false);

    print_m(neural_output,8,1);

    return 0;
}

void print_m(double * matrix, int rows, int cols)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
            printf("\t%0f, ",*matrix++);
        printf("\n");
    }
}
void init_weights(double * matrix_pointer, int len)
{
        for(int j = 0; j < len; j++)
            matrix_pointer[j] = rand() % 2 - 1;
}

void dot_prod(double * m1, double * m2, int m1_height, int m2_width, int m2_height, double * prod)
{
    double sum;
    for(int i = 0; i < m1_height; i++, m1 += m2_height)
        for(int j = 0; j < m2_width; j++)
        {
            sum = 0;
            for(int k = 0; k < m2_height; k++)
                sum += m1[k] * m2[k];
            *prod++ = sum;
        }
}

double sigmoid(double x, bool derive)
{
    if(derive)
        return x * (1 - x);
    return 1 / (1 + exp(-x));
}

void sigmoid_all(double * matrix, int len, bool derive)
{
    for(int i = 0; i < len; i++)
        matrix[i] = sigmoid(matrix[i],derive);
}

void calc_error(double * m1, double * m2, int len, double * prod)
{
    for(int j = 0; j < len; j++)
            *prod++ = *m1++ - *m2++;
}

void calc_change(double * m1, double * m2, int len, double * prod)
{
  sigmoid_all(m1,len,true);
  for(int j = 0; j < len; j++)
            prod[j] = m1[j] * m2[j];
}
void apply_change(double * changed, double * changed_by, int len)
{
    for(int j = 0; j < len; j++)
            changed[j] += changed_by[j];
}

void transpose(double * arr, int width, int height, double * prod)
{
    for(int i = 0; i < width; i++)
        for(int j = 0; j < height; j++)
        {
            int swap1 = i * height + j;
            int swap2 = j * width + i;

            prod[swap2] = arr[swap1];
        }
}


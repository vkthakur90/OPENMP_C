#include <stdio.h>

#ifndef MAX_DATA
#define MAX_DATA 100000000
#endif

struct ProgramData 
{ 
    size_t size;
    float num1[MAX_DATA];
    float num2[MAX_DATA];
    float sum[MAX_DATA];
    float diff[MAX_DATA];
    float res[MAX_DATA];
};

static struct ProgramData data;

void ProgramData_init(struct ProgramData * restrict data_ptr) 
{
    data_ptr->size = MAX_DATA;
    #pragma omp parallel for 
    for (size_t idx = 0; idx < data_ptr->size; ++idx) 
    {
        data_ptr->num1[idx] = (float)((idx + 1) * 3);
        data_ptr->num2[idx] = (float)((idx + 1) * 2);
    }
}

void ProgramData_compute(struct ProgramData * restrict data_ptr) 
{
    #pragma omp parallel for 
    for (size_t idx = 0; idx < data_ptr->size; ++idx) 
    {
        data_ptr->sum[idx]  = data_ptr->num1[idx] + data_ptr->num2[idx];
        data_ptr->diff[idx] = data_ptr->num1[idx] - data_ptr->num2[idx];
        data_ptr->res[idx]  = data_ptr->sum[idx]  * data_ptr->diff[idx];
    }
}

int main() 
{
    ProgramData_init(&data);
    ProgramData_compute(&data);
      
    for (size_t idx = 0; idx < 10; ++idx) 
    {
        printf("%f\n", data.res[idx]);
    }

    return 0;
}

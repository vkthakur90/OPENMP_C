#ifndef MAX_DATA
#define MAX_DATA 1000
#endif

#include <stdio.h>
#include <stdlib.h>

typedef struct _ProgramData ProgramData;

void ProgramData_init(ProgramData * restrict data_ptr);
void ProgramData_compute(ProgramData * restrict data_ptr);
void ProgramData_displayOutput(ProgramData * restrict data_ptr);


static ProgramData data;

int main() {
    ProgramData_init(&data);
    ProgramData_compute(&data);    
    ProgramData_displayOutput(&data);

    return EXIT_SUCCESS;
}

struct _ProgramData { 
    size_t size;
    struct {
        float num1[MAX_DATA];
        float num2[MAX_DATA];
    } inputs;
    struct {
        float sum[MAX_DATA];
        float diff[MAX_DATA];
        float prod[MAX_DATA];
        float ratio[MAX_DATA];
    } outputs;
};

void ProgramData_init(struct _ProgramData * restrict data_ptr) {
    data_ptr->size = MAX_DATA;
    
    #pragma omp parallel for simd schedule(static)
    for (size_t idx = 0; idx < data_ptr->size; ++idx) {
        data_ptr->inputs.num1[idx] = (float)((idx + 1) * 3);
        data_ptr->inputs.num2[idx] = (float)((idx + 1) * 2);
    }
}

void ProgramData_compute(struct _ProgramData * restrict data_ptr) {
    #pragma omp parallel for simd schedule(static)
    for (size_t idx = 0; idx < data_ptr->size; ++idx) {
        float a = data_ptr->inputs.num1[idx];
        float b = data_ptr->inputs.num2[idx];

        data_ptr->outputs.sum[idx]   = a + b;
        data_ptr->outputs.diff[idx]  = a - b;
        data_ptr->outputs.prod[idx]  = a * b;
        data_ptr->outputs.ratio[idx] = (b != 0.0f) ? (a / b) : 0.0f;
    }
}

void ProgramData_displayOutput(struct _ProgramData * restrict data_ptr){
    
    for (size_t idx = 0; idx < 50; ++idx) {
        printf(
            "%d\t%f\t%f\t\t%f\t%f\t%f\t%f\n",
            idx,
            data_ptr->inputs.num1[idx],
            data_ptr->inputs.num2[idx],            
            data_ptr->outputs.sum[idx], 
            data_ptr->outputs.diff[idx],
            data_ptr->outputs.prod[idx],
            data_ptr->outputs.ratio[idx]
        );
    }
}
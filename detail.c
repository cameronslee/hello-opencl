#include <stdio.h>
#include <OpenCL/opencl.h>

int main(void) {
    cl_device_id my_id;
    int res;

    cl_uint n_compute_units;

    cl_ulong mem_size;
    
    res = clGetDeviceIDs(
        NULL,
        CL_DEVICE_TYPE_GPU, 1, &my_id, NULL
    );

    if (res != CL_SUCCESS) {
        printf("error: initialization\n");
        return 1;
    }

    clGetDeviceInfo(
        my_id, CL_DEVICE_MAX_COMPUTE_UNITS,
        sizeof(cl_uint), &n_compute_units, NULL
    );

    printf("MAX COMPUTE UNITS: %d\n", n_compute_units);

    clGetDeviceInfo(
        my_id, CL_DEVICE_GLOBAL_MEM_SIZE,
        sizeof(cl_uint), &n_compute_units, NULL
    );

    printf("GLOBAL MEM SIZE: %llu GB\n", mem_size / 1000000000L);
}

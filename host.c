#include <stdio.h>
#include <OpenCL/opencl.h>

int main(void) {
  cl_device_id gpu;
  cl_context context;
  cl_int result;

  clGetDeviceIDs(
    NULL, CL_DEVICE_TYPE_GPU, 1, &gpu, NULL
  );
  context = clCreateContext(
    NULL, 1, &gpu, NULL, NULL, &result
  );

  if (result != CL_SUCCESS) {
    printf("error: context");
    return 2;
  }

}

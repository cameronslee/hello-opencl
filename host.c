/*
 * Demonstrates how to run computations with a kernel on the GPU
 *
 */
#include <stdio.h>
#include <OpenCL/opencl.h>

int main(void) {
  cl_device_id gpu;
  cl_context context;
  cl_int result;

  cl_command_queue queue;
  cl_program program;

  size_t count = 3;

  int i;

  cl_kernel kernel;

  float in[3] = {9,25,64};
  float out[3] = {0,0,0};

  cl_mem buffer_in, buffer_out;

  FILE *fp;
  char kernel_code[1024];

  clGetDeviceIDs(
    NULL, CL_DEVICE_TYPE_GPU, 1, &gpu, NULL
  );
  context = clCreateContext(
    NULL, 1, &gpu, NULL, NULL, &result
  );

  queue = clCreateCommandQueue(
    context, gpu, 0, &result
  );

  if (result != CL_SUCCESS) {
    printf("error: context");
    return 2;
  }
  // read kernel

  fp = fopen("kernel.cl", "r");
  fread(kernel_code, 1, 1024, fp);
  fclose(fp);
  printf("%s\n", kernel_code);

  // build kernel
  program = clCreateProgramWithSource(
    context, 1, (const char *[]) {kernel_code}, NULL, &result
  );

  if (result != CL_SUCCESS) {
    printf("error: could not create program\n");
    return 3;
  }

  result = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (result != CL_SUCCESS) {
    printf("error: could not build program\n");
    return 4;
  }

  buffer_in = clCreateBuffer(
    context, CL_MEM_READ_ONLY, sizeof(float) * 3,
    NULL, &result
  );

  if (result != CL_SUCCESS) {
    printf("error: could not init input buffer\n");
    return 5;
  }

  buffer_out = clCreateBuffer(
    context, CL_MEM_WRITE_ONLY, sizeof(float) * 3,
    NULL, &result
  );

  if (result != CL_SUCCESS) {
    printf("error: could not init output buffer\n");
    return 6;
  }

  result = clEnqueueWriteBuffer(
    queue, buffer_in, CL_TRUE, 0, sizeof(float) * 3,
    in, 0, NULL, NULL
  );

  if (result != CL_SUCCESS) {
    printf("error: could not enqueue write buffer\n");
    return 7;
  }

  kernel = clCreateKernel(
    program, "square_root", NULL
  );

  clSetKernelArg(
    kernel, 0, sizeof(cl_mem), &buffer_in
  );

  clSetKernelArg(
    kernel, 1, sizeof(cl_mem), &buffer_out
  );

  // run kernel - n dimensional kernel
  clEnqueueNDRangeKernel(
    queue, kernel, 1, NULL, &count, NULL, 0, NULL, NULL 
  );

  clFinish(queue);

  // put into output array
  clEnqueueReadBuffer(
    queue, buffer_out, CL_TRUE, 0, sizeof(float) * 3, 
    out, 0, NULL, NULL
  );

  // print results
  for (i=0; i < 3; i++) {
    printf("%f\n", out[i]);
  }
}

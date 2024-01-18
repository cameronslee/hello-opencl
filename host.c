/*
 * Demonstrates how to run computations with a kernel on the GPU
 *
 */
#include <stdio.h>
#include <OpenCL/opencl.h>

int main(void) {
  cl_context context;
  cl_platform_id platform;
  cl_command_queue queue;
  cl_program program;
  cl_kernel kernel;

  size_t count = 3;
  cl_int result;

  int i;
  FILE *fp;
  char kernel_code[1024];

  
  // input and output buffers for kernel computation
  cl_mem buffer_in, buffer_out;
  float in[3] = {9,25,64};
  float out[3] = {0,0,0};

  /* Obtain Platform */
  result = clGetPlatformIDs(
    1, &platform, NULL
  );

  if (result != CL_SUCCESS) {
    printf("error: unable to get platform id");
    return 1;
  }

  /* Obtain Device ID */
  cl_device_id gpu;
  result = clGetDeviceIDs(
    platform, CL_DEVICE_TYPE_GPU, 1, &gpu, NULL
  );

  if (result != CL_SUCCESS) {
    printf("error: could not obtain device ID");
    return 2;
  }

  /* Create context (abstract container connected to device) for device */
  // contains program kernels and memory objects
  // context holds the command queue used for program execution
  context = clCreateContext(
    NULL, 1, &gpu, NULL, NULL, &result
  );

  if (result != CL_SUCCESS) {
    printf("error: could not create context");
    return 3;
  }

  /* Create command queue */
  queue = clCreateCommandQueue(
    context, gpu, 0, &result
  );

  if (result != CL_SUCCESS) {
    printf("error: could not create command queue");
    return 4;
  }

  /* Read accelerator program source code*/
  fp = fopen("kernel.cl", "r");
  fread(kernel_code, 1, 1024, fp);
  fclose(fp);
  printf("%s\n", kernel_code);

  /* Create Program */
  program = clCreateProgramWithSource(
    context, 1, (const char *[]) {kernel_code}, NULL, &result
  );

  if (result != CL_SUCCESS) {
    printf("error: could not create program\n");
    return 3;
  }

  /* Build Program */
  result = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (result != CL_SUCCESS) {
    printf("error: could not build program\n");
    return 4;
  }
  
  /* Create Kernel */
  kernel = clCreateKernel(
    program, "square_root", NULL
  );
  
  /* Allocate Device Memory */
  buffer_in = clCreateBuffer(
    context, CL_MEM_READ_ONLY, sizeof(float) * 3,
    NULL, &result
  );

  if (result != CL_SUCCESS) {
    printf("error: could not allocate input buffer\n");
    return 5;
  }

  buffer_out = clCreateBuffer(
    context, CL_MEM_WRITE_ONLY, sizeof(float) * 3,
    NULL, &result
  );

  if (result != CL_SUCCESS) {
    printf("error: could not allocate output buffer\n");
    return 6;
  }
  
  /* Move input data to device */
  result = clEnqueueWriteBuffer(
    queue, buffer_in, CL_TRUE, 0, sizeof(float) * 3,
    in, 0, NULL, NULL
  );

  if (result != CL_SUCCESS) {
    printf("error: could not enqueue write buffer\n");
    return 7;
  }

  /* Associate arguements to kernel with kernel object */
  clSetKernelArg(
    kernel, 0, sizeof(cl_mem), &buffer_in
  );

  clSetKernelArg(
    kernel, 1, sizeof(cl_mem), &buffer_out
  );

  /*/ Deplay Kernel */
  clEnqueueNDRangeKernel(
    queue, kernel, 1, NULL, &count, NULL, 0, NULL, NULL 
  );

  // Blocks until all commands in queue are issued and completed 
  clFinish(queue);

  // Move output data to host memory */
  clEnqueueReadBuffer(
    queue, buffer_out, CL_TRUE, 0, sizeof(float) * 3, 
    out, 0, NULL, NULL
  );

  // print results
  for (i=0; i < 3; i++) {
    printf("%f\n", out[i]);
  }

  /* Release context/program/kernels/memory */
  clReleaseMemObject(buffer_in);
  clReleaseMemObject(buffer_out);
  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseContext(context);
  clReleaseCommandQueue(queue);
}

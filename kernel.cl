kernel void square_root(global float *in, global float *out) {
  int id = get_global_id(0);

  out[id] = sqrt(in[id]);

}

kernel void mat_mul(const int N, __global float *A, __global float *B, __global float *C) {
  int i, j, k;

  i = get_global_id(0);
  j = get_global_id(1);

  C[i*N+j] = 0.0f;

  for (k = 0; k < N; k++) {
    C[i*N+j] += A[i*N+k] * B[k*N+j];
  }
}


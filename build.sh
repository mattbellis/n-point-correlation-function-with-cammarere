#NVCC=/usr/local/cuda-11/bin/nvcc
NVCC=/usr/bin/nvcc

$NVCC hello_world.cu -o hello_world
$NVCC vector_add.cu -o vector_add
$NVCC main_bellis_CUDA.cu -o main_bellis_CUDA

#define CL_TARGET_OPENCL_VERSION 120

//Header Files
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <CL/cl.h>
#include <string.h>

void static inline analyse_parsing(int argc1, const char *argv1[], int * num_iterations1, float * diffusion_constant1);

int main(int argc,const char * argv[])
{
  int num_iterations,length,width;
  float diffusion_constant;
  #define d diffusion_constant
  analyse_parsing(argc , argv , &num_iterations , &diffusion_constant);

  /* Open file, read number of characters */
  FILE *fptr;
  fptr = fopen("diffusion","r");
  if (!fptr)
  {
    printf("can't open diffusion file\n");
    exit(0);
  }
  fseek(fptr,0,SEEK_END);
  int last_character = (ftell(fptr));
  fseek(fptr,0,SEEK_SET);
  fscanf(fptr,"%d  %d",&length,&width);
  length+=2;
  width+=2;
  
  /* Assign all doubles with initial value as 0 and boundary too*/  
  double * temperatureold = (double *)malloc(sizeof(double) * length * width);

  for (int i = 0; i < length*width ; ++i)
    temperatureold[i] = 0;

  int x_position,y_position;

  // Initialize the temperature values from file
  do
  {
    fscanf(fptr ,"%d %d ", &x_position, &y_position);
    fscanf(fptr ,"%lf", &temperatureold[(x_position+1)*width + (y_position+1)]);
  }while(ftell(fptr)<last_character);

  fclose(fptr);  

  #define told temperatureold

  cl_int error;

  // Create an OpenCL platform
  cl_platform_id platform_id;
  cl_uint num_platforms;
  error = clGetPlatformIDs(1, &platform_id, &num_platforms);
  if(error != CL_SUCCESS)
  {
    printf("Can't get platform\n");
    return 0;
  }

  // Create an OpenCL device  
  cl_device_id device_id;   
  cl_uint num_devices;
  if(clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &num_devices) != CL_SUCCESS)
  {
    printf("Can't get device\n");
    return 0;
  }
   
  // Create an OpenCL context
  cl_context context;
  cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties) platform_id, 0};
  context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &error);
  if(error != CL_SUCCESS)
  {
    printf("Cannot create context\n");
    return 0;
  }

  // Create a command queue
  cl_command_queue command_queue;
  command_queue = clCreateCommandQueue(context, device_id, 0, &error);
  if(error != CL_SUCCESS)
  {
    printf("Cannot create command queue\n");
    return 0;
  }  

  // Load the kernel function code into string
  FILE *fp;
  fp = fopen("heat_diffusion.cl", "r");
  if (!fp)
  {
    printf("Can't load kernel\n");
    exit(1);
  }
  size_t kernel_stringsize;
  fseek(fp,0,SEEK_END);
  kernel_stringsize = ftell(fp);
  fseek(fp,0,SEEK_SET);
  char * kernel_string = (char *)malloc((sizeof(char))*(kernel_stringsize+1));
  fread(kernel_string, 1 , kernel_stringsize , fp);
  fclose( fp );
  kernel_string[kernel_stringsize] = 0;

  // Create a program from the kernel source
  cl_program program;
  program = clCreateProgramWithSource(context, 1, (const char **) &kernel_string, (const size_t *)&kernel_stringsize , &error);
  if(error != CL_SUCCESS)
  {
    printf("Can't create program\n");
    return 0;
  }

  // Build the program
  error = clBuildProgram(program,1,&device_id, NULL, NULL, NULL);
  if(error != CL_SUCCESS)
  {
    printf("can't build the program\n");
    size_t log_size = 0;
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
    char * log = calloc(log_size,sizeof(char));
    if (log == NULL) 
    {
      printf("could not allocate memory\n");
      return 1;
    }
    clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
    printf("%s\n", log);
    free(log);
    return 1;
  }
  
  // Create the OpenCL kernel
  cl_kernel kernel;
  kernel = clCreateKernel(program, "heat_diffusion" , &error);
  if(error != CL_SUCCESS)
  {
    printf("Can't create Kernel\n");
    return 0;
  }

   // Create memory buffers on the device for each matrix 
  cl_mem told_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, length * width * sizeof(double), NULL, &error);
  cl_mem tnew_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, length * width * sizeof(double), NULL, &error);

  // Copy the temperatures matrices to respective memory buffers
  clEnqueueWriteBuffer(command_queue, told_mem_obj, CL_TRUE, 0, length * width * sizeof(double), told , 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, tnew_mem_obj, CL_TRUE, 0, length * width * sizeof(double), told , 0, NULL, NULL);

  // Set the arguments of the kernel
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &told_mem_obj);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &tnew_mem_obj);
  clSetKernelArg(kernel, 2, sizeof(int), &width);
  clSetKernelArg(kernel, 3, sizeof(int), &diffusion_constant);
    
  // Execute the OpenCL kernel on the list for n-iterations
  const size_t global[] = {length-2,width-2}; 
  const size_t local[] = {20,20};
  for(int i=0;i<num_iterations/2;++i)
  {
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &told_mem_obj);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &tnew_mem_obj);
    clEnqueueNDRangeKernel(command_queue, kernel, 2 , NULL, (const size_t *)&global, (const size_t *)&local, 0, NULL, NULL);
    
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &tnew_mem_obj);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &told_mem_obj);
    clEnqueueNDRangeKernel(command_queue, kernel, 2 , NULL, (const size_t *)&global, (const size_t *)&local, 0, NULL, NULL);
  }

  if(num_iterations%2!=0)
  {
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &told_mem_obj);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &tnew_mem_obj);
    clEnqueueNDRangeKernel(command_queue, kernel, 2 , NULL, (const size_t *)&global, NULL, 0, NULL, NULL);
    
    clEnqueueReadBuffer(command_queue, tnew_mem_obj, CL_TRUE,0, length * width * sizeof(double), temperatureold , 0, NULL, NULL);  
  }
  else
    clEnqueueReadBuffer(command_queue, told_mem_obj, CL_TRUE,0, length * width * sizeof(double), temperatureold , 0, NULL, NULL);

  // Calculate average after n iterations
  double average = 0,averageabsolute = 0, difference = 0;
  for(int i = width+1 ; i < ((length-1)*width)-1; ++i)  
    average += told[i];
  average = average/((length-2)*(width-2));

  // Calculate average absolute differences after n iterations
  for(int i = 1; i < length - 1; ++i)
  {
    for(int j = 1; j < width - 1; ++j)
    {
      difference = told[i*width + j] - average;
      averageabsolute += sqrtl(difference*difference);
    }
  } 
  averageabsolute = averageabsolute/((length-2)*(width-2));

  //Display result
  printf("%lf\n%lf\n",average,averageabsolute);

  // Clean up
  clReleaseMemObject(told_mem_obj);

  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(command_queue);
  clReleaseContext(context); 
  
  free(temperatureold);
  free(kernel_string);
  
  return 0;
}

void static inline analyse_parsing(int argc1, const char *argv1[], int * num_iterations1, float * diffusion_constant1)
{
  int index, iter=0; 
  char * args;
  while((index=getopt(argc1,(char* const*)argv1,"-n:-d:-l:-w:"))!=-1)
    {
      ++iter;
      if (index == 'n')
	    {
	      args = optarg;
        *num_iterations1 = strtol(args,(char ** restrict)argv1,10);
      }
      else if(index == 'd')
      {
        args = optarg;
        *diffusion_constant1 = strtof(args,(char ** restrict)argv1);
      }
    }
  if(iter==0)
  {
    printf("No values are passed for iterations and diffusion constant\n");	  
    exit(0);
  }
}
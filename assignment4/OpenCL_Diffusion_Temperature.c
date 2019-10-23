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
  fptr = fopen("diffusion_1_1","r");
  fseek(fptr,0,SEEK_END);
  int last_character = (ftell(fptr));
  fseek(fptr,0,SEEK_SET);
  fscanf(fptr,"%d  %d",&length,&width);
  length+=2;
  width+=2;
  /* Assign all long doubles with initial value as 0 and boundary too*/  
  //long double temperatureold[length+2][width+2],temperaturenew[length+2][width+2];
  long double * temperatureold = (long double *)malloc(sizeof(long double) * length * width);
  long double * temperaturenew = (long double *)malloc(sizeof(long double) * length * width);
  for (int i = 0; i < length*width ; ++i)
    temperatureold[i] = 0;

  int x_position,y_position;

  // Initialize the temperature values from file
  do
  {
    //fscanf(fptr,"%d %d %Lf",&x_position,&y_position,&temperatureold[(x_position+1)*width+(y_position+1)]);
    fscanf(fptr ,"%d %d ", &x_position, &y_position);
    fscanf(fptr ,"%Lf", &temperatureold[(x_position+1)*width + (y_position+1)]);
  }while(ftell(fptr)<last_character);

  for(int i=0; i<length*width ; ++i)
    temperaturenew[i] = temperatureold[i];

  fclose(fptr);  

  #define told temperatureold
  #define tnew temperaturenew
 
  for(int i=0; i< length*width; ++i)
  {
    printf("%Lf ",tnew[i]);
    if((i+1)%width==0)
    printf("\n");
  }

  cl_int error;
  
  // Create an OpenCL platform
  cl_platform_id platform_id;
  cl_uint num_platforms;
  if(clGetPlatformIDs(1, &platform_id, &num_platforms) != CL_SUCCESS)
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

  // cl_device_info deviceinfo;
  // size_t deviceinfosize;
  // char devicename[1000];
  // if(clGetDeviceInfo(device_id,deviceinfo,deviceinfosize, devicename, CL_DEVICE_NAME) != CL_SUCCESS)
  // {
  //   printf("Can't get info\n");
  //   return 0;
  // }
  // else
  // {
  //   printf("%s\n",devicename);
  // }
  
    
  // Create an OpenCL context
  cl_context context;
  cl_context_properties properties[] = 
  {
    CL_CONTEXT_PLATFORM,
    (cl_context_properties) platform_id,
    0
  };
  context = clCreateContext( properties, 1, &device_id, NULL, NULL, &error);
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
  fp = fopen("compute_temperatures_kernel.cl", "r");
  if (!fp)
  {
    printf("Can't load kernel\n");
    exit(1);
  }
  size_t kernel_stringsize;
  fseek(fp,0,SEEK_END);
  kernel_stringsize = ftell(fp);
  fseek(fp,0,SEEK_SET);
  char * kernel_string = (char *)malloc((sizeof(char))*kernel_stringsize);
  fread(kernel_string, 1 , kernel_stringsize , fp);
  fclose( fp );

  // Create a program from the kernel source
  cl_program program;
  program = clCreateProgramWithSource(context, 1, (const char **) &kernel_string, (const size_t *)&kernel_stringsize , &error);
  if(error != CL_SUCCESS)
  {
    printf("Can't create program\n");
    return 0;
  }

  // Build the program
  error = clBuildProgram(program,num_devices,(const cl_device_id *) &device_id, NULL, NULL, NULL);
  //error = clBuildProgram(program, num_devices , &device_id , NULL, NULL, NULL);
  switch (error)
  {
    case CL_INVALID_PROGRAM : printf("invalid program\n");
      break;
    case CL_INVALID_VALUE : printf("invalid value\n");
      break;
    case CL_INVALID_BINARY : printf("invalid Binary\n");
      break;
    case CL_INVALID_BUILD_OPTIONS : printf("invalid build options\n");
      break;
    case CL_INVALID_OPERATION : printf("invalid operations\n");
      break;
    case CL_COMPILER_NOT_AVAILABLE : printf("compiler not available\n");
      break;
    case CL_BUILD_PROGRAM_FAILURE : printf("Build program failure\n");
      break;
    case CL_OUT_OF_HOST_MEMORY : printf("Out of Host memory\n");
      break;
    case CL_INVALID_DEVICE : printf("Invalid device\n");
      break;
    default : printf("can't recognize %d\n", error);
      break;
  }
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
  kernel = clCreateKernel(program, "temperature_diffusion" , &error);
  if(error != CL_SUCCESS)
  {
    printf("Can't create Kernel\n");
    return 0;
  }

  // Create memory buffers on the device for each matrix 
  cl_mem told_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, length * width * sizeof(long double), NULL, &error);
  cl_mem tnew_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, length * width * sizeof(long double), NULL, &error);

  // Copy the temperatures matrices to respective memory buffers
  clEnqueueWriteBuffer(command_queue, told_mem_obj, CL_TRUE, 0, length * width * sizeof(long double), told , 0, NULL, NULL);
  clEnqueueWriteBuffer(command_queue, tnew_mem_obj, CL_TRUE, 0, length * width * sizeof(long double), tnew , 0, NULL, NULL);

  // Set the arguments of the kernel
  clSetKernelArg(kernel, 0, sizeof(cl_mem), &told_mem_obj);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), &tnew_mem_obj);
  clSetKernelArg(kernel, 2, sizeof(int), &length);
  clSetKernelArg(kernel, 3, sizeof(int), &width);
  clSetKernelArg(kernel, 4, sizeof(int), &diffusion_constant);
    
  // Execute the OpenCL kernel on the list for n-iterations
  for(int i=0;i<num_iterations;++i)
  {
    const size_t global = length-2; 
    clEnqueueNDRangeKernel(command_queue, kernel, 1 , NULL, (const size_t *)&global, NULL, 0, NULL, NULL);
    
    // Read the memory buffer tnew on the device to the local variable tnew
    clEnqueueReadBuffer(command_queue, tnew_mem_obj, CL_TRUE, 0, length * width * sizeof(long double), tnew , 0, NULL, NULL);
    clFinish(command_queue);

    //copy the i-th iterated values to local matrix which will be passed to memory buffer of type Read only
    for(int i=width+1 ; i<(length-1)*width ; ++i)
      told[i] = tnew[i];
    clEnqueueWriteBuffer(command_queue, told_mem_obj, CL_TRUE, 0, length * width * sizeof(long double), told , 0, NULL, NULL);
  }

  // Calculate average after n iterations
  long double average = 0,averageabsolute = 0, difference = 0;
  for(int i=width+1; i < (length-1)*(width); ++i)
    average += told[i];
  average = average/((length-2)*(width-2));
  
  // Calculate average absolute differences after n iterations
  for(int i=width+1; i < (length-1)*width; ++i)
    {
      difference = told[i] - average;
      averageabsolute += sqrtl(difference*difference);
    }
  averageabsolute = averageabsolute/((length-2)*(width-2));

  //Display result
  printf("%Lf\n%Lf\n",average,averageabsolute);

  // Clean up
  clReleaseMemObject(told_mem_obj);
  clReleaseMemObject(tnew_mem_obj);

  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(command_queue);
  clReleaseContext(context); 
  
  free(temperatureold);
  free(temperaturenew);
  free(kernel_string);
  
  return 0;
}

void static inline analyse_parsing(int argc1, const char *argv1[], int * num_iterations1, float * diffusion_constant1)
{
  int index, iter=0; 
  char * args;
  while((index=getopt(argc1,(char* const*)argv1,"-n:-d:"))!=-1)
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
    printf("No values is passed for iterations and diffusion constant\n");	  
    exit(0);
  }
}


// ctrl+k c/u for comm/uncomm
//   /* Calculate heat diffusion for n - iterations */
//   for(int k=0 ; k < num_iterations ; ++k)
//   {
//     for(int i=1 ; i < length - 1 ; ++i)
//     {
//       for(int j=1 ; j < width - 1 ; ++j)
//       tnew[i][j] = told[i][j] + ((long double)d * ((told[i-1][j] + told[i+1][j] + told[i][j-1] + told[i][j+1])/4 - told[i][j]));
//     }
//     for(int i=1 ; i < length - 1 ; ++i)
//     {
//       for(int j=1 ; j < width - 1 ; ++j)
//       told[i][j] = tnew[i][j];
//     }
//   }


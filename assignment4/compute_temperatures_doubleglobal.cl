__kernel void temperature_diffusion(
    __global long double * told,
    int l,
    int w,
    float dt
)
{
    int i = get_global_id(0);
    int j = get_global_id(1);
    long double newtemperature;
    newtemperature = told[(i+1) * w + (j+1)] + (dt * 
                                ((told[(i+1) * w + j] + told[(i+1) * w + (j+2)] + told[i * w + (j+1)] + told[(i+2) * w + (j+1)])/4 
                                - told[(i+1) * w + (j+1)]));
    
    barrier(CLK_GLOBAL_MEM_FENCE);
    told[(i+1) * w + (j+1)] = newtemperature;
}
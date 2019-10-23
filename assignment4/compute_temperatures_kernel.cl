__kernel void temperature_diffusion(
    __global long double * told,
    __global long double * tnew,
    int l,
    int w,
    float dt
)
{
    int i = get_global_id(0);
    for (int j = 1 ; j < w-1 ; ++j)
        tnew[(i+1) * w + j] = told[(i+1) * w + j] + (dt * 
                                ((told[(i+1) * w + (j-1)] + told[(i+1) * w + (j+1)] + told[i * w + j] + told[(i+2) * w + j])/4 
                                - told[(i+1) * w + j]));
}
__kernel void temperature_diffusion(
    __global double * told,
    __global double * tnew,
    int l,
    int w,
    float dt
)
{
    int j = get_global_id(0);
    int iter = 0;
    for (int i = j+1 ; ++iter < l-1 ; i=i+w)
        tnew[i+w] = told[i+w] + (dt * 
                                ((told[i+w-1] + told[i+w+1] + told[i+w-w] + told[i+w+w])/4 
                                - told[i+w]));
}
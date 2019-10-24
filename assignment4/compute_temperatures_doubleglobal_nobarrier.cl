__kernel void temperature_diffusion(
    __global double * told,
    __global double * tnew,
    int w,
    float dt
)
{
    int i = get_global_id(0);
    int j = get_global_id(1);
    int k = (i+1) * w + (j+1);
        // tnew[(i+1) * w + (j+1)] = told[(i+1) * w + (j+1)] + (dt * 
        //                         ((told[(i+1) * w + (j+2)] + told[(i+2) * w + (j+1)] + told[i * w + (j+1)] + told[(i+1) * w + j])/4 
        //                         - told[(i+1) * w + (j+1)]));

        tnew[k] = told[k] + dt * (((told[k+1] + told[k+w] + told[k-w] + told[k-1])/4) - told[k]);

}
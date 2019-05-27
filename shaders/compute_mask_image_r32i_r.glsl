#version 430

#local_size_define
layout(binding=0, r32i) uniform mediump iimage2D inputImage;
layout(binding=1, r8ui) uniform mediump uimage2D maskImage;
layout(binding=2, r32i) uniform mediump iimage2D resultImage;

#include compute_basic_functions.glsl

void main()
{
       ivec2 image_pixel_cord = ivec2(gl_GlobalInvocationID.xy);

       uint mask = getImagePixel(maskImage,image_pixel_cord);

       int outf = getImagePixel(inputImage,image_pixel_cord) * (int(mask) / 255);

       setImagePixel(resultImage,image_pixel_cord,outf);
       //iauscrynoaigtcnogucor6tgnr
}

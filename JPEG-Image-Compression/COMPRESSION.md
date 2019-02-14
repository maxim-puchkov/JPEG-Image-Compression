# JPEG Image Compression
### Algorithms describe every required step of encoding a JPEG image solution.


## Requirement 1. Conversion.
    1. Input image (_M_*_N_) is always converted to RGB color space
    2. RGB vectors of input image are saved in a **RGB_Matrix** (_M_*_N_)
    3. __(Important)__ Values of **RGBtoYUV_Matrix** (3x3) are given by [equation (2)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf):
        $$
        \left(
        \begin{array}{cc}
        0.299 & 0.587 & 0.114\\
        −0.299 & −0.587 & 0.886\\
        0.701 & −0.587 & −0.114
        \end{array}\right)
        $$
    4. Color space is converted from RGB to YUV if  **RGBtoYUV_Matrix** multiplies any 1x3 RGB vector:
        $$
        \left(\begin{array}{cc}
        Y \\
        U \\
        V \\
        \end{array}\right)
        
        \left(
        \begin{array}{cc}
        0.299 & 0.587 & 0.114\\
        −0.299 & −0.587 & 0.886\\
        0.701 & −0.587 & −0.114
        \end{array}\right)
        
        \left(\begin{array}{cc}
        R \\
        G \\
        B \\
        \end{array}\right)
        $$
    5. Every RGB vector of **RGB_Matrix** (_M_*_N_) is converted and stored in **YUV_Matrix** (_M_*_N_)
    6. Conversion is completed: **RGB_Matrix** stores the input colors; and 
    **YUV_Matrix** now stores YUV colors converted from the input's RGB.


## Requirement 2. Sampling. **INCOMPLETE**
    1. Step 1
    2. Step 2
    3. Step 3 ...
    
    


## Requirement 3. Partitioning. **INCOMPLETE**




## Requirement 4. 2D DCT. **INCOMPLETE**
    1. 2D DCT is applied to each 8x8 partitioned block to compute the DCT coefficients.
    2. 
    
    
    
## Requirement 5. Quantization. **INCOMPLETE**
    1. Quantization is needed to remove high frequency DCT coefficients.
    2. Every resulting matrix from DCT transformations is divided by standard JPEG quantization table (both luminance and chrominance).
    
    
_Quantization Refs:_
https://www.impulseadventure.com/photo/jpeg-quantization.html





















```javascript
if ( var ) {
    return true
}
```

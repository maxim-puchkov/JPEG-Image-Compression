# JPEG Image Compression


### Decode
1. Load image
2. Dequantize
3. Inverse Discrete Cosine Transform
4. Convert color from YUV to RGB
5. Save


### Encode
1. Load image
2. Convert color from RGB to YUV
3. Partition
4. Chroma subsample
5. Discrete Cosine Transform
6. Quantize 
7. Compress





















# JPEG Encoder

## _Step 1_ Color Conversion from RGB to YUV 
1. Input image is always RGB
2. All RGB vectors are saved stored in __RGB_Matrix__
``` 
    cvImg = cv::imread(fileName.toStdString(), IMREAD_COLOR);
```
3. [Equation (2)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) gives __3×3__  translation  __RGB_to_YUV__ :

||||
| - | - | - |
| 0.299   |  0.587  |  0.114  |
| -0.299  | -0.587  |  0.114 |
| 0.299   | -0.587  | -0.114 |

4. __YUV_Matrix__  saves product of __RGB_to_YUV__ and vectors of __RGB_Matrix__
```c++
for (col : columns) {
    for (row : rows) {
        YUV_Matrix[col, row] = RGB_to_YUV * RGB_Matrix[col, row]
    }
}
```









 
 ## _Step 2_ Partitioning Image into 8×8 blocks 
1.  
2. 
3.
4.
5.















## _Step 3_ Chroma Subsampling 4:2:0 
1.  
2. 
3.
4.
5.









## _Step 4_ DCT: Discrete Cosine Transform
1. Two dimiensional DCT is applied to each 8x8 block to compute the DCT coefficients.
2. 
3.
4.
5.










## _Step 5_ Quantization
1. ...... Quantization is needed to remove high frequency DCT coefficients.
2. ....... Every resulting matrix from DCT transformations is divided by standard JPEG quantization table (both luminance and chrominance).


_Quantization Refs:_
https://www.impulseadventure.com/photo/jpeg-quantization.html











> Color Conversion from YUV to RGB 

> IDCT: Inverse Discrete Cosine Transform

> ...


















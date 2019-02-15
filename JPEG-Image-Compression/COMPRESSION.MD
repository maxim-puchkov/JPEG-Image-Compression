# JPEG Image Compression

### Decode
1. Load image
2. Dequantization
3. Inverse Discrete Cosine Transform
4. Convert YUV to RGB
5. Save

### Encode
1. Load image
2. Convert RGB to YUV
3. Partition
4. Chroma subsample
5. Discrete Cosine Transform
6. Quantization
7. Compression





















# JPEG Encoder

## 1. Color Conversion from RGB to YUV 
1. Input image is always RGB
2. All RGB vectors are saved in __RGB_Matrix__ cvImg
``` 
    cvImg = cv::imread(fileName.toStdString(), IMREAD_COLOR);
```
3. [Eq. (4)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) gives translation matrix  __RGB_to_YUV__:

|            |            |           |
|------------|----------- |-----------|
|   0.299    |   0.587    |  0.114    |
|  −0.14713  |  −0.28886  |  0.436    |
|   0.615    |  −0.51499  | −0.10001  |

4. __YUV_Matrix__ convertedImage saves products of __RGB_to_YUV__ and all vectors of __RGB_Matrix__ cvImg
```
for (col : columns) {
    for (row : rows) {
        convertedImage[col, row] = RGB_to_YUV * cvImg[col, row]
    }
}
```









 
## 2. Partitioning Image into 8×8 blocks 
1.  
2. 
3.
4.
5.















## 3. Chroma Subsampling 4:2:0 
1.  
2. 
3.
4.
5.
>  4:2:0
>  Y + (U, V)









## 4. DCT: Discrete Cosine Transform
1. Two dimiensional DCT is applied to each 8x8 block to compute the DCT coefficients.
2. 
3.
4.
5.
> IDCT: Inverse Discrete Cosine Transform










## 5. Quantization




> Quantization is needed to remove high frequency DCT coefficients.
> Every resulting matrix from DCT transformations is divided by standard JPEG quantization table (both luminance and chrominance).

_Quantization Refs:_
https://www.impulseadventure.com/photo/jpeg-quantization.html











## • Color Conversion from YUV to RGB 
1. [Eq. (5)](http://www.cs.sfu.ca/CourseCentral/365/li/material/work/RGB-YUV.pdf) gives translation matrix __YUV_to_RGB__ :

|            |            |           |
|------------|----------- |-----------|
|    1.00    |        0   |   1.1398  |
|    1.00    |  −0.3946   | −0.58060  |
|    1.00    |  2.03211   |        0  |














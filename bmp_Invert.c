#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *image, *outputImage, *lecturas;
    image = fopen("sample.bmp","rb");          //Imagen original a transformar
    outputImage = fopen("img2_dd.bmp","wb");    //Imagen transformada
    long ancho;
    long alto;
    unsigned char r, g, b;               //Pixel
    unsigned char* ptr;

    unsigned char xx[54];
    unsigned char kernel[3][3] = { {1/9,1/9,1/9}, {1/9,1/9,1/9} };
    int cuenta = 0;
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    printf("largo img %li\n",alto);
    printf("ancho img %li\n",ancho);

    ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));

    while(!feof(image)){
      b = fgetc(image);
      g = fgetc(image);
      r = fgetc(image);


      unsigned char pixel = 0.21*r+0.72*g+0.07*b;

      ptr[cuenta] = pixel; //b
      ptr[cuenta+1] = pixel; //g
      ptr[cuenta+2] = pixel; //r

      cuenta++;
    }                                        //Grises

    cuenta = ancho;
    for (int i = 0; i < alto*ancho*3; i++) {
      fputc(ptr[i+(cuenta*3)+2], outputImage);
      fputc(ptr[i+(cuenta*3)+1], outputImage);
      fputc(ptr[i+(cuenta*3)], outputImage);
      cuenta--;
      if (cuenta == 0){
        cuenta = ancho;
      }
    }

    free(ptr);
    fclose(image);
    fclose(outputImage);
    return 0;
}

/*
for (int i = 1; i < (alto*ancho-1)*3; i++) {
  pixel = 0;
  for (int n = 0; n < 3; n++) {
    for (int m = 0; m < 3; m++) {
      pixel = pixel + (i+(m-1),j+(n-1))*kernel[m,n]);
    }
  }
  // Asigna Pixel
}
*/

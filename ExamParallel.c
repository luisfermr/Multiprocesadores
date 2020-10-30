#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 2

struct my_pixel
{
  unsigned char r, g, b;
};

double t1, t2, totalT;

void procImg(char *input_img, char *output_img);

int main()
{
  omp_set_num_threads(NUM_THREADS);

  double t1, t2;
  t1 = omp_get_wtime();
#pragma omp parallel
  {
#pragma omp sections
    {
#pragma omp section
      {
        (void)procImg("f1.bmp", "f1_dd.bmp");
        printf("Termino la 1\n");
      }
#pragma omp section
      {
        (void)procImg("f2.bmp", "f2_dd.bmp");
        printf("Termino la 2\n");
      }
#pragma omp section
      {
        (void)procImg("f3.bmp", "f3_dd.bmp");
        printf("Termino la 3\n");
      }
#pragma omp section
      {
        (void)procImg("f4.bmp", "f4_dd.bmp");
        printf("Termino la 4\n");
      }
#pragma omp section
      {
        (void)procImg("f5.bmp", "f5_dd.bmp");
        printf("Termino la 5\n");
      }
#pragma omp section
      {
        (void)procImg("f6.bmp", "f6_dd.bmp");
        printf("Termino la 6\n");
      }
#pragma omp section
      {
        (void)procImg("f7.bmp", "f7_dd.bmp");
        printf("Termino la 7\n");
      }
#pragma omp section
      {
        (void)procImg("f8.bmp", "f8_dd.bmp");
        printf("Termino la 8\n");
      }
#pragma omp section
      {
        (void)procImg("f9.bmp", "f9_dd.bmp");
        printf("Termino la 9\n");
      }
#pragma omp section
      {
        (void)procImg("f10.bmp", "f10_dd.bmp");
        printf("Termino la 10\n");
      }
    }
  }
  t2 = omp_get_wtime();
  printf("Tiempo: %.3f \n", (t2 - t1));
  return 0;
}

void procImg(char *input_img, char *output_img)
{
  int kernel_size = 21;
  FILE *image, *outputImage, *lecturas;
  long ancho;
  long alto;
  long tamano;
  int comp = 0;
  image = fopen(input_img, "rb");        //Imagen original a transformar
  outputImage = fopen(output_img, "wb"); //Imagen transformada
  unsigned char r, g, b;                 //Pixel
  double kernel[kernel_size * kernel_size];
  for (int i = 0; i < kernel_size; i++)
  {
    for (int j = 0; j < kernel_size; j++)
    {
      kernel[(i * kernel_size) + j] = 1.0 / (double)(kernel_size * kernel_size);
    }
  }
  int m = kernel_size - 1;
  int n = m / 2;

  unsigned char xx[54];
  for (int i = 0; i < 54; i++)
  {
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage); //Copia cabecera a nueva imagen
  }
  ancho = (long)xx[21] * (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
  alto = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];
  printf("largo img %li\n", alto);
  printf("ancho img %li\n", ancho);

  while (((ancho * 3) + comp) % 4 != 0)
  {
    comp++;
  }

  unsigned char *arr_in = (unsigned char *)malloc(((ancho * 3) + comp) * alto * sizeof(unsigned char));
  unsigned char *arr_out = (unsigned char *)malloc(((ancho * 3) + comp) * alto * sizeof(unsigned char));
  struct my_pixel *my_arr = (struct my_pixel *)malloc(ancho * alto * sizeof(struct my_pixel));
  struct my_pixel *out_arr = (struct my_pixel *)malloc(ancho * alto * sizeof(struct my_pixel));
  struct my_pixel *image_expanded = (struct my_pixel *)malloc((ancho + m) * (alto + m) * sizeof(struct my_pixel));

  int j = 0;
  struct my_pixel pixel_null;
  pixel_null.b = 0;
  pixel_null.g = 0;
  pixel_null.r = 0;

  for (int i = 0; i < ((ancho * 3) + comp) * alto; i++)
  {
    *(arr_in + i) = fgetc(image);
    *(arr_out + i) = *(arr_in + i);
  }
  //Imagen a blanco y negro
  int index = 0;
  int count = 0;

#pragma omp parallel
{
  #pragma omp for ordered schedule(static, 1)
    for (int i = 0; i < ((ancho * 3) + comp) * alto; i += 3)
    {
      b = *(arr_in + i);
      g = *(arr_in + i + 1);
      r = *(arr_in + i + 2);
      unsigned char pixel = 0.21 * r + 0.72 * g + 0.07 * b;
      my_arr[index].b = pixel;
      my_arr[index].g = pixel;
      my_arr[index].r = pixel;
      index++;

      count += 3;
      if (count == ancho * 3)
      {
        i += comp;
        count = 0;
      }
    }


  #pragma omp for ordered schedule(static, 1)
    //Efecto Espejo
    for (int i = 0; i < alto - 1; i++)
    {
      for (int j = 0; j < ancho - 1; j++)
      {
        out_arr[(i * ancho) + j].b = my_arr[(i * ancho) + ancho - j].b;
        out_arr[(i * ancho) + j].g = my_arr[(i * ancho) + ancho - j].g;
        out_arr[(i * ancho) + j].r = my_arr[(i * ancho) + ancho - j].r;
      }
    }
  
  #pragma omp for ordered schedule(static, 1)
    //Efecto Blur
    for (int i = 0; i < alto + m; i++)
    {
      for (int j = 0; j < ancho + m; j++)
      {
        image_expanded[(i * (ancho + m)) + j] = pixel_null;
      }
    }

    for (int i = 0; i < alto; i++)
    {
      for (int j = 0; j < ancho; j++)
      {
        image_expanded[((i + n) * (ancho + m)) + j + n] = out_arr[(i * ancho) + j];
      }
    }

    double suma_b = 0;
    double suma_g = 0;
    double suma_r = 0;
    for (int i = 0; i < alto; i++)
    {
      for (int j = 0; j < ancho; j++)
      {
        for (int k = 0; k < kernel_size; k++)
        {
          for (int l = 0; l < kernel_size; l++)
          {
            suma_b += (double)(image_expanded[((i + k) * (ancho + m)) + j + l].b) * kernel[(k * kernel_size) + l];
            suma_g += (double)(image_expanded[((i + k) * (ancho + m)) + j + l].g) * kernel[(k * kernel_size) + l];
            suma_r += (double)(image_expanded[((i + k) * (ancho + m)) + j + l].r) * kernel[(k * kernel_size) + l];
          }
        }
        out_arr[(i * ancho) + j].b = (unsigned char)(suma_b);
        out_arr[(i * ancho) + j].g = (unsigned char)(suma_b);
        out_arr[(i * ancho) + j].r = (unsigned char)(suma_b);
        suma_b = 0;
        suma_g = 0;
        suma_r = 0;
      }
    }
  }

  index = 0;
  count = 0;
  for (int i = 0; i < ((ancho * 3) + comp) * alto; i += 3)
  {
    *(arr_out + i) = out_arr[index].b;
    *(arr_out + i + 1) = out_arr[index].g;
    *(arr_out + i + 2) = out_arr[index].r;
    index++;

    count += 3;
    if (count == ancho * 3)
    {
      i += comp;
      count = 0;
    }
  }

  for (int i = 0; i < ((ancho * 3) + comp) * alto; i++)
  {
    fputc(*(arr_out + i), outputImage);
  }

  free(arr_in);
  free(arr_out);
  free(my_arr);
  free(out_arr);
  free(image_expanded);
  fclose(image);
  fclose(outputImage);
}

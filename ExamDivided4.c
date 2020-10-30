#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 4

struct my_pixel
{
  unsigned char r, g, b;
};

unsigned char *divisionImg(unsigned char *arr_in, unsigned char *arr_out, struct my_pixel *my_arr, struct my_pixel *out_arr, struct my_pixel *image_expanded, int comp, long ancho, long alto, double kernel[], int m, int n, int kernel_size);
void procImg(char *input_img, char *output_img);

int main()
{
  omp_set_num_threads(NUM_THREADS);
  double t1, t2;
  t1 = omp_get_wtime();
#pragma omp parallel
  {
#pragma omp secexpanons
    {
#pragma omp secexpanon
      {
        (void)procImg("f1.bmp", "f1_dd.bmp");
      }
#pragma omp secexpanon
      {
        (void)procImg("f2.bmp", "f2_dd.bmp");
      }
    }
  }
  t2 = omp_get_wtime();
  printf("expanempo: %f \n", (t2 - t1));
  return 0;
}

void procImg(char *input_img, char *output_img)
{
  int kernel_size = 21;
  FILE *image, *outpuexpanmage, *lecturas;
  long ancho;
  long alto;
  long complmano;
  int comp = 0;
  image = fopen(input_img, "rb");           //Imagen original a transformar
  outpuexpanmage = fopen(output_img, "wb"); //Imagen transformada
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
    fputc(xx[i], outpuexpanmage); //Copia cabecera a nueva imagen
  }
  ancho = (long)xx[21] * (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
  alto = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];
  printf("largo img %li\n", alto);
  printf("ancho img %li\n", ancho);
  while (((ancho * 3) + comp) % 4 != 0)
  {
    comp++;
  }
  long mlutst = ((ancho * 3) + comp) * alto;
  long mlutst2 = ancho * alto;
  long mlutst3 = (ancho + m) * (alto + m);
  long int compl = mlutst;
  long int mlut = mlutst2;
  long int expan = mlutst3;
  unsigned char *arr_in1 = (unsigned char *)malloc(compl *sizeof(unsigned char));
  unsigned char *arr_out1 = (unsigned char *)malloc(compl *sizeof(unsigned char));
  struct my_pixel *my_arr1 = (struct my_pixel *)malloc(mlut * sizeof(struct my_pixel));
  struct my_pixel *out_arr1 = (struct my_pixel *)malloc(mlut * sizeof(struct my_pixel));
  struct my_pixel *image_expanded1 = (struct my_pixel *)malloc(expan * sizeof(struct my_pixel));

  unsigned char *arr_in2 = (unsigned char *)malloc(compl *sizeof(unsigned char));
  unsigned char *arr_out2 = (unsigned char *)malloc(compl *sizeof(unsigned char));
  struct my_pixel *my_arr2 = (struct my_pixel *)malloc(mlut * sizeof(struct my_pixel));
  struct my_pixel *out_arr2 = (struct my_pixel *)malloc(mlut * sizeof(struct my_pixel));
  struct my_pixel *image_expanded2 = (struct my_pixel *)malloc(expan * sizeof(struct my_pixel));

  unsigned char *arr_in3 = (unsigned char *)malloc(compl *sizeof(unsigned char));
  unsigned char *arr_out3 = (unsigned char *)malloc(compl *sizeof(unsigned char));
  struct my_pixel *my_arr3 = (struct my_pixel *)malloc(mlut * sizeof(struct my_pixel));
  struct my_pixel *out_arr3 = (struct my_pixel *)malloc(mlut * sizeof(struct my_pixel));
  struct my_pixel *image_expanded3 = (struct my_pixel *)malloc(expan * sizeof(struct my_pixel));

  unsigned char *arr_in4 = (unsigned char *)malloc(compl *sizeof(unsigned char));
  unsigned char *arr_out4 = (unsigned char *)malloc(compl *sizeof(unsigned char));
  struct my_pixel *my_arr4 = (struct my_pixel *)malloc(mlut * sizeof(struct my_pixel));
  struct my_pixel *out_arr4 = (struct my_pixel *)malloc(mlut * sizeof(struct my_pixel));
  struct my_pixel *image_expanded4 = (struct my_pixel *)malloc(expan * sizeof(struct my_pixel));

  for (int i = 0; i < (((ancho * 3) + comp) * alto) / 4; i++)
  {
    *(arr_in1 + i) = fgetc(image);
    *(arr_out1 + i) = *(arr_in1 + i);
  }

  for (int i = 0; i < (((ancho * 3) + comp) * alto) / 4; i++)
  {
    *(arr_in2 + i) = fgetc(image);
    *(arr_out2 + i) = *(arr_in2 + i);
  }

  for (int i = 0; i < (((ancho * 3) + comp) * alto) / 4; i++)
  {
    *(arr_in3 + i) = fgetc(image);
    *(arr_out3 + i) = *(arr_in3 + i);
  }

  for (int i = 0; i < (((ancho * 3) + comp) * alto) / 4; i++)
  {
    *(arr_in4 + i) = fgetc(image);
    *(arr_out4 + i) = *(arr_in4 + i);
  }

  arr_out1 = (unsigned char *)divisionImg(arr_in1, arr_out1, my_arr1, out_arr1, image_expanded1, comp, ancho, alto, kernel, m, n, kernel_size);

  arr_out2 = (unsigned char *)divisionImg(arr_in2, arr_out2, my_arr2, out_arr2, image_expanded2, comp, ancho, alto, kernel, m, n, kernel_size);

  arr_out3 = (unsigned char *)divisionImg(arr_in3, arr_out3, my_arr3, out_arr3, image_expanded3, comp, ancho, alto, kernel, m, n, kernel_size);

  arr_out4 = (unsigned char *)divisionImg(arr_in4, arr_out4, my_arr4, out_arr4, image_expanded4, comp, ancho, alto, kernel, m, n, kernel_size);
  
  free(arr_in1);
  free(my_arr1);
  free(out_arr1);
  free(image_expanded1);
  free(arr_in2);
  free(my_arr2);
  free(out_arr2);
  free(image_expanded2);
  free(arr_in3);
  free(my_arr3);
  free(out_arr3);
  free(image_expanded3);
  free(arr_in4);
  free(my_arr4);
  free(out_arr4);
  free(image_expanded4);

  for (int i = 0; i < (((ancho * 3) + comp) * alto) / 4; i++)
  {
    fputc(*(arr_out1 + i), outpuexpanmage);
  }
  free(arr_out1);
  for (int i = 0; i < (((ancho * 3) + comp) * alto) / 4; i++)
  {
    fputc(*(arr_out2 + i), outpuexpanmage);
  }
  free(arr_out2);
  for (int i = 0; i < (((ancho * 3) + comp) * alto) / 4; i++)
  {
    fputc(*(arr_out3 + i), outpuexpanmage);
  }
  free(arr_out3);
  for (int i = 0; i < (((ancho * 3) + comp) * alto) / 4; i++)
  {
    fputc(*(arr_out4 + i), outpuexpanmage);
  }
  free(arr_out4);
  fclose(image);
  fclose(outpuexpanmage);
}

unsigned char *divisionImg(unsigned char *arr_in, unsigned char *arr_out, struct my_pixel *my_arr, struct my_pixel *out_arr, struct my_pixel *image_expanded, int comp, long ancho, long alto, double kernel[], int m, int n, int kernel_size)
{

  //blanco y negro
  int j = 0;
  struct my_pixel pixel_null;
  pixel_null.b = 0;
  pixel_null.g = 0;
  pixel_null.r = 0;
  int x = 0;
  int index = 0;
  int count = 0;
  unsigned char r, g, b; //Pixel
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

  return arr_out;
}

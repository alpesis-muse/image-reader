#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct bitmap
{
    unsigned int width, height;
    unsigned char *pixels;
} bitmap;

struct bitmap* bmp;

void FreeBmp()
{
    if (bmp && bmp->pixels)
    {
        bmp->width = 0;
        bmp->height = 0;
        free(bmp->pixels);
        free(bmp);
        bmp = NULL;
    }
}
bool LoadBmp(const char *filepath)
{
    bmp = NULL;
    FILE *f = fopen(filepath, "rb");
    if (f)
    {
        bmp = malloc(sizeof(bitmap));
        bmp->width = 0;
        bmp->height = 0;
        bmp->pixels = NULL;
        unsigned char info[54] = {0};
        fread(info, sizeof(unsigned char), 54, f);
        bmp->width = *(unsigned int *)&info[18];
        bmp->height = *(unsigned int *)&info[22];
        unsigned int size = ((((bmp->width * bmp->height) + 31) & ~31) / 8) * bmp->height;
        bmp->pixels = malloc(size);
        fread(bmp->pixels, sizeof(unsigned char), size, f);
        fclose(f);
        for(int i = 0; i < size; i += 3)
        {
            unsigned char tmp = bmp->pixels[i];
            bmp->pixels[i] = bmp->pixels[i + 2];
            bmp->pixels[i + 2] = tmp;
        }
        return true;
    }
    return false;
}

int main()
{
    LoadBmp("/home/kelly/Studio/westwell/DeepBrain/quant_caffe/py-R-FCN/data/demo/00001.bmp");
    int width_step = bmp->width * 3 + (bmp->width % 4);
    printf("%d %d %d\n", bmp->height, bmp->width, width_step);
    for (int i = 0; i < bmp->height; ++i)
    {
        unsigned char * ptr = (unsigned char*)(bmp->pixels + i * width_step);
        for (int j = 0; j < bmp->width; ++j)
        {
            printf("%d %d %d\n", ptr[j], ptr[j+1], ptr[j+2]);
        }
    }

    FreeBmp();
}

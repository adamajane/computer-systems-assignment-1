// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <string.h>

// Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
        output_image[x][y][c] = 255 - input_image[x][y][c];
      }
    }
  }
}

// This function converts the image to black and white (applies the binary threshold)
void convert_blackwhite(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  int res = 0;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if ((input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3 > 90)
      {
        blackwhite_image[x][y][0] = 255;
        blackwhite_image[x][y][1] = 255;
        blackwhite_image[x][y][2] = 255;
      }
      else
      {
        blackwhite_image[x][y][0] = 0;
        blackwhite_image[x][y][1] = 0;
        blackwhite_image[x][y][2] = 0;
      }
    }
  }
}

void copyImage(unsigned char orgImg[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char cpyImg[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      cpyImg[x][y][0] = orgImg[x][y][0];
      cpyImg[x][y][1] = orgImg[x][y][1];
      cpyImg[x][y][2] = orgImg[x][y][2];
    }
  }
}


// This function erodes the black and white image using binary erosion
// Returns 1 if a pixel was changed as part of the erosion, 0 otherwise (if image is already black)
int erode_image(unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  int pixelChange = 0;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      // printf("x = %d, y = %d, color = %d \n", x, y, blackwhite_image[x][y][0]);
      int yLow = y - 1;
      if (yLow < 0) yLow = 0;
      int yHigh = y + 1;
      if (yHigh > BMP_HEIGTH) yHigh = BMP_HEIGTH;

      int xLow = x - 1;
      if (xLow < 0) xLow = 0;
      int xHigh = x + 1;
      if (xHigh > BMP_WIDTH) xHigh = BMP_WIDTH;
      // printf("  pixel above color: %d \n", blackwhite_image[x][yLow][0]);
      // printf("  pixel below color: %d \n", blackwhite_image[x][yHigh][0]);
      // printf("  pixel left  color: %d \n", blackwhite_image[xLow][y][0]);
      // printf("  pixel right color: %d \n", blackwhite_image[xHigh][y][0]);

      // Change the color of the pixels according to the structuring element
      if (blackwhite_image[x][yLow][0] == 0 && blackwhite_image[x][yLow][1] == 0 && blackwhite_image[x][yLow][2] == 0)
      {
        eroded_image[x][y][0] = 0;
        eroded_image[x][y][1] = 0;
        eroded_image[x][y][2] = 0;
        // printf("  setting black 1");
      }
      else if (blackwhite_image[x][yHigh][0] == 0 && blackwhite_image[x][yHigh][1] == 0 && blackwhite_image[x][yHigh][2] == 0)
      {
        eroded_image[x][y][0] = 0;
        eroded_image[x][y][1] = 0;
        eroded_image[x][y][2] = 0;
        // printf("  setting black 2");
      }
      else if (blackwhite_image[xLow][y][0] == 0 && blackwhite_image[xLow][y][1] == 0 && blackwhite_image[xLow][y][2] == 0)
      {
        eroded_image[x][y][0] = 0;
        eroded_image[x][y][1] = 0;
        eroded_image[x][y][2] = 0;
        // printf("  setting black 3");
      }
      else if (blackwhite_image[xHigh][y][0] == 0 && blackwhite_image[xHigh][y][1] == 0 && blackwhite_image[xHigh][y][2] == 0)
      {
        eroded_image[x][y][0] = 0;
        eroded_image[x][y][1] = 0;
        eroded_image[x][y][2] = 0;
        // printf("  setting black 4");
      }
      if(eroded_image[x][y][0] != blackwhite_image[x][y][0]) pixelChange = 1;
    }
  }
  return pixelChange;
  // The following for-loops converts the image back to black and white (where cells are white)

  // for (int x = 0; x < BMP_WIDTH; x++)
  // {
  //   for (int y = 0; y < BMP_HEIGTH; y++)
  //   {
  //     if (eroded_image[x][y][0] == 0 && eroded_image[x][y][1] == 0 && eroded_image[x][y][2] == 0)
  //     {
  //       eroded_image[x][y][0] = 255;
  //       eroded_image[x][y][1] = 255;
  //       eroded_image[x][y][2] = 255;
  //     }
  //     else if (eroded_image[x][y][0] == 255 && eroded_image[x][y][1] == 255 && eroded_image[x][y][2] == 255)
  //     {
  //       eroded_image[x][y][0] = 0;
  //       eroded_image[x][y][1] = 0;
  //       eroded_image[x][y][2] = 0;
  //     }
  //   }
  // }
}

// Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

// Main function
int main(int argc, char **argv)
{
  // argc counts how may arguments are passed
  // argv[0] is a string with the name of the program
  // argv[1] is the first command line argument (input image)
  // argv[2] is the second command line argument (output image)

  // Checking that 2 arguments are passed
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("Example program - 02132 - A1\n");

  // Load image from file
  read_bitmap(argv[1], input_image);

  // Run inversion
  //invert(input_image, output_image);

  // Run blackwhite conversion
  convert_blackwhite(input_image, blackwhite_image);

  char bwImgName[100];
  sprintf(bwImgName,"black_white_%s", argv[2]);
  write_bitmap(blackwhite_image, bwImgName);

  int pixelChange = 0;
  int imgIndex = 0;
  copyImage(blackwhite_image, eroded_image);
  do {
    // Runs the erosion
    pixelChange = erode_image(blackwhite_image, eroded_image);
    char imgName[strlen(argv[2])+10];
    sprintf(imgName, "tmp_%d_%s", imgIndex, argv[2]);
    write_bitmap(eroded_image, imgName);
    copyImage(eroded_image, blackwhite_image);
    imgIndex += 1;
    // detection step (keep track of location of detected cells)
  } while (pixelChange == 1 /*&& imgIndex < 10*/);

  // generate output image

  // save output image and print results
  

  // Save image of each erosion step to file
  write_bitmap(eroded_image, argv[2]);

  printf("Done!\n");
  return 0;
}

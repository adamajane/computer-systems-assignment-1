// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

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
void convert_blackwhite(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  int res = 0;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if ((output_image[x][y][0] + output_image[x][y][1] + output_image[x][y][2]) / 3 > 130)
      {
        blackwhite_image[x][y][0] = 0;
        blackwhite_image[x][y][1] = 0;
        blackwhite_image[x][y][2] = 0;
      }
      else
      {
        blackwhite_image[x][y][0] = 255;
        blackwhite_image[x][y][1] = 255;
        blackwhite_image[x][y][2] = 255;
      }
    }
  }
}

// This function erodes the black and white image using binary erosion
void erode_image(unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{

  int white_count;
  int black_count;
  int other_count;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (blackwhite_image[x][y][0] == 0 && blackwhite_image[x][y][1] == 0 && blackwhite_image[x][y][2] == 0)
      {
        black_count++;
      }
      else if (blackwhite_image[x][y][0] == 255 && blackwhite_image[x][y][1] == 255 && blackwhite_image[x][y][2] == 255)
      {
        white_count++;
      }
      else
      {
        other_count++;
      }
    }
  }

  int pixel_count = 0;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      // TODO: The erosion of the image should stop when all the pixels are black (The image is white and black)
      // while (eroded_image[x][y][0] == 255 && eroded_image[x][y][1] == 255 && eroded_image[x][y][2] == 255)
      // {
      // Change the color of the pixels according to the structuring element
      if (blackwhite_image[x][y - 1][0] == 0 && blackwhite_image[x][y - 1][1] == 0 && blackwhite_image[x][y - 1][2] == 0)
      {
        eroded_image[x][y][0] = 255;
        eroded_image[x][y][1] = 255;
        eroded_image[x][y][2] = 255;
      }
      else if (blackwhite_image[x][y + 1][0] == 0 && blackwhite_image[x][y + 1][1] == 0 && blackwhite_image[x][y + 1][2] == 0)
      {
        eroded_image[x][y][0] = 255;
        eroded_image[x][y][1] = 255;
        eroded_image[x][y][2] = 255;
      }
      else if (blackwhite_image[x - 1][y][0] == 0 && blackwhite_image[x - 1][y][1] == 0 && blackwhite_image[x - 1][y][2] == 0)
      {
        eroded_image[x][y][0] = 255;
        eroded_image[x][y][1] = 255;
        eroded_image[x][y][2] = 255;
      }
      else if (blackwhite_image[x + 1][y][0] == 0 && blackwhite_image[x + 1][y][1] == 0 && blackwhite_image[x + 1][y][2] == 0)
      {
        eroded_image[x][y][0] = 255;
        eroded_image[x][y][1] = 255;
        eroded_image[x][y][2] = 255;
      }
      pixel_count++;
      // The following for-loops converts the image back to black and white (where cells are white)
      if (eroded_image[x][y][0] == 0 && eroded_image[x][y][1] == 0 && eroded_image[x][y][2] == 0)
      {
        eroded_image[x][y][0] = 255;
        eroded_image[x][y][1] = 255;
        eroded_image[x][y][2] = 255;
      }
      else if (eroded_image[x][y][0] == 255 && eroded_image[x][y][1] == 255 && eroded_image[x][y][2] == 255)
      {
        eroded_image[x][y][0] = 0;
        eroded_image[x][y][1] = 0;
        eroded_image[x][y][2] = 0;
      }
      //}
    }
  }
  printf("Pixels: %d\n", pixel_count);
  printf("White count: %d\n", white_count);
  printf("Black count: %d\n", black_count);
  printf("Other count: %d\n", other_count);
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
  invert(input_image, output_image);

  // Run blackwhite conversion
  convert_blackwhite(output_image, blackwhite_image);

  // TODO: Should the black/white image
  // Save the black/white image to file
  // write_bitmap(blackwhite_image, argv[2]);

  // Runs the erosion
  erode_image(blackwhite_image, eroded_image);

  // Save image of each erosion step to file
  write_bitmap(eroded_image, argv[2]);

  printf("Done!\n");
  return 0;
}

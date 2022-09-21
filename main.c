// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <string.h>

unsigned char detected_cells[BMP_WIDTH][BMP_HEIGTH];
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];


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
// this function checks all the pixels in the exclusion frame to see if there
// is any white pixels. If there is, it should return 1. If there isn't, it 
// should return 0.


void generateOutputImg ( unsigned char redcross_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS] ){
  for (int x = 0; x < BMP_WIDTH; x++){
    for (int y = 0; y < BMP_HEIGTH; y++){
        if( detected_cells[x][y] == 1) {
          for( int k = -10; k < 10; k++){
            for ( int l = -2; l < 2; l++){
            // makes pixels in cross red
            input_image[x+k][y+l][0] = 255; 
            input_image[x+k][y+l][1] = 0; 
            input_image[x+k][y+l][2] = 0; 

            // makes pixels in cross red
            input_image[x+l][y+k][0] = 255; 
            input_image[x+l][y+k][1] = 0; 
            input_image[x+l][y+k][2] = 0; 
            }
          }
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




// Declaring the arrays to store the images (unsigned char = unsigned 8 bit)

unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char eroded_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char redcross_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];



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

  detected_cells[1][588] = 1;
detected_cells[75][296] = 1;
detected_cells[99][731] = 1;
detected_cells[157][407] = 1;
detected_cells[243][445] = 1;
detected_cells[387][450] = 1;
detected_cells[510][499] = 1;
detected_cells[683][348] = 1;
detected_cells[817][574] = 1;
detected_cells[931][603] = 1;

  // Load image from file
  read_bitmap(argv[1], input_image);

  generateOutputImg( input_image);

  // Run blackwhite conversion
  //convert_blackwhite(output_image, blackwhite_image);

  // Save image of each erosion step to file
  write_bitmap(input_image, argv[2]);

    for ( int i = 0; i < BMP_WIDTH; i++){
    for ( int j = 0; j < BMP_HEIGTH; j++){
      if(detected_cells[i][j] == 1){
      printf("Cell found at: (%d,%d) \n", i,j);
      }
    }
  }

  

  printf("Done!\n");
  return 0;
}

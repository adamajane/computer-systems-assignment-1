// To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
// To run (linux/mac): ./main.out example.bmp example_inv.bmp

// To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
// To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include <string.h>
#include <time.h>

unsigned char detected_cells[BMP_WIDTH][BMP_HEIGTH];

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
int checkExclusionFrame(unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y)
{
  // initiating for-loop, to go through 10 pixels on all 4 sides of the exclusionframe(square)
  int frame_size = 10;
  int initial_posX = x - (frame_size / 2);
  int initial_posY = y - (frame_size / 2);
  for (int exclusion_pixel = 0; exclusion_pixel < frame_size; exclusion_pixel++)
  {
    int observed_x = initial_posX + exclusion_pixel;
    int observed_y = initial_posY + exclusion_pixel;
    if (observed_x < 0)
    {
      observed_x = 0;
    }
    if (observed_y < 0)
    {
      observed_y = 0;
    }
    if (observed_x > BMP_WIDTH - 1)
    {
      observed_x = BMP_WIDTH - 1;
    }
    if (observed_y > BMP_HEIGTH - 1)
    {
      observed_y = BMP_HEIGTH - 1;
    }


    // if statement checks for white pixels in the UPPER side of square
      if(initial_posY >= 0 ){
      if (blackwhite_image[observed_x][(initial_posY)][0] == 255 && blackwhite_image[observed_x][(initial_posY)][1] == 255 && blackwhite_image[observed_x][(initial_posY)][2] == 255)
      {
        return 1;
        break;
      }
      }
    // if statement checks for white pixels in the LOWER side of square
    if(initial_posY + frame_size-1 <= BMP_HEIGTH-1){
     if (blackwhite_image[observed_x][((initial_posY) + (frame_size - 1))][0] == 255 && blackwhite_image[observed_x][((initial_posY) + (frame_size - 1))][1] == 255 && blackwhite_image[observed_x][((initial_posY) + (frame_size - 1))][2] == 255)
    {
      return 1;
      break;
    }
    }
    // if statement checks for white pixels in the LEFT side of square
    if(initial_posX >= 0){
     if (blackwhite_image[(initial_posX)][observed_y][0] == 255 && blackwhite_image[(initial_posX)][observed_y][1] == 255 && blackwhite_image[(initial_posX)][observed_y][2] == 255)
    {
      return 1;
      break;
    }
    }
    // if statement checks for white pixels in the RIGHT side of square
    if(initial_posX + frame_size-1 <= BMP_WIDTH-1){
     if (blackwhite_image[((initial_posX) + (frame_size - 1))][observed_y][0] == 255 && blackwhite_image[((initial_posX) + (frame_size - 1))][observed_y][1] == 255 && blackwhite_image[((initial_posX) + (frame_size - 1))][observed_y][2] == 255)
    {
      return 1;
      break;
    }
    }
  }
  return 0;
}

// for every pixel in the image, we check the 12x12 pixel area around it. The
// pixel we are looking at in this 12by12 area, is located at (7,7).
// if there is a white pixel in this area, we should proceed to check if the
// exclusionframe contains any white pixels, using the "checkExclusionFrame()"- function.
// If it doesn't contain a white pixel, we will add 1 to cell_count, color the entire
// area black, and move on to the next pixel.
int detectCell(unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  int cell_count = 0;
  int detect_x_min;
  int detect_y_min;
  int detect_x_max = 4;
  int detect_y_max = 4;
  int flag = 0;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      // the next nested for-loop go through the 8x8 pixel area and checks for white pixel
      for (detect_x_min = -4; detect_x_min < detect_x_max; detect_x_min++)
      {

        for (detect_y_min = -4; detect_y_min < detect_y_max; detect_y_min++)
        {
          if (x + detect_x_min < 0)
          {
            while (x + detect_x_min != 0)
            {
              detect_x_min++;
            }
          }
          if (x + detect_x_min > BMP_WIDTH - 1)
          {
            break;
          }
          if (y + detect_y_min < 0)
          {
            while (y + detect_y_min != 0)
            {
              detect_y_min++;
            }
          }
          if (y + detect_y_min > BMP_HEIGTH - 1)
          {
            break;
          }

          if (blackwhite_image[x + detect_x_min][y + detect_y_min][0] == 255 && blackwhite_image[x + detect_x_min][y + detect_y_min][1] == 255 && blackwhite_image[x + detect_x_min][y + detect_y_min][2] == 255)
          {

            if (checkExclusionFrame(blackwhite_image, x, y) == 0)
            {
              // if we have found a white pixel and the exclusionframe is clear aswell
              // we increment the cell count by 1. We set the flag to 1
              // to let us know that we should break out of the nested detection
              // for-loop after coloring all the pixels black
              cell_count++;
              detected_cells[x][y] = 1;
              flag = 1;
              // after we have incremented the cell count, we color all the pixels
              // in the detection area black and break out of

              for (int black_x = -4; black_x < detect_x_max; black_x++)
              {

                for (int black_y = -4; black_y < detect_y_max; black_y++)
                {
                  if (x + black_x < 0)
                  {
                    while (x + black_x != 0)
                    {
                      black_x++;
                    }
                  }
                  if (x + black_x > BMP_WIDTH - 1)
                  {
                    break;
                  }
                  if (y + black_y < 0)
                  {
                    while (y + black_y != 0)
                    {
                      black_y++;
                    }
                  }
                  if (y + black_y > BMP_HEIGTH - 1)
                  {
                    break;
                  }
                  blackwhite_image[x + black_x][y + black_y][0] = 0;
                  blackwhite_image[x + black_x][y + black_y][1] = 0;
                  blackwhite_image[x + black_x][y + black_y][2] = 0;
                }
              }

              break;
            }
          }
        }
        if (flag == 1)
        {
          break;
        }
      }
    }
  }
  // finally prints the total count of cells

  return cell_count;
}

void generateOutputImg(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (detected_cells[x][y] == 1)
      {
        for (int k = -10; k < 10; k++)
        {
          for (int l = -2; l < 2; l++)
          {
            // makes pixels in cross red
            input_image[x + k][y + l][0] = 255;
            input_image[x + k][y + l][1] = 0;
            input_image[x + k][y + l][2] = 0;

            // makes pixels in cross red
            input_image[x + l][y + k][0] = 255;
            input_image[x + l][y + k][1] = 0;
            input_image[x + l][y + k][2] = 0;
          }
        }
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

// This function makes a copy of our image so that we can always apply our erosion onto a new array
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
      int yLow = y - 1;
      if (yLow < 0)
        yLow = 0;
      int yHigh = y + 1;
      if (yHigh > BMP_HEIGTH)
        yHigh = BMP_HEIGTH;

      int xLow = x - 1;
      if (xLow < 0)
        xLow = 0;
      int xHigh = x + 1;
      if (xHigh > BMP_WIDTH)
        xHigh = BMP_WIDTH;

      // Change the color of the pixels according to the structuring element
      if (blackwhite_image[x][yLow][0] == 0 && blackwhite_image[x][yLow][1] == 0 && blackwhite_image[x][yLow][2] == 0)
      {
        eroded_image[x][y][0] = 0;
        eroded_image[x][y][1] = 0;
        eroded_image[x][y][2] = 0;
      }
      else if (blackwhite_image[x][yHigh][0] == 0 && blackwhite_image[x][yHigh][1] == 0 && blackwhite_image[x][yHigh][2] == 0)
      {
        eroded_image[x][y][0] = 0;
        eroded_image[x][y][1] = 0;
        eroded_image[x][y][2] = 0;
      }
      else if (blackwhite_image[xLow][y][0] == 0 && blackwhite_image[xLow][y][1] == 0 && blackwhite_image[xLow][y][2] == 0)
      {
        eroded_image[x][y][0] = 0;
        eroded_image[x][y][1] = 0;
        eroded_image[x][y][2] = 0;
      }
      else if (blackwhite_image[xHigh][y][0] == 0 && blackwhite_image[xHigh][y][1] == 0 && blackwhite_image[xHigh][y][2] == 0)
      {
        eroded_image[x][y][0] = 0;
        eroded_image[x][y][1] = 0;
        eroded_image[x][y][2] = 0;
      }
      // pixelChange checks if any changes are made in the erosion step so we know when to stop
      if (eroded_image[x][y][0] != blackwhite_image[x][y][0])
        pixelChange = 1;
    }
  }
  return pixelChange;
}

// Declaring the arrays to store the images (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
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
  clock_t start, end;
  double cpu_time_used;
  start = clock();

  // Load image from file
  read_bitmap(argv[1], input_image);

  // Run blackwhite conversion
  convert_blackwhite(input_image, blackwhite_image);

  int pixelChange = 0;
  int imgIndex = 0;
  int cell_count = 0;
  copyImage(blackwhite_image, eroded_image);

  // Do-while loop that allows us to run erosion and detection as long as the image is not fully eroded
  do
  {
    // Runs the erosion
    pixelChange = erode_image(blackwhite_image, eroded_image);

    // Saves our erosion steps so that we can keep track of the erosion
    char imgName[strlen(argv[2]) + 10];
    sprintf(imgName, "tmp_%d_%s", imgIndex, argv[2]);
    write_bitmap(eroded_image, imgName);

    copyImage(eroded_image, blackwhite_image);
    imgIndex += 1;
    // TODO: Detection step (keep track of location of detected cells)

    cell_count += detectCell(blackwhite_image);

    // these two lines of code, changes the 10 output images, so it shows the red crosses
    // after every erosion step
    // generateOutputImg(input_image);
    // write_bitmap(input_image, imgName);

  } while (pixelChange == 1 /*&& imgIndex < 10*/);

  // TODO: Generate output image
  // generates output image with red crosses into "example_inv.bmp"
  generateOutputImg(input_image);
  write_bitmap(input_image, argv[2]);

  // TODO: Save output image and print results

  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      if (detected_cells[i][j] == 1)
      {
        printf("Cell found at: (%d,%d) \n", i, j);
      }
    }
  }
  printf("Cell count: %d\n", cell_count);

  printf("Done!\n");

  end = clock();
  cpu_time_used = end - start;
  printf("Total time: %f ms\n", cpu_time_used * 1000.0 / CLOCKS_PER_SEC);

  return 0;
}
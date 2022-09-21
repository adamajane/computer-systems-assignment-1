//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

//Function to invert pixels of an image (negative)
 void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
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
int checkExclusionFrame (unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],int x, int y){
  // initiating for-loop, to go through 14 pixels on all 4 sides of the exclusionframe(square)
		for (int exclusion_pixel = 0; exclusion_pixel <= 31; exclusion_pixel++) {
      // if statement checks for white pixels in the UPPER side of square
			if(blackwhite_image[(x-1)+exclusion_pixel][(y-1)][0]== 255 
      && blackwhite_image[(x-1)+exclusion_pixel][(y-1)][1] == 255 
      && blackwhite_image[(x-1)+exclusion_pixel][(y-1)][2] == 255) {
				return 1;
				break;
			}
      // if statement checks for white pixels in the LOWER side of square
			else if (blackwhite_image[(x-1)+exclusion_pixel][((y-1)+31)][0]== 255 
      && blackwhite_image[(x-1)+exclusion_pixel][((y-1)+31)][1] == 255 
      && blackwhite_image[(x-1)+exclusion_pixel][((y-1)+31)][2] == 255) {
				return 1;
				break;
			}
      // if statement checks for white pixels in the LEFT side of square
			else if(blackwhite_image[(x-1)][(y-1)+exclusion_pixel][0]== 255 
      && blackwhite_image[(x-1)][(y-1)+exclusion_pixel][1] == 255 
      && blackwhite_image[(x-1)][(y-1)+exclusion_pixel][2] == 255) {
				return 1;
				break;
			}
      // if statement checks for white pixels in the RIGHT side of square
			else if(blackwhite_image[((x-1)+31)][(y-1)+exclusion_pixel][0]== 255 
      && blackwhite_image[((x-1)+31)][(y-1)+exclusion_pixel][1] == 255 
      && blackwhite_image[((x-1)+31)][(y-1)+exclusion_pixel][2] == 255) {
				return 1;
				break;
			}
      
		}
		return 0;
		
	}


	
	
	// for every pixel in the image, we check the 12x12 pixel area to the right and down.
  // if there is a white pixel in this area, we should proceed to check if the 
  // exclusionframe contains any white pixels, using the "checkExclusionFrame()"- function.
  // If it doesn't contain a white pixel, we will add 1 to cell_count, and move on to
  // the next pixel.
	void detectCell (unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
		  int cell_count = 0;
      int detect_x;
      int detect_y;
      int flag = 0;
		  for (int x = 0; x < BMP_WIDTH-30; x++)
		  {
		    for (int y = 0; y < BMP_HEIGTH-30; y++)
		    {
          // the next nested for-loop go through the 12x12 pixel area and checks for white pixel
		    	for (detect_x = 0; detect_x <= 30; detect_x++ ) {
		    		
		    		for (detect_y = 0; detect_y <= 30; detect_y++) {

		    			if (blackwhite_image[x+detect_x][y+detect_y][0] == 255 
              && blackwhite_image[x+detect_x][y+detect_y][1] == 255 
		    			&& blackwhite_image[x+detect_x][y+detect_y][2] == 255) {

		    				if(checkExclusionFrame(blackwhite_image, x, y) == 0) {
                  // if we have found a white pixel and the exclusionframe is clear aswell
                  // we increment the cell count by 1. We set the flag to 1
                  // to let us know that we should break out of the nested detection 
                  // for-loop after coloring all the pixels black
			    			  cell_count++;
                  flag = 1;
                  // after we have incremented the cell count, we color all the pixels
                  // in the detection area black and break out of
                  
                    for (int black_x = detect_x; black_x <= 30; black_x++ ) {
		    		
		    		          for (int black_y = detect_y; black_y <= 30; black_y++) {
                          blackwhite_image[x+black_x][y+black_y][0] = 0;
                          blackwhite_image[x+black_x][y+black_y][1] = 0;
                          blackwhite_image[x+black_x][y+black_y][2] = 0;
                        }
                      }
                  
                  break;
		    				}
			    			
			    		}
		    		}
		    		if(flag == 1) {
               break;
	    			}
		    		
		    	}
          
		    } 	
        printf("%d \n", cell_count);
        
		  }  
      // finally prints the total count of cells
      printf("%d \n", cell_count);
     }

// This function converts the image to black and white (applies the binary threshold)
 void convert_blackwhite(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  int res = 0;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if ((output_image[x][y][0] + output_image[x][y][1] + output_image[x][y][2]) /3 > 130){
      blackwhite_image[x][y][0] = 0;
      blackwhite_image[x][y][1] = 0;
      blackwhite_image[x][y][2] = 0;
      }
      else {
      blackwhite_image[x][y][0] = 255;
      blackwhite_image[x][y][1] = 255;
      blackwhite_image[x][y][2] = 255;
      }
    
    }
  }
}


  //Declaring the array to store the image (unsigned char = unsigned 8 bit)
  unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

//Main function
int main(int argc, char** argv)
{
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)

  //Checking that 2 arguments are passed
  if (argc != 3)
  {
      fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
      exit(1);
  }

  printf("Example program - 02132 - A1\n");

  //Load image from file
  read_bitmap(argv[1], input_image);

  //Run inversion
  invert(input_image,output_image);

  //Run blackwhite conversion
  convert_blackwhite(output_image, blackwhite_image);

  //Save image to file
  write_bitmap(blackwhite_image, argv[2]);

  detectCell(blackwhite_image);


  printf("Done!\n");
  return 0;
}

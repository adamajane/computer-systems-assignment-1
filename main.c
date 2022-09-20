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

int checkExclusionFrame (){
		for (int exclusion_pixel = 0; exclusion_pixel <= 14; exclusion_pixel++) {
			if(blackwhite_image[(x-1)+exclusion_pixel][(y-1)][0]== 255 
      && blackwhite_image[(x-1)+exclusion_pixel][(y-1)][1] == 255 
      && blackwhite_image[(x-1)+exclusion_pixel][(y-1)][2] == 255) {
				return 1;
				break;
			}
			else if (blackwhite_image[(x-1)+exclusion_pixel][((y-1)+13)][0]== 255 
      && blackwhite_image[(x-1)+exclusion_pixel][((y-1)+13)][1] == 255 
      && blackwhite_image[(x-1)+exclusion_pixel][((y-1)+13)][2] == 255) {
				return 1;
				break;
			}
			else if(blackwhite_image[(x-1)][(y-1)+exclusion_pixel][0]== 255 
      && blackwhite_image[(x-1)][(y-1)+exclusion_pixel][1] == 255 
      && blackwhite_image[(x-1)][(y-1)+exclusion_pixel][2] == 255) {
				return 1;
				break;
			}
			else if(blackwhite_image[((x-1)+13)][(y-1)+exclusion_pixel][0]== 255 
      && blackwhite_image[((x-1)+13)][(y-1)+exclusion_pixel][1] == 255 
      && blackwhite_image[((x-1)+13)][(y-1)+exclusion_pixel][2] == 255) {
				return 1;
				break;
			}
      
		}
		return 0;
		
	}


	
	
	
	void detectCell (unsigned char blackwhite_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
		  int cell_count = 0;
      int detect_x;
      int detect_y;
		  for (int x = 0; x < BMP_WIDTH-11; x++)
		  {
		    for (int y = 0; y < BMP_HEIGTH-11; y++)
		    {
		    	for (detect_x = 0; detect_x <= 11; detect_x++ ) {
		    		
		    		for (detect_y = 0; detect_y <= 11; detect_y++) {

		    			if (blackwhite_image[x+detect_x][y+detect_y][0] == 255 
              && blackwhite_image[x+detect_x][y+detect_y][1] == 255 
		    			&& blackwhite_image[x+detect_x][y+detect_y][2] == 255) {

		    				if(checkExclusionFrame() == 0) {
			    			  cell_count++;
			    			  break;
		    				}
			    			
			    		}
		    		}
		    		if(checkExclusionFrame() == 0) {

              if (blackwhite_image[x+detect_x][y+detect_y][0] == 255 
              && blackwhite_image[x+detect_x][y+detect_y][1] == 255 
		    			&& blackwhite_image[x+detect_x][y+detect_y][2] == 255) {

		    			  break;
              }
	    			}
		    		
		    	}
		    } 	
		  }  
      printf("%d", cell_count);
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

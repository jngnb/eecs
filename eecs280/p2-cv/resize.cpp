// Project UID af1f95f547e44c8ea88730dfb185559d
#include <iostream>
using namespace std;

#include <cassert>
#include "Matrix.h"
#include "Image.h"
#include "processing.h"
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>

int main(int argc, char *argv[]){

    string input_img = argv[1];
    string written_img = argv[2];
    int width_arg = atoi(argv[3]);
    int height_arg = atoi(argv[4]);

    ifstream fin(input_img);

    Image *img = new Image;
    Image_init(img,fin);
    
    
    if (!fin.is_open()) {
        cout << "Error opening file: " << input_img << endl;
        return 1;
    }
  
    if (argc != 4 && argc != 5){ 
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }
     if (width_arg < 0 || width_arg > Image_width(img)){
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
         << "WIDTH and HEIGHT must be less than or equal to original" << endl;
         return 1;
    }

     if (height_arg < 0 || height_arg > Image_height(img)){ 
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }
                
    seam_carve(img, width_arg, height_arg); 

    ofstream fout(written_img);

    Image_print(img,fout);
    
    delete img;
    
}

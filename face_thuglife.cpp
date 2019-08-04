#include <iostream>
#include <cstring>
#include <unistd.h>
#include <dlib/image_io.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace dlib;

char output_file[FILENAME_MAX];

/*
 * This function transforms the input file name to the output name
 * if none output name given
 *
 * An example of the conversion is shown below:
 * "/path/to/img.jpg" => "/path/to/img_thuglife.png"
 * Notice that the extension is forced to be PNG at last
 *
 * P.S. I like C-style symbols!
 */
extern "C" void form_output_filename(const char *input_file)
{
	char *ptr,*end;
	strcpy(output_file,input_file);	// Alter them in the output buffer
	end=output_file+strlen(output_file);	// Start from the end
	/*
	 * Loop from last char to 1st char in order to
	 * split the extension
	 */
	for(ptr=end-1;ptr>=output_file && *ptr!='.';ptr--);
	/*
	 * If dot was found, then place the suffix here,
	 * otherwise place it at the end
	 */
	ptr= *ptr=='.' ? ptr : end;
	strcpy(ptr,"_thuglife.png");
}

int main(int argc,char **argv)
{
	array2d<rgb_pixel> img;
	if(argc<2){
		cerr<<"usage: "<<argv[0]<<" image_file [output_file]"<<endl;
		return 1;
	}
	if(argc>=3)
		strcpy(output_file,argv[2]);
	else
		form_output_filename(argv[1]);
	cout<<"Processing `"<<argv[1]<<'\''<<endl;
	if(access(argv[1],R_OK)){
		perror(argv[1]);
		return -1;
	}
	cout<<"Output production to `"<<output_file<<'\''<<endl;
	return 0;
}

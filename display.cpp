#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc,char **argv)
{
	if(argc<2){
		cerr<<"usage: "<<argv[0]<<" image_file"<<endl;
		return 1;
	}
	Mat img=imread(String(argv[1]),IMREAD_UNCHANGED);
	if(img.data==NULL){
		cerr<<"error: File `"<<argv[1]<<"\' not found"<<endl;
		return 1;
	}
	imshow(String("View image"),img);
	waitKey();
	return 0;
}

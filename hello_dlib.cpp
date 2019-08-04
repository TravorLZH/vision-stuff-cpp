#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_io.h>
#include <iostream>

using namespace std;
using namespace dlib;

int main(int argc,char **argv)
{
	array2d<unsigned char> img;
	frontal_face_detector fd=get_frontal_face_detector();
	if(argc<2){
		cerr<<"usage: "<<argv[0]<<" image_file"<<endl;
		return 1;
	}
	cout<<"Looking for faces in `"<<argv[1]<<'\''<<endl;
	load_image(img,argv[1]);
	std::vector<rectangle> dets=fd(img);
	cout<<"Found "<<dets.size()<<" faces"<<endl;
	return 0;
}

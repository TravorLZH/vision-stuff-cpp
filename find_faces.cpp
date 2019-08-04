#include <iostream>
#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_io.h>

using namespace std;
using namespace cv;
using namespace dlib;

static Rect convert_rect(dlib::rectangle rect)
{
	return Rect(Point2i(rect.left(),rect.top()),Point2i(rect.right()+1,
				rect.bottom()+1));
}

int main(int argc,char **argv)
{
	array2d<rgb_pixel> img;	// Use grayscale to optimize
	frontal_face_detector detector=get_frontal_face_detector();
	if(argc<2){
		cerr<<"usage: "<<argv[0]<<" image_file"<<endl;
		return 1;
	}
	cout<<"Looking for faces in `"<<argv[1]<<'\''<<endl;
	load_image(img,argv[1]);
	Mat cv_img;
	cvtColor(toMat(img),cv_img,COLOR_RGB2BGR);
	std::vector<dlib::rectangle> dets=detector(img);
	cout<<"Found "<<dets.size()<<" faces"<<endl;
	for(auto iter=dets.cbegin();iter!=dets.cend();iter++){
		cv::rectangle(cv_img,convert_rect(*iter),Scalar(0,0xFF,0),
				3);
	}
	imshow(String("See your faces"),cv_img);
	waitKey();
	return 0;
}

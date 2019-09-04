#include <iostream>
#include <cstring>
#define	NDEBUG	// Do not perform assertion
#include <cassert>
#include <cmath>
#include <unistd.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>

/* This is precise enough to convert radians to degrees */
#define	PI	3.14159265358

using namespace std;
using namespace dlib;

char output_file[FILENAME_MAX];

/* Name of the landmark shape predictor model file */
const char landmark_sp_model[]="shape_predictor_68_face_landmarks.dat";

/* Shape predictor that is used to detect face landmarks */
static shape_predictor sp;

/* Image of the sunglasses */
const char thuglife_name[]="thuglife_resized.png";
cv::Mat thuglife_img;
int tl_width;
int tl_height;

/*
 * This function transforms the input file name to the output name
 * if none output name given
 *
 * An example of the conversion is shown below:
 * "/path/to/img.jpg" => "/path/to/img_thuglife.png"
 * Notice that the extension is forced to be PNG at last
 */
void form_output_filename(const char *input_file)
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

void put_sunglasses(cv::Mat &img,full_object_detection &shape)
{
	cv::Mat rot_matrix;
	cv::Mat tl_resized;
	cv::Mat tl_rotated;	// Rotated sunglasses picture
	point p1,p2;
	int i,j,w,h;
	int x0,y0,x1,y1;	// Region to put sunglasses picture
	int len;
	float slope,angle,ratio;
	len=shape.num_parts();
	assert(len==68);	// Make sure it is the genuine shape predictor
	p1=shape.part(36);
	p2=shape.part(45);
	/* Rotate the sunglasses picture */
	int dy=p2.y()-p1.y();
	int dx=p2.x()-p1.x();
	slope=(float)dy/(float)dx;
	angle=-atan(slope)/PI*180;	// This is used to create rotation matrix
	ratio=sqrt(dx*dx+dy*dy)/tl_width*2;
	cout<<"Factor to resize sunglasses: "<<ratio<<endl;
	cout<<"Degree to rotate (clockwise) sunglasses: "<<angle<<endl;
	cv::resize(thuglife_img,tl_resized,cv::Size(),ratio,ratio);
	w=tl_resized.cols;h=tl_resized.rows;
	rot_matrix=cv::getRotationMatrix2D(cv::Point(w/2,h/2),angle,1);
	cv::warpAffine(tl_resized,tl_rotated,rot_matrix,tl_resized.size());
	x0=p1.x()-0.37*(float)w;
	y0=p1.y()-h/2;
	x1=x0+w;
	y1=y0+h;
	/* Now put this sunglasses on the person */
	cout<<"Start putting sunglasses"<<endl;
	for(i=y0;i<y1;i++){
		for(j=x0;j<x1;j++){
			auto &bg_color=img.at<cv::Vec3b>(i,j);
			auto fg_color=tl_rotated.at<cv::Vec4b>(i-y0,j-x0);
			if(fg_color[3]>=0x80){
				bg_color[0]=fg_color[0];
				bg_color[1]=fg_color[1];
				bg_color[2]=fg_color[2];
			}
		}
	}
}

int main(int argc,char **argv)
{
	array2d<rgb_pixel> img;
	cv::Mat cv_img;
	frontal_face_detector detector;
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
	cout<<"Loading `"<<landmark_sp_model<<"\'..."<<endl;
	if(access(landmark_sp_model,R_OK)){
		cerr<<"error: `"<<landmark_sp_model<<"\' must be in the" \
			" working directory"<<endl;
		cerr<<"Obtain it from <http://dlib.net/files/" \
			"shape_predictor_68_face_landmarks.dat.bz2>"<<endl;
		return -2;
	}
	/* Initialize face detector and landmark predictor */
	detector=get_frontal_face_detector();
	deserialize(landmark_sp_model) >> sp;
	/* Get rectangles that contain faces */
	load_image(img,argv[1]);
	cout<<"Detecting faces"<<endl;
	std::vector<rectangle> dets=detector(img);
	if(dets.size()==0){
		cerr<<"error: Found no faces, quitting..."<<endl;
		return -1;
	}
	cout<<"Found "<<dets.size()<<" faces"<<endl;
	/* Now read the sunglasses picture */
	// NOTE: It is a four channel image
	cout<<"Loading `"<<thuglife_name<<"\'..."<<endl;
	if(access(thuglife_name,R_OK)){
		perror(thuglife_name);
		return 2;
	}
	thuglife_img=imread(thuglife_name,cv::IMREAD_UNCHANGED);
	tl_width=thuglife_img.cols;
	tl_height=thuglife_img.rows;
	assert(thuglife_img.channels()==4);
	cv::cvtColor(toMat(img),cv_img,cv::COLOR_RGB2BGR);
	cout<<"Detecting face landmarks..."<<endl;
	for(int i=0;i<dets.size();i++){
		full_object_detection shape=sp(img,dets[i]);
		put_sunglasses(cv_img,shape);
	}
	cout<<"Output production to `"<<output_file<<"\'..."<<endl;
	cv::imwrite(output_file,cv_img);
	return 0;
}

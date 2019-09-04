#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc,char **argv)
{
	Mat kernel,img,result;
	if(argc<2){
		cerr<<"usage: "<<argv[0]<<" image_file"<<endl;
		return 1;
	}
	kernel=(Mat_<int>(3,3)<<-1,0,1,-2,0,2,-1,0,1);
	img=imread(argv[1],IMREAD_GRAYSCALE);
	if(!img.data){
		cerr<<argv[1]<<" does not exist"<<endl;
		return -1;
	}
	namedWindow("Convolved",WINDOW_AUTOSIZE);
	filter2D(img,result,-1,kernel,Point(-1,-1),0,BORDER_DEFAULT);
	imshow("Convolved",result);
	waitKey(0);
	return 0;
}

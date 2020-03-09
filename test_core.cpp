#include <stdio.h>
#include "core.h"
#include <hls_opencv.h>
#include "hls_video.h"
//#define INPUT_IMAGE "test(1).jpg"
//#define INPUT_IMAGE "test(2).jpg"
#define INPUT_IMAGE "test(3).jpg"

#define OUTPUT_IMAGE "outputimage.jpg"

int main()
{
	ap_uint<11> row=1080,col=1920,custom_row=566,custom_col=677;
	/*unsigned char re_val;*/
	ap_uint<1> set_led;
	ap_uint<4> variation=5;
	ap_uint<4> time_m=3;
	ap_uint<6> fps=1;
	/*for(int i=0;i<5;i++)
	{*/
	cv::Mat src = cv::imread(INPUT_IMAGE);
	cv::Mat dst = src;
    stream_t video_in,video_out;

    cvMat2AXIvideo(src, video_in);

   led_timer(video_in,video_out,row,col,custom_row,custom_col,
		   variation,time_m,fps,&set_led);

    printf("led_timer return =%d, central pxl= \n ",(unsigned char)set_led);

    AXIvideo2cvMat(video_out, dst);
	cv::imwrite(OUTPUT_IMAGE, dst);

	/*}*/
	return 0;
}

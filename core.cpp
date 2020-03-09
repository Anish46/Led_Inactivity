#include "core.h"

unsigned short count;
unsigned char pxl_check;


void led_timer(stream_t &video_in,stream_t &video_out,
		ap_uint<11> row,ap_uint<11> col,
		ap_uint<11> custom_row,ap_uint<11> custom_col,
		ap_uint<4> variation,ap_uint<4> time_m,ap_uint<6>fps,
		ap_uint<1> *set_led)
{

/*
#pragma HLS INTERFACE ap_stable port=fps
#pragma HLS INTERFACE ap_stable port=time_m
#pragma HLS INTERFACE ap_stable port=variation
#pragma HLS INTERFACE ap_stable port=custom_col
#pragma HLS INTERFACE ap_stable port=custom_row
*/
#pragma HLS INTERFACE ap_stable port=col
#pragma HLS INTERFACE ap_stable port=row

/*#pragma HLS RESOURCE variable=ctl_pxl_c core=AddSub_DSP*/
/*#pragma HLS ALLOCATION instances=add limit=3500000 operation*/
#pragma HLS INTERFACE s_axilite port=fps bundle=CTL
#pragma HLS INTERFACE s_axilite port=time_m bundle=CTL
#pragma HLS INTERFACE s_axilite port=variation bundle=CTL
#pragma HLS INTERFACE s_axilite port=custom_col bundle=CTL
#pragma HLS INTERFACE s_axilite port=custom_row bundle=CTL clock=axi_clk
#pragma HLS INTERFACE s_axilite port=col bundle=CTL
#pragma HLS INTERFACE s_axilite port=row bundle=CTL


#pragma HLS INTERFACE s_axilite register depth=1 port=set_led bundle=CTL
#pragma HLS INTERFACE s_axilite port=return bundle=CTL
#pragma HLS INTERFACE axis register both port=video_in
#pragma HLS INTERFACE axis register both port=video_out

interface_t pixelOut;
ap_uint<11> accum=0;
ap_uint<11> temp1=0,temp2=0;

if (pxl_check<variation)
{	pxl_check=variation;}

else if (pxl_check>255-variation)
{   pxl_check=255-variation;}


for (temp1=0; temp1 < row; temp1++)
{
/*#pragma HLS LOOP_FLATTEN*/
#pragma HLS LOOP_TRIPCOUNT min=0 max=1080
	for (temp2=0; temp2 < col; temp2++)
	{
#pragma HLS LOOP_TRIPCOUNT min=0 max=1920
		#pragma HLS PIPELINE

	    interface_t currentPixel = video_in.read();

	     if(((custom_row-2 < temp1) && (temp1 < custom_row+2)) &&
	    	((custom_col-2 < temp2) && (temp2 < custom_col+2)))
			{
	    	 accum +=  currentPixel.data;
			}
		pixelOut.data = currentPixel.data;

		pixelOut.keep = currentPixel.keep;
		pixelOut.strb = currentPixel.strb;
		pixelOut.user = currentPixel.user;
		pixelOut.last = currentPixel.last;
		pixelOut.id   = currentPixel.id;
		pixelOut.dest = currentPixel.dest;

		video_out.write(pixelOut);

	}
 }
temp1=accum/9;



//Reusing existing variable 'temp1' optimizes area
//Checks whether the current pixel matches with pixel of previous frame
    if (((pxl_check-variation) < temp1)  && (temp1 < (pxl_check+variation)))
     {	   count++;}
     else
     {count=0;}

   /*(((ctl_pxl-variation) < temp1)  && (temp1 < (ctl_pxl+variation))) ?
		   count=count+1 :count=0;
    */
   if(count>60000)
   	   {count=60000;}

   pxl_check=temp1;


  (count>time_m*fps)? *set_led=0 : *set_led=1 ;

}

#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_int.h>

typedef ap_axiu<24,1,1,1> interface_t;
typedef hls::stream<interface_t> stream_t;


void led_timer(stream_t &video_in,stream_t &video_out,
		ap_uint<11> row,ap_uint<11> col,
		ap_uint<11> custom_row,ap_uint<11> custom_col,
		ap_uint<4> variation,ap_uint<4> time_m,ap_uint<6>fps,
		ap_uint<1> *set_led);

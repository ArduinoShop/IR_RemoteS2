#ifndef  H_IRrmt_H
#define H_IRrmt_H

#include "IRrmt.h"
#include "driver/rmt.h"
#include "ir_tools.h"
#include "sdkconfig.h"

class IRrmt {
	public:
	
		IRrmt(   int ch );  //  Constructor

		void ir_rx(  );

	private:
	
		rmt_channel_t rx_channel;
		RingbufHandle_t rb;
		ir_parser_t *ir_parser;

						};

#endif

// #include "TankS2.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include "driver/rmt.h"
#include "ir_tools.h"

#include "IRrmt.h"

static const char *TAG = "IR";

IRrmt::IRrmt(   int ch )  //  constructor( ch )
{
// static rmt_channel_t tx_channel = RMT_CHANNEL_0;

// gpio_pad_select_gpio( (gpio_num_t)CONFIG_EXAMPLE_RMT_RX_GPIO ); gpio_set_direction( (gpio_num_t)CONFIG_EXAMPLE_RMT_RX_GPIO, GPIO_MODE_INPUT);

//		ESP_ERROR_CHECK( gpio_set_pull_mode( (gpio_num_t) CONFIG_EXAMPLE_RMT_RX_GPIO, GPIO_FLOATING ) );  //  PULLUP_ONLY ) );
		
		

	rx_channel = RMT_CHANNEL_1;

	rb = NULL;
//	*ir_parser = ( ir_parser_t * ) NULL;
    
    rmt_config_t rmt_rx_config; 
    
         rmt_rx_config.rmt_mode = RMT_MODE_RX;
         rmt_rx_config.channel = rx_channel;
         rmt_rx_config.gpio_num = (gpio_num_t) CONFIG_EXAMPLE_RMT_RX_GPIO;
         rmt_rx_config.clk_div = 80;
         rmt_rx_config.mem_block_num = 1;
         rmt_rx_config.flags = 0;
       
		rmt_rx_config.rx_config.idle_threshold = 12000;
        rmt_rx_config.rx_config  .filter_ticks_thresh = 100;
	    rmt_rx_config.rx_config.filter_en = true;
       
    rmt_config(&rmt_rx_config);
    rmt_driver_install(rx_channel, 1000, 0);
    ir_parser_config_t ir_parser_config = IR_PARSER_DEFAULT_CONFIG((ir_dev_t)rx_channel);
    ir_parser_config.flags |= IR_TOOLS_FLAGS_PROTO_EXT; // Using extended IR protocols (both NEC and RC5 have extended version)

    #if CONFIG_EXAMPLE_IR_PROTOCOL_NEC
    ir_parser = ir_parser_rmt_new_nec(&ir_parser_config);
      printf( "IRrmt Constructor:  using NEC protocol on pin #%d\n", CONFIG_EXAMPLE_RMT_RX_GPIO );
#elif CONFIG_EXAMPLE_IR_PROTOCOL_RC5
    ir_parser = ir_parser_rmt_new_rc5(&ir_parser_config);
   printf( "IRrmt Constructor:  using RC5 protocol on pin #%d\n", CONFIG_EXAMPLE_RMT_RX_GPIO );
#endif	


    //get RMT RX ringbuffer
    rmt_get_ringbuf_handle(rx_channel, &rb);
    // Start receive
    rmt_rx_start(rx_channel, true);
    
    printf( "IRrmt Constructor Done\n");
    
  }  //  end of init  
    
void IRrmt::ir_rx( ) {  
    uint32_t addr = 0;
    uint32_t cmd = 0;
    uint32_t length = 0;
    bool repeat = false;
	rmt_item32_t *items = NULL;  
	
	rmt_get_ringbuf_handle( rx_channel, &rb );
    rmt_rx_start( rx_channel, true );
	
    while (rb) {
        items = (rmt_item32_t *) xRingbufferReceive(rb, &length, 1000);
        if (items) {
            length /= 4; // one RMT = 4 Bytes
            if (ir_parser->input(ir_parser, items, length) == ESP_OK) {
                if (ir_parser->get_scan_code(ir_parser, &addr, &cmd, &repeat) == ESP_OK) {
                    printf("Scan Code %s --- addr: 0x%04x cmd: 0x%04x", repeat ? "(repeat)" : "", addr, cmd);
                }
            }
            //after parsing the data, return spaces to ringbuffer.
            vRingbufferReturnItem(rb, (void *) items);
        } else {
            break;
        }
    }
   }  //  end  ir_rx
   



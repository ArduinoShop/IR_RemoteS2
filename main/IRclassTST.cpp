#include "IRrmt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C"  void app_main() {
	
	IRrmt IR = IRrmt( 1 );
	
	while( 1 ) {
	printf( " call ir rx \n");
	    IR.ir_rx();
		printf( "ir rx  called \n");
		vTaskDelay( 300 );
}
	
}   


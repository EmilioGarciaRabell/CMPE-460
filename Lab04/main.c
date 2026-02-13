#include "../lib/lab1/switches.h"
#include "../lib/lab4/I2C/oled.h"

void part2(void);
void part3(void);
void print_ascii(const char *ascii);
bool DisplayCameraData = false; 
bool DisplayThreshCameraData = false;
unsigned char OLED_TEXT_ARR[1024];
unsigned char OLED_GRAPH_ARR[1024];

// delay constant
#define DELAY_COUNT 2000000

void s_delay(void);


/**
 * @brief Delay function
 * @note Uses a volatile loop create a delay
 */
void s_delay(void) {
    for(volatile long i = 0; i < DELAY_COUNT; i++); 
}



void part2(){
	
  
	OLED_Init();
	OLED_display_clear();
	OLED_draw_line(1,1, (unsigned char *)"Hello World");
    OLED_draw_line(2, 2, (unsigned char *)"How are you?");
   OLED_draw_line(3, 3, (unsigned char *)"Goodbye");
	
	while(1)
	{
		OLED_write_display(OLED_TEXT_ARR);
		s_delay();
	}
}

void part3(){

    int press_count = 0;

        
		char *cat =
		"     \n"
    "     /\\_/\\ \n"
    "    ( o o )\n"
    "     > ^ < \0";

    char *robot = 
						"     \n"
            "    [o_o] \n"
            "    /|_|\\ \n"
            "    /   \\ \0";

    char *ghost = 
        "   .-.   \n"
        "  (o o)  \n"
        "  | O |  \n"
        "  |   |  \n"
        " /'---'\\ \0";
    OLED_Init();
		OLED_display_clear();
    S1_init();
    
    
    print_ascii(cat);
		s_delay();
    while(1){
        if(S1_pressed()){
            press_count ++;
        
            
            if (press_count == 1){
                print_ascii(robot);
            
            }else if(press_count == 2){
                print_ascii(ghost);
           
            }else if (press_count == 3){
                print_ascii(cat);
                press_count = 0;
            }
            s_delay();
            
        }
    }
}

void print_ascii(const char *ascii){
		OLED_display_clear();
    int row = 0;
    int col = 0;
    while(*ascii != '\0'){
        if (*ascii == '\n'){
            row ++;
            col = 0;
        }else{
            OLED_draw(row, col,*ascii);
            col ++;
        }
        ascii++;
			OLED_write_display(OLED_TEXT_ARR);
			
    }
		s_delay();
	
}


int main(void){
	part2();
}




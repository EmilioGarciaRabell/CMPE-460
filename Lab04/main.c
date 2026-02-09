#include "../lib/lab1/switches.h"
#include "../lib/lab4/I2C/oled.h"

void part2(void);
void part3(void);
void print_ascii(char *ascii);
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
	
    char *str1 = "Hello World";
    char *str2 = "How are you?";
    char *str3 = "Goodbye";


	// i. initialize OLED
	OLED_Init();
	OLED_display_clear();
	// ii. Write �Hello World� on line 1, column 1
	OLED_draw_line(1,1, (unsigned char *)"Hello World");
	// iii. Write �How are you?� on line 2, column 2
    //OLED_Print(2, 2, str2);
	// iv. Write �Goodbye� on line 3, column 3
    //OLED_Print(3, 3, str1);
	while(1)
	{
		OLED_write_display(OLED_TEXT_ARR);
		for (volatile int i = 0; i < 10000000; i++)
		{
		}
	}
}

void part3(){

    int press_count = 0;

    char *heavy_thumb = 
        "╱╱┏╮\n"
        "╱╱┃┃\n"
        "|━╯┗━╮\n"
        "|┈┈┈┈┃\n"
        "|╮┈┈┈┃\n"
        "╱╰━━━╯";
        

    char *robot = 
            "  [o_o] \n"
            "  /|_|\\ \n"
            "  /   \\ ";

    char *ghost = 
        "   .-.   \n"
        "  (o o)  \n"
        "  | O |  \n"
        "  |   |  \n"
        " /'---'\\ \n";
    OLED_Init();
    S1_init();
    
    // i. Display an ASCII art image (image constructed from ASCII symbols)
    print_ascii(heavy_thumb);
    while(1){
        if(S1_pressed()){
            press_count ++;
        
            // ii. When SW1 is pressed, display a second ASCII art image.
            if (press_count == 1){
                print_ascii(robot);
            // iii. When SW1 is pressed again, display a third ASCII art image.
            }else if(press_count == 2){
                print_ascii(ghost);
            // iv. When SW1 is pressed a third time, display the first ASCII art image again, and continue this loop
            }else if (press_count == 3){
                print_ascii(heavy_thumb);
                press_count = 0;
            }
            s_delay();
            
        }
    }
}

void print_ascii(char *ascii){
    int row = 0;
    int col = 0;
    while(*ascii != '\0'){
        if (*ascii == '\n'){
            row ++;
            col = 0;
        }else{
            OLED_Print(row, col,ascii);
            col ++;
        }
        ascii++;

    }
}


int main(void){
	part2();
}




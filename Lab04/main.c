#include "oled.c"
#inculde "switches.c"


void part2(void);
void part3(void);
void print_ascii(char *ascii);


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
    char *str3 = "Goodbye"


	// i. initialize OLED
	OLED_Init();
	// ii. Write �Hello World� on line 1, column 1
	OLED_Print(1, 1, str1);
	// iii. Write �How are you?� on line 2, column 2
    OLED_Print(2, 2, str2);
	// iv. Write �Goodbye� on line 3, column 3
    OLED_Print(3, 3, str1);
}

void part3(){

    int press_count = 0;

    const char *thumb_up = 
        "      _ \n"
        "     /(|\n"
        "    (  :\n"
        "   __\\  \\  _____\n"
        " (____)  `|\n"
        "(____)|   |\n"
        " (____).__|\n"
        "  (___)__.|_____";

    const char *heavy_thumb = 
        "╱╱┏╮\n"
        "╱╱┃┃\n"
        "|━╯┗━╮\n"
        "|┈┈┈┈┃\n"
        "|╮┈┈┈┃\n"
        "╱╰━━━╯";
        

    const char *robot = 
            "  [o_o] \n"
            "  /|_|\\ \n"
            "  /   \\ ";

    const char *ghost = 
        "   .-.   \n"
        "  (o o)  \n"
        "  | O |  \n"
        "  |   |  \n"
        " /'---'\\ \n";
    OLED_Init();
    S1_init();
    
    // i. Display an ASCII art image (image constructed from ASCII symbols)
    print_ascii(thumb_up);
    while(1){
        if(S1_pressed()){
            press_count ++;
        
            // ii. When SW1 is pressed, display a second ASCII art image.
            if (press_count == 1){
                print_ascii();
            // iii. When SW1 is pressed again, display a third ASCII art image.
            }else if(press_count == 2){
                print_ascii();
            // iv. When SW1 is pressed a third time, display the first ASCII art image again, and continue this loop
            }else if (press_count == 3){
                print_ascii();
                press_count == 0;
            }
            s_delay();
            
        }
    }
}

void print_ascii(char *ascii){
    int row = 0;
    int col = 0;
    while(*ascii != "\0"){
        if (*ascii == "\n"){
            row ++;
            col = 0;
        }else{
            OLED_Print(row, col,ascii);
            col ++;
        }
        ascii++

    }
}


int main(void){
	
}




#include "MK60D10.h"
#include <ctype.h>
#include "characters.h"

#define BTN_SW2 0x400     // bit 10
#define BTN_SW3 0x1000    // bit 12
#define BTN_SW4 0x8000000 // bit 27
#define BTN_SW5 0x4000000 // bit 26
#define BTN_SW6 0x800     // bit 11


#define BOTH_DISPLAYS 16


// Macros for the GPIO registers
#define GPIO_PIN_MASK 0x1Fu
#define GPIO_PIN(x)   (((1) << (x & GPIO_PIN_MASK)))


void MCUInit(void){
    // Set clock for MCU
    MCG_C4 |= (MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01));
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);

    // Deactivate watchdog
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}


void system_config(){
    // Configure out pins for columns and rows
    uint32_t colPins = {8, 10, 6, 11};
    uint32_t rowPins = {26, 24, 9, 25, 28, 7, 27, 29};

    // Enable clock for PORTA and PORTE
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;

    // Set pins of PORTA as GPIO - columns
    for(int i = 0; i < 4; i++){
        PORTA->PCR[colPins[i]] = PORT_PCR_MUX(0x01);
    }

    // Set pins of PORTA as GPIO - rows
    for (int i = 0; i < 8; i++) {
        PORTA->PCR[rowPins[i]] = PORT_PCR_MUX(0x01);
    }

    // Set pin in PORTE
    PORTE->PCR[28] = PORT_PCR_MUX(0x01);

    // Set outputs pins of PORTA
    PTA->PDDR = GPIO_PDDR_PDD(0x3F000FC0);

    // Set outputs pins of PORTE
    PTE->PDDR = GPIO_PDDR_PDD(GPIO_PIN(28));
}


void init_button_ports(void){
    // List of pins for buttons
    uint32_t pins[] = {10, 12, 27, 26, 11};

    // Set pins of PORTE as GPIO
    for(int i = 0; i < 5; i++){
        PORTE->PCR[pins[i]] = PORT_PCR_MUX(0x01); // Buttons SW2, SW3, SW4, SW5, SW6
    }
}


void check_buttons_and_display(void){
    if(!(GPIOE_PDIR & BTN_SW2)){
        display_text("PROJEKT", 7); // Display text "PROJEKT" when button SW2 is pressed
    }
    else if(!(GPIOE_PDIR & BTN_SW3)){
        display_text("IMP", 3); // Display text "IMP" when button SW3 is pressed
    }
    else if(!(GPIOE_PDIR & BTN_SW4)){
        display_text("XHRDLI18", 8); // Display text "XHRDLI18" when button SW4 is pressed
    }
    else if(!(GPIOE_PDIR & BTN_SW5)){
        display_text("HELLO WORLD", 11);    // Display text "AHOJ" when button SW5 is pressed
    }
    else if(!(GPIOE_PDIR & BTN_SW6)){
        display_text("0123", 4);    // Display text "0123" when button SW6 is pressed
    }
}


void create_string(char field[HEIGHT][MAX], char *text, int length){
    int fieldLength = 0;

    // Convert text to uppercase
    for(int i = 0; i < length; i++){
        text[i] = toupper(text[i]);
    }

    // Field of functions for each character
    void(*char_to_func[256])(char[HEIGHT][MAX], int) = {
        [' '] = add_Space, ['A'] = add_A, ['B'] = add_B, ['C'] = add_C,
        ['D'] = add_D, ['E'] = add_E, ['F'] = add_F, ['G'] = add_G,
        ['H'] = add_H, ['I'] = add_I, ['J'] = add_J, ['K'] = add_K,
        ['L'] = add_L, ['M'] = add_M, ['N'] = add_N, ['O'] = add_O,
        ['P'] = add_P, ['Q'] = add_Q, ['R'] = add_R, ['S'] = add_S,
        ['T'] = add_T, ['U'] = add_U, ['V'] = add_V, ['W'] = add_W,
        ['X'] = add_X, ['Y'] = add_Y, ['Z'] = add_Z, ['0'] = add_0,
        ['1'] = add_1, ['2'] = add_2, ['3'] = add_3, ['4'] = add_4,
        ['5'] = add_5, ['6'] = add_6, ['7'] = add_7,
        ['8'] = add_8, ['9'] = add_9,
    };

    // Loop for each character in the text
    for(int i = 0; i < length; i++){
        // Get the function for the character
        void(*func)(char[HEIGHT][MAX], int) = char_to_func[(unsigned char)text[i]];
        // Call the function for the character
        if(func){
            func(field, fieldLength);
        }
        // Increment the field length to next character
        fieldLength += 8;
    }
}


void column_select(unsigned int col_num){
    // Array of pins for columns
    uint32_t pins[] = {8, 10, 6, 11};

    unsigned int number = col_num;
    unsigned int col_sel[4];

    // Change the number of column to binary
    for(int i = 0; i < 4; i++){
        col_sel[i] = number % 2;
        number /= 2;
    }
    
    // Set pins by col_sel values
    for(int i = 0; i < 4; i++){
        uint32_t pin = pins[i];
        uint32_t pinmask = GPIO_PIN(pin);

        if(col_sel[i] == 0){
            PTA->PDOR &= ~GPIO_PDOR_PDO(pinmask); // Set the pin to low
        }
        else{
            PTA->PDOR |= GPIO_PDOR_PDO(pinmask); // Set the pin to high
        }
    }
}



void row_select(unsigned int row_num){
    // Array of pins for rows
    uint32_t pins[] = {
        GPIO_PIN(26),  // R0
        GPIO_PIN(24),  // R1
        GPIO_PIN(9),   // R2
        GPIO_PIN(25),  // R3
        GPIO_PIN(28),  // R4
        GPIO_PIN(7),   // R5
        GPIO_PIN(27),  // R6
        GPIO_PIN(29)   // R7
    }

    // Check if the row is in the range
    if(row_num >= 0 && row_num < 8){
        // Set all pins to low
        for(int i = 0; i < 8; i++){
            PTA->PDOR &= ~pins[i];
        }

        // Set corresponding pin to high
        PTA->PDOR |= pins[row_num];
    }
}


void delay(int t){
    for(volatile int i = 0; i < t; i++){
        __asm__("nop"); // No operation
    }
}


void pixel_on(unsigned int x, unsigned int y){
    // Check if the pixel is in the range
    if(x >= 0 && x < BOTH_DISPLAYS && y >= 0 && y < BOTH_DISPLAYS){
        // Set the pixel on column
        column_select(x);
        // Set the pixel on row
        row_select(y);
    }

    // Delay for the pixel
    delay(100);
}


void show_field(char field[HEIGHT][MAX], int length, int offset){
    // Loop for each column
    for(int x = 0; x < length; x++){
        // Loop for each row
        for(int y = 0; y < HEIGHT; y++){
            // Calculate the position of the pixel with offset
            int column = offset - 7 - x;
            int row = 7 - y;

            // Check if the value in field is 1 (pixel is on)
            if(field[x][y] == '1'){
                pixel_on(column, row); // Turn on the pixel
            }
        }
    }
    // Clear the display
    PTA->PDOR = 0x00;
}


void display_text(const char *text, int len){
    int max_len = len * 8; // Max lenght of field of characters based on the lenght of the text (8 pixels per character)
    int limit = max_len * 2 + 20; // Limit for the loop (animation) (set speed of the animation)
    char field[HEIGHT][MAX] = {0}; // 2D array for the field of characters
    create_string(field, text, len); // Create the field of characters

    // Outer loop for the animation
    for(int i = 0; i < limit; i++){
        // Inner loop for the animation
        for(int j = 0; j < 12; j++){
            show_field(field, max_len, i); // Show the field of characters with offset i
        }
    }
}


int main(void){
    MCUInit(); // Initialize the MCU
    system_config(); // Configure the system
    init_button_ports(); // Initialize the buttons

    while(1){
        check_buttons_and_display(); // Check the buttons and display the text
    }
    return 0;
}
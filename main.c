#include "MK60D10.h"
#include "characters.h"


#define BTN_SW2 0x400     // bit 10
#define BTN_SW3 0x1000    // bit 12
#define BTN_SW4 0x8000000 // bit 27
#define BTN_SW5 0x4000000 // bit 26
#define BTN_SW6 0x800     // bit 11

#define BOTH_DISPLAYS 16

// bit-level registers
#define GPIO_PIN(x)		(((1)<<(x & 0x1Fu)))


void delay(int t1, int t2){
	for(int i = 0; i < t1; i++){
		for(int j = 0; j < t2; j++);
	}
}


void column_select(unsigned int col_num){
	// Array of pins for columns
    uint32_t pins[] = {8, 10, 6, 11};

    unsigned int number = col_num;
    unsigned int col_sel[4];

    // Change the number of column to binary
    for (int i = 0; i < 4; i++){
        col_sel[i] = number % 2;
        number /= 2;
    }

    // Set pins by col_sel values
    for (int i = 0; i < 4; i++){
        uint32_t pin = pins[i];
        uint32_t pin_mask = GPIO_PIN(pin);

        if (col_sel[i] == 0){
            PTA->PDOR &= ~GPIO_PDOR_PDO(pin_mask);
        } else {
            PTA->PDOR |= GPIO_PDOR_PDO(pin_mask);
        }
    }
}


void row_select(unsigned int rowNum){
	// Array of pins for rows
    uint32_t rowValues[] = {
        GPIO_PIN(26),  // R0
        GPIO_PIN(24),  // R1
        GPIO_PIN(9),   // R2
        GPIO_PIN(25),  // R3
        GPIO_PIN(28),  // R4
        GPIO_PIN(7),   // R5
        GPIO_PIN(27),  // R6
        GPIO_PIN(29)   // R7
    };

    // Check if the row is in the range
    if (rowNum >= 0 && rowNum < 8){
        // Set all pins to LOW
        for (int i = 0; i < 8; i++) {
            PTA->PDOR &= ~rowValues[i];
        }

        // Set the corresponding pin to HIGH
        PTA->PDOR |= rowValues[rowNum];
    }
}


void pixel_on(unsigned int x, unsigned int y){
	// Check if the pixel is in the range
    if (x >= 0 && x < BOTH_DISPLAYS && y >= 0 && y < BOTH_DISPLAYS){
    	// Set the pixel on column
        column_select(x);
        // Set the pixel on row
        row_select(y);
    }

    delay(1, 500);
}


void showField(char field[HEIGHT][MAX], int length, int offset){
	// Loop for each column
    for (int x = 0; x < length; x++) {
    	// Loop for each row
        for (int y = 0; y < HEIGHT; y++) {
        	// Calculate the position of the pixel with offset
            int column = offset - 7 - x;
            int row = 7 - y;

            // Check if the value in field is 1 (pixel is on)
            if (field[y][x] == '1') {
                // Turn on the pixel
                pixel_on(column, row);
            }
        }
    }

    // Clear the screen
    PTA->PDOR = 0x00;
}


void createString(char field[HEIGHT][MAX], char *text, int len){
    int fieldLength = 0;
    // Loop for each character in the text
    for (int i = 0; i < len; i++) {
        switch (text[i]){
        	case ' ':
        		add_Space(field, fieldLength);
        		fieldLength += 8;
        		break;
        	case 'A':
        	    add_A(field, fieldLength);
        	    fieldLength += 8;
             	break;
        	case 'B':
        	    add_B(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'C':
        	    add_C(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'D':
        	    add_D(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'E':
        	    add_E(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'F':
        	    add_F(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'G':
        	    add_G(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'H':
        	    add_H(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'I':
        	    add_I(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'J':
        	    add_J(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'K':
        	    add_K(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'L':
        	    add_L(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'M':
        	    add_M(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'N':
        	    add_N(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'O':
        	    add_O(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'P':
        	    add_P(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'Q':
        	    add_Q(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'R':
        	    add_R(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'S':
        	    add_S(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'T':
        	    add_T(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'U':
        	    add_U(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'V':
        	    add_V(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'W':
        	    add_W(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'X':
        	    add_X(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'Y':
        	    add_Y(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case 'Z':
        	    add_Z(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case '0':
        	    add_0(field, fieldLength);
        	    fieldLength += 8;
        	    break;
    		case '1':
    			add_1(field, fieldLength);
    			fieldLength += 8;
    			break;
        	case '2':
        	    add_2(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case '3':
        	    add_3(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case '4':
        	    add_4(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case '5':
        	    add_5(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case '6':
        	    add_6(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case '7':
        	    add_7(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case '8':
        	    add_8(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        	case '9':
        	    add_9(field, fieldLength);
        	    fieldLength += 8;
        	    break;
        }
    }
}


void display_text(const char *text, int len){
    int maxLen = len * 8; // Max length of field of characters based on the lenght of the text (8 pixels per character)
    int forLoopLimit = maxLen * 2 + 20;
    char field[HEIGHT][MAX]; // 2D array for the field of characters
    createString(field, text, len); // Create the field of characters

    // Outer loop for the animation
    for (int i = 0; i < forLoopLimit; i++) {
    	// Inner loop for the animation
        for (int k = 0; k < 12; k++) {
            showField(field, maxLen, i); // Show the field of characters with offset i
        }
    }
}


int main(void){
	// Set clock for MCU
	MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
	SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);

	// Deactivate watchdog
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;

    // Configure out pins for columns and rows
    uint32_t colPins[] = {8, 10, 6, 11};
    uint32_t rowPins[] = {26, 24, 9, 25, 28, 7, 27, 29};

    // Enable clock for PORTA and PORTE
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;

    // Set pins of PORTA as GPIO - columns
    for (int i = 0; i < 4; i++) {
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

    // List of pins for buttons
    uint32_t pins[] = {10, 12, 27, 26, 11};

    // Set pins of PORTE as GPIO
    for (int i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
        PORTE->PCR[pins[i]] = PORT_PCR_MUX(0x01); // SW2, SW3, SW4, SW5, SW6
    }

    while (1){                  // Start an infinite loop
        if (!(GPIOE_PDIR & BTN_SW2)) {
            display_text("XHRDLI18", 8);   // Display text "XHRDLI18" when SW2 button is pressed
        } else if (!(GPIOE_PDIR & BTN_SW3)) {
            display_text("PROJEKT", 7);    // Display text "PROJEKT" when SW3 button is pressed
        } else if (!(GPIOE_PDIR & BTN_SW4)) {
            display_text("IMP", 3);         // Display text "IMP" when SW4 button is pressed
        } else if (!(GPIOE_PDIR & BTN_SW5)) {
            display_text("HELLO", 5);         // Display text "HELLO" when SW5 button is pressed
        } else if (!(GPIOE_PDIR & BTN_SW6)) {
            display_text("0123", 4);     // Display text "0123" when SW6 button is pressed
        }
    }

    // Infinite loop, the program will never reach this point
    return 0;
}

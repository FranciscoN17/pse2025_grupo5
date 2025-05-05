// LCD module connections
sbit LCD_RS at RA2_bit;
sbit LCD_EN at RA3_bit;
sbit LCD_D4 at RA4_bit;
sbit LCD_D5 at RB0_bit;      // Dados do LCD
sbit LCD_D6 at RB1_bit;      // Dados do LCD
sbit LCD_D7 at RB2_bit;      // Dados do LCD

sbit LCD_RS_Direction at TRISA2_bit;
sbit LCD_EN_Direction at TRISA3_bit;
sbit LCD_D4_Direction at TRISA4_bit;
sbit LCD_D5_Direction at TRISB0_bit;
sbit LCD_D6_Direction at TRISB1_bit;
sbit LCD_D7_Direction at TRISB2_bit;
// End LCD module connections

// Definições dos botões
#define BTN_NAVIGATE RA0_bit
#define BTN_SELECT   RA1_bit

// Dados
char *bebidas[] = {"Onidlag ", "Odeveza ", "ONIFLED ", "Coca-IME"};
char *tamanhos[] = {"P      ", "M      ", "G      "};
unsigned short tempos[] = {3, 5, 7}; // tempos em segundos

// Prototipação
void greetUser();
unsigned short selectBeverage();
unsigned short selectSize();

void main() {
    unsigned short bebida, tamanho, tempo;
    unsigned short i;

    CMCON = 0x07;         // Desabilita comparadores
    TRISA = 0b00000011;   // RA0 e RA1 como entradas
    TRISB = 0x00;         // PORTB como saída
    PORTB = 0x00;

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);

    greetUser();

    while(1) {
        bebida = selectBeverage();
        tamanho = selectSize();
        tempo = tempos[tamanho];

        Lcd_Cmd(_LCD_CLEAR);
        Lcd_Out(1, 1, bebidas[bebida]);
        Lcd_Out(2, 1, tamanhos[tamanho]);

        // Acende LED correspondente nos pinos RB3 a RB6 (sem afetar RB0–RB2 usados pelo LCD)
        PORTB = (PORTB & 0x07) | (1 << (bebida + 3));  // Mantém RB0–RB2, seta RB3–RB6

        for (i = 0; i < tempo; i++) {
            Delay_ms(1000);  // Espera 1 segundo
        }

        // Apaga LEDs em RB3–RB6 (mantém os bits RB0–RB2 do LCD)
        PORTB &= 0x07;
        Delay_ms(1000);
        Lcd_Cmd(_LCD_CLEAR);
        Lcd_Out(1,1,"Se for dirigir,");
        Lcd_Out(2,1,"nao beba!");
        Delay_ms(2000);
             // Pausa antes de reiniciar
    }
}

void greetUser() {
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"ELE 25, a maior!");
    Lcd_Out(2,1,"Bem-vindo");
    Delay_ms(5000);
}

unsigned short selectBeverage() {
    unsigned short opcao = 0;

    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"Escolha o chopp:");

    while(1) {
        Lcd_Out(2,1,bebidas[opcao]);

        if (!BTN_NAVIGATE) {
            Delay_ms(300);
            while (!BTN_NAVIGATE);
            Delay_ms(300);
            opcao = (opcao + 1) % 4;
        }

        if (!BTN_SELECT) {
            Delay_ms(300);
            while (!BTN_SELECT);
            Delay_ms(300);
            return opcao;
        }
    }
}

unsigned short selectSize() {
    unsigned short opcao = 0;

    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"Tamanho bebida:");

    while(1) {
        Lcd_Out(2,1,tamanhos[opcao]);

        if (!BTN_NAVIGATE) {
            Delay_ms(300);
            while (!BTN_NAVIGATE);
            Delay_ms(300);
            opcao = (opcao + 1) % 3;
        }

        if (!BTN_SELECT) {
            Delay_ms(300);
            while (!BTN_SELECT);
            Delay_ms(300);
            return opcao;
        }
    }
}
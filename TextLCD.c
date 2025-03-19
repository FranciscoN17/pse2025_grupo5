// ---------------------------------------------------------------------------
// Definição dos bits de controle e dados do LCD (conforme a biblioteca mikroC)
// Ajuste conforme a sua conexão física.
// ---------------------------------------------------------------------------
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;

// ---------------------------------------------------------------------------
// Programa principal
// ---------------------------------------------------------------------------
void main() {
   // Desabilita comparadores para que os pinos sejam usados como digitais
   CMCON = 0x07;  // Para o PIC16F628A
   TRISA = 0;
   // Inicializa o LCD
   Lcd_Init();

   // Limpa o display
   Lcd_Cmd(_LCD_CLEAR);

   // Desativa cursor piscante/sublinhado (opções: _LCD_CURSOR_OFF, _LCD_CURSOR_ON, _LCD_BLINK_CURSOR_ON)
   Lcd_Cmd(_LCD_CURSOR_OFF);

   // Escreve "Hello World" na linha 1, coluna 1
   Lcd_Out(1, 1, "Parabens Chalegre");

   while(1) {
   PORTA = "0x00";
   delay_ms(500);
   PORTA = "0XFF";
   delay_ms(500);
      // Loop infinito
   }
}
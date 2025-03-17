; Codigo para Partida Estrela-Delta com Reversao
; P1 (saidas):
;   P1.0 - Contator principal (Motor): 1 = ligado, 0 = desligado
;   P1.1 - Contator Delta: 1 = conectado, 0 = desconectado
;   P1.2 - Contator Estrela: 1 = conectado, 0 = desconectado
;   P1.3 - Relay de Reversao (inverte sentido de rotacao)
; P2 (entradas):
;   P2.0 a P2.2 - DIP switches para definir tempo
;   P2.3      - Botao Start
;   P2.4      - Botao Reversao
ORG 0000h

MAIN:
    ACALL WaitStart       ; Aguarda botao Start (P2.3)
    ACALL ReadTime        ; Le DIP switches e define tempo (1 a 8 s)
    ACALL InitialConfig   ; Configuracao inicial

Countdown:
    ACALL Delay1s         ; Espera 1 segundo
    DEC R0                ; Decrementa o tempo
    JNZ Countdown         ; Enquanto R0 /= 0, repete a contagem

    ACALL StarDeltaTransition  ; Transicao de Estrela para Delta
    ACALL ReversalCheck        ; Se acionado, realiza reversao

    SJMP MAIN             ; Reinicia o processo
;------------------------------
; WaitStart: aguarda botao Start pressionado e liberado.
WaitStart:
WaitStart_Loop:
    JB P2.3, WaitStart_Loop
    ACALL Delay50ms         ; Delay para debounce
WaitRelease:
    JNB P2.3, WaitRelease
    RET
;------------------------------
; ReadTime: le DIP switches (P2.0 a P2.2) e define tempo (R0).
ReadTime:
    MOV A, P2
    ANL A, #07h
    JZ SetOne
    ADD A, #1
    MOV R0, A
    RET
SetOne:
    MOV R0, #1
    RET
;------------------------------
; InitialConfig: configura para partida em Estrela.
InitialConfig:
    SETB P1.2             ; Conecta Estrela (1 = conectado)
    CLR P1.1              ; Desconecta Delta (0 = desconectado)
    SETB P1.0             ; Liga o motor (1 = ligado)
    RET
;------------------------------
; Delay1s: 1 segundo (2 x 500ms).
Delay1s:
    ACALL Delay500ms
    ACALL Delay500ms
    RET
;------------------------------
; Delay500ms: delay de 500 ms com busywait.
Delay500ms:
    MOV R2, #0FAh         ; Contador externo
Delay500ms_Loop:
    MOV R3, #0F9h         ; Contador interno
Delay500ms_Inner:
    NOP                   ; Consome um ciclo.
    DJNZ R3, Delay500ms_Inner
    DJNZ R2, Delay500ms_Loop
    RET
;------------------------------
; Delay50ms: delay curto para debounce.
Delay50ms:
    MOV R2, #05h
Delay50ms_Loop:
    NOP
    DJNZ R2, Delay50ms_Loop
    RET
;------------------------------
; StarDeltaTransition: transicao de Estrela para Delta.
StarDeltaTransition:
    CLR P1.0              ; Desliga o motor (0 = desligado)
    CLR P1.2              ; Desconecta Estrela (0 = desconectado)
    SETB P1.1             ; Conecta Delta (1 = conectado)
    SETB P1.0             ; Religa o motor (1 = ligado)
    RET
;------------------------------
; ReversalCheck: verifica botao de reversao (P2.4).
ReversalCheck:
    JNB P2.4, EndReversal
    CLR P1.0              ; Desliga o motor
    ACALL Delay3s         ; Aguarda 3 s
    CPL P1.3              ; Inverte o sinal de reversao
    SETB P1.0             ; Religa o motor
EndReversal:
    RET
;------------------------------
; Delay3s: 3 segundos (3 x 1 s).
Delay3s:
    ACALL Delay1s
    ACALL Delay1s
    ACALL Delay1s
    RET

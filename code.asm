; Código Simples para Partida Estrela-Triângulo com Reversão
; Microcontrolador: AT89S51
; 
; P1 (saídas):
;   P1.0 - Contator principal (Motor): 0 = ligado, 1 = desligado
;   P1.1 - Contator Delta: 0 = conectado, 1 = desconectado
;   P1.2 - Contator Estrela: 0 = conectado, 1 = desconectado
;   P1.3 - Relé de Reversão (inverte sentido de rotação)
;
; P2 (entradas):
;   P2.0 a P2.2 - DIP switches para definir tempo (valor 0–7, somando 1 = 1–8 s)
;   P2.3      - Botão Start
;   P2.4      - Botão Reversão

ORG 0000h

MAIN:
    ACALL WaitStart       ; Aguarda o acionamento do botão Start (P2.3)
    ACALL ReadTime        ; Lê DIP switches e define o tempo (R0 = 1 a 8 s)
    ACALL InitialConfig   ; Configura: Estrela ativada, Delta desativado e motor ligado

Countdown:
    ACALL Delay1s         ; Espera 1 segundo
    DEC R0                ; Decrementa o tempo
    JNZ Countdown         ; Enquanto R0 ≠ 0, repete a contagem

    ACALL StarDeltaTransition  ; Transição de Estrela para Delta
    ACALL ReversalCheck        ; Se acionado, realiza reversão

    SJMP MAIN             ; Reinicia o processo

;------------------------------
; Sub-rotina: WaitStart
; Aguarda o botão Start (P2.3) ser pressionado e depois liberado.
WaitStart:
WaitStart_Loop:
    JB P2.3, WaitStart_Loop   ; Enquanto P2.3 = 1, aguarda
    ACALL Delay50ms           ; Pequeno delay para debounce
    ; Aguarda a liberação do botão
WaitRelease:
    JNB P2.3, WaitRelease     
    RET

;------------------------------
; Sub-rotina: ReadTime
; Lê os DIP switches (P2.0 a P2.2) e define o tempo em segundos (R0).
ReadTime:
    MOV A, P2               ; Lê todo o P2
    ANL A, #07h             ; AND lógico. Isola os bits P2.0 a P2.2
    JZ SetOne               ; Se for zero, define 1 s
    ADD A, #1               ; Ajusta: 0 vira 1, 7 vira 8
    MOV R0, A               ; Armazena o tempo em R0
    RET
SetOne:
    MOV R0, #1
    RET

;------------------------------
; Sub-rotina: InitialConfig
; Configura o sistema para partida em Estrela.
InitialConfig:
    CLR P1.2              ; Ativa Estrela (0 = ligado)
    SETB P1.1             ; Desativa Delta (1 = desligado)
    CLR P1.0              ; Liga o motor (0 = ligado)
    RET

;------------------------------
; Sub-rotina: Delay1s
; Cria um atraso de aproximadamente 1 segundo usando dois delays de 500 ms.
Delay1s:
    ACALL Delay500ms
    ACALL Delay500ms
    RET

;------------------------------
; Sub-rotina: Delay500ms
; Delay aproximado de 500 ms com laço de busy‑wait.
Delay500ms:
    MOV R2, #0FAh         ; Contador externo (250)
Delay500ms_Loop:
    MOV R3, #0F9h         ; Contador interno (249)
Delay500ms_Inner:
    NOP																							; Consome um ciclo.
    DJNZ R3, Delay500ms_Inner ; Decrementa R3, se não zerou, repete o Inner
    DJNZ R2, Delay500ms_Loop ; Decremente R2, se não zerou, repete o Loop
    RET

;------------------------------
; Sub-rotina: Delay50ms
; Delay curto para debounce (ajustado)
Delay50ms:
    MOV R2, #05h
Delay50ms_Loop:
    NOP
    DJNZ R2, Delay50ms_Loop
    RET

;------------------------------
; Sub-rotina: StarDeltaTransition
; Realiza a transição de Estrela para Delta.
StarDeltaTransition:
    SETB P1.0             ; Desliga o motor
    SETB P1.2             ; Desativa Estrela
    CLR P1.1              ; Ativa Delta
    CLR P1.0              ; Religa o motor
    RET

;------------------------------
; Sub-rotina: ReversalCheck
; Verifica se o botão de reversão (P2.4) foi pressionado.
; Se sim, desliga o motor, aguarda 3 s, inverte a rotação e religa o motor.
ReversalCheck:
    JNB P2.4, EndReversal ; Se o botão não for pressionado, não faz nada
    SETB P1.0             ; Desliga o motor
    ACALL Delay3s         ; Aguarda 3 segundos
    CPL P1.3              ; Inverte o sinal de reversão (muda o sentido)
    CLR P1.0              ; Religa o motor
EndReversal:
    RET

;------------------------------
; Sub-rotina: Delay3s
; Atraso de aproximadamente 3 segundos (3 x 1 s).
Delay3s:
    ACALL Delay1s
    ACALL Delay1s
    ACALL Delay1s
    RET

END
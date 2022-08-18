# Yahtzee-em-Anel
UFPR - Redes I - T3  Rede em Anel para o jogo Yahtzee

```txt
Mensagens:

Sequencialização > 
Detecção de erros > Paridade horizontal

Marcador inicial | Tipo | Origem | Tipo Aposta | Aposta | Resultado | Erros |
  1byte            1byte    1byte                                        1byte

-> A -> B -> C -> D ->
  
Tipo:
  -> Configuração: Inicio / Fim de jogo 
  -> Bastão
  -> Aposta
  -> Jogada
  
 TODO:
  - Mandar / Receber a mensagem
  - Passar o bastão
  - Implementar regras iniciais do jogo
  Qua:
    - Regras
    - Alterar
```
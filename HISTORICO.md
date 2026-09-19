# Histórico do projeto — Umidade Relé

Última atualização: 2026-09-19.

## Preferência de comunicação sobre alterações — 2026-09-18

- O usuário solicitou explicitamente que, ao alterar código, o assistente diga onde fez a alteração, em vez de apenas comunicar que alterou. Registrar arquivo e linha, mostrar antes/depois e explicar brevemente o motivo.
- Preferência também registrada em `AGENTS.md` para orientar as próximas sessões deste projeto.
- Localização conferida da correção anterior: `src/main.cpp`, linha 22, de `DHT dht(sensor, DHT11);` para `DHT dht(sensor, DHT22);`, para corresponder ao sensor DHT22 confirmado pelo usuário. Nenhuma nova alteração de firmware nesta etapa.

## Objetivo deste registro

Preservar o contexto das conversas e do diagnóstico de travamento. Consultar este arquivo antes de repetir perguntas ou propor alterações. Este registro cobre apenas o histórico disponível na sessão em que foi criado; não recupera conversas anteriores ausentes.

## Fatos confirmados pelo usuário

- O projeto apresenta um problema descrito como travamento.
- Na última ocorrência relatada, o LED estava desligado e o relé ficou ligado.
- A fonte é de 12 V e está conectada ao conector redondo de alimentação do Arduino (jack).
- O módulo relé está conectado à alimentação e ao GND do Arduino, descritos pelo usuário como “VCC e GND”. O pino exato de alimentação não foi documentado por foto ou esquema nesta sessão.
- O relé é de estado sólido (SSR), conforme correção explícita do usuário. Não tratar como relé eletromecânico: não há bobina de acionamento nem contatos mecânicos nesse SSR. Modelo, tipo de saída e proteções internas ainda desconhecidos.
- O relé aciona um pequeno exaustor de 100 mm, confirmado pelo usuário após a criação deste histórico. A tensão, a potência e o modelo do exaustor ainda não foram informados no contexto disponível.
- Um capacitor cerâmico foi recomendado anteriormente. A recomendação original e seu ponto de instalação não estão disponíveis. Nesta conversa, o assistente mencionou 100 nF / marcação 104 de forma condicional; isso não confirma o valor originalmente indicado nem que tenha sido instalado.

## Código consultado em 2026-09-15

Arquivos: `src/main.cpp`, `lib/PiscaLed/PiscaLed.cpp` e `platformio.ini`. Conferir os arquivos novamente se houver alterações posteriores.

- PlatformIO configurado para Arduino Uno (`board = uno`, plataforma `atmelavr`).
- DHT11 no pino 10; comando do relé no pino 8.
- LED no pino 13, configurado para 125 ms ligado e 2000 ms apagado.
- `led.atualizar()` é chamado no início de cada execução do `loop()` e usa `millis()`.
- Serial a 9600 baud; imprime umidade e `digitalRead(rele)` a cada segundo enquanto o fluxo do programa avança normalmente.
- Primeira leitura do sensor é antecipada pelo ajuste de `ultimaLeitura` no `setup()`.
- Quando a umidade é maior que 13, escreve HIGH no pino do relé e define intervalo de leitura de 120000 ms.
- Caso contrário, escreve LOW e define intervalo de 1000 ms.
- A média de dez posições é calculada e impressa, mas a decisão do relé usa a leitura atual. O vetor começa zerado.
- Não há tratamento explícito de leitura inválida (`NaN`) nem configuração de watchdog em `main.cpp`.
- Não foi confirmado se o módulo SSR é ativo em HIGH ou em LOW. Não confundir nível lógico de comando com condução na saída de potência.

## Interpretações e limites

- Se o LED realmente deixou de piscar por vários ciclos, isso é compatível com interrupção do fluxo normal do programa. Vê-lo apagado apenas por um instante é normal: ele passa aproximadamente 94% do ciclo apagado.
- Um programa parado pode deixar a saída no último nível, enquanto o microcontrolador segue alimentado e sem reset. Isso é compatível com o relato, mas não comprova a causa do travamento.
- O intervalo de 120 segundos usa `millis()` e não deveria, por si só, bloquear a atualização do LED.
- O valor mostrado por `digitalRead(rele)` informa o nível do pino, não confirma condução na saída de potência do SSR.
- Se a alimentação do módulo vier do pino 5 V, com 12 V entrando pelo jack, a corrente do módulo também passa pelo regulador do Arduino. Isso não prova sobrecarga ou aquecimento excessivo.
- Ruído elétrico, instabilidade da alimentação, aquecimento do regulador e bloqueio de execução foram discutidos como hipóteses. Nenhuma foi comprovada.
- Um capacitor de desacoplamento pode ajudar com ruído rápido; não é solução confirmada e não corrige sozinho sobrecarga ou bloqueio de software.
- Correção após identificação do SSR: argumentos sobre bobina, arco ou contatos colados de relé eletromecânico não se aplicam. Não estimar o consumo do módulo a partir de uma bobina inexistente nem atribuir sobrecarga ao regulador sem conhecer o consumo real.
- Um SSR ainda pode sofrer transientes ao comutar motor. Para SSRs AC com triac/tiristor, há condições em que a comutação causa acionamento indevido; a proteção depende do modelo, da carga e dos componentes internos. Isso não comprova que o Arduino travou por interferência.
- Distinguir execução parada no Arduino de saída do SSR conduzindo indevidamente: observar o pisca do LED, o Serial e o comando. Nenhum desses testes foi relatado como executado.

## Testes sugeridos, ainda sem resultado informado

1. Observar se as mensagens do Monitor Serial também param durante a falha.
2. Desconectar a fonte de 12 V do jack e testar a alimentação pela USB com uma fonte de 5 V confiável, mantendo o restante igual e observando por mais tempo que o intervalo habitual até a falha.
   - Se ficar estável, aumenta a suspeita sobre o caminho de alimentação anterior; não distingue sozinho fonte, regulador e efeitos da mudança de ligação.
   - Se continuar falhando, investigar também a carga acionada, interferência e o ponto em que a execução deixa de avançar.
3. Comparar o funcionamento com o exaustor desconectado, mantendo Arduino e módulo relé funcionando e observando as comutações. Fazer qualquer alteração de ligação com tudo fora da tomada e manter os terminais isolados antes de religar.
   - Se a falha ocorrer apenas com o exaustor conectado, isso reforça a associação com a carga/comutação; não prova sozinho o mecanismo da falha.
   - Para interferência causada pela comutação de uma carga AC, avaliar um supressor adequado somente após identificar o SSR, suas proteções internas e a carga. Snubber RC e varistor têm funções distintas; a necessidade e o dimensionamento dependem do circuito. Não foi especificada peça nem ligação.

Não há confirmação de que esses testes tenham sido realizados. Não há medição de tensão, corrente ou temperatura registrada.

## Informações ainda ausentes deste registro

- Tensão de alimentação (AC ou DC), potência e modelo do exaustor de 100 mm.
- Modelo do módulo SSR, tipo de saída (AC/DC), tensão nominal, polaridade de acionamento, proteções internas e esquema completo das ligações.
- Relação temporal entre a falha e a comutação do relé; tempo habitual até falhar.
- Comportamento do Serial durante a falha e se houve reinicialização.
- Valor e local da recomendação original do capacitor; se foi instalado e com qual resultado.

## Registro da conversa — 2026-09-15

- O usuário perguntou se LED apagado com relé ligado ajudava no diagnóstico.
- O assistente consultou o código e explicou a dependência do LED em relação ao `loop()` e a possibilidade de retenção do estado da saída.
- O usuário perguntou se o capacitor anteriormente recomendado resolveria.
- O usuário esclareceu sucessivamente a fonte de 12 V, o relé alimentado pelo Arduino e o uso do jack redondo.
- O assistente insistiu na hipótese de alimentação sem evidência suficiente e repetiu perguntas; reconheceu esse problema. Evitar repetir esse padrão.
- Ao ser perguntado sobre a carga, o usuário lembrou que já havia informado isso. O assistente reconheceu que a informação não estava no contexto disponível.
- O usuário solicitou um histórico nesta pasta para consulta futura.
- Foram criados este histórico e uma instrução local de consulta em `AGENTS.md`. Nenhum código ou ligação foi alterado nesta etapa.
- Em seguida, o usuário esclareceu que a carga é um pequeno exaustor de 100 mm. O resumo foi atualizado, resolvendo a ausência da identificação do aparelho. Ainda falta identificar sua tensão para avaliar a proteção adequada contra interferência; nenhum componente de proteção foi dimensionado ou instalado nesta etapa.
- O usuário voltou a perguntar o que resolveria além do capacitor. Foi consultada a documentação da Omron sobre interferência na comutação de cargas e supressores. Foi proposto comparar com e sem o exaustor conectado. Supressão da carga, correção de alimentação e correção de software continuam sendo caminhos condicionais à causa, sem diagnóstico confirmado ou teste executado.
- O usuário lembrou que o relé é de estado sólido. O assistente reconheceu que vinha tratando indevidamente o componente como eletromecânico. Foram corrigidas as referências a contatos e a identificação do módulo no resumo, e consultadas referências específicas de SSR. As referências anteriores de relés eletromecânicos ficam apenas como registro da discussão, não como especificação para este hardware.

## Referências consultadas

- [Arduino — formas de alimentação e regulador](https://support.arduino.cc/hc/en-us/articles/360018922259-What-power-supply-can-I-use-with-my-Arduino-board)
- [Microchip — AVR040, interferência eletromagnética e desacoplamento](https://www.microchip.com/en-us/application-notes/an1619-2)
- [Omron — precauções de relés e interferência em microcontroladores](https://www.ia.omron.com/product/cautions/36/safety_precautions.html)
- [Omron — supressores para cargas AC e DC](https://www.ia.omron.com/support/faq/answer/36/faq02804/)
- [Omron — transientes e acionamento indevido em SSRs com cargas indutivas](https://www.ia.omron.com/support/faq/answer/18/faq02088/)
- [Omron — proteção contra surtos e proteções internas de SSRs](https://www.ia.omron.com/support/faq/answer/18/faq02075/)

## Registro da conversa — 2026-09-16

- Fato relatado: o usuário encontrou tudo úmido dentro da caixa do projeto e suspeita de ter perdido a placa e os demais componentes.
- A umidade passa a ser uma hipótese prioritária para investigar a falha. Não foi confirmado se houve condensação, infiltração, curto, corrosão ou dano permanente; não há confirmação de perda da placa, do SSR ou do sensor.
- Orientação nesta conversa: manter todas as alimentações desligadas, inclusive USB e alimentação da carga; deixar secar em local seco e ventilado, sem calor forte, e inspecionar antes de qualquer tentativa de energização. Avaliar os componentes separadamente após secagem e inspeção. Nenhum desses procedimentos foi relatado como executado.
- Os testes energizados sugeridos em 2026-09-15 devem aguardar a avaliação da umidade e das condições do hardware. Nenhum código foi alterado.
- Referência consultada: [Omron — condições de instalação de controladores, umidade e condensação](https://www.ia.omron.com/data_pdf/guide/454/plc_installation_tg_e_1_1_connecting_plc_cg_e_5_1.pdf), que descreve a possibilidade de curtos e mau funcionamento por condensação; isso não comprova o mecanismo ocorrido neste projeto.

### DHT com leitura constante e componente fotografado — 2026-09-16

- O usuário relatou que o DHT só indica um valor e suspeita de dano por umidade. Dano permanente e sua causa continuam sem confirmação.
- A foto mostra um componente pequeno de três terminais; a inscrição não está legível o suficiente para identificar o modelo. LM35 e DS18B20 foram mencionados apenas como exemplos de sensores de temperatura em formato semelhante, não como identificação. Esses modelos não medem umidade e não substituem a função de umidade do DHT11. Solicitada a inscrição ou foto mais próxima para identificação.
- Nova consulta a `src/main.cpp`: o sensor agora está no pino 7 e o relé no pino 6, diferentemente dos pinos 10 e 8 observados em 2026-09-15. Essas alterações já estavam presentes; não foram feitas pelo assistente nesta etapa.
- O código atual imprime `ultimaUmidade` a cada segundo, mas, quando a leitura é maior que 13, só consulta o DHT a cada 120000 ms (2 minutos). Portanto, repetição no Serial durante esse intervalo não confirma defeito do sensor. Não há resultado de teste independente registrado.
- Consultadas as fichas dos fabricantes: [TI LM35](https://www.ti.com/lit/ds/symlink/lm35.pdf) e [Analog Devices DS18B20](https://www.analog.com/en/products/ds18b20.html). Nenhum código foi alterado.

## Leitura de 25,5 no DHT — 2026-09-18

- O usuário relata leitura constante de 25,5 e confirma que o DHT está ligado no pino 7. Não especificou se o número aparece na linha `Umidade` ou na linha `Media ate o momento`.
- Código novamente consultado: `src/main.cpp` configura `DHT dht(sensor, DHT11)` com `sensor = 7` e usa `readHumidity()`, não leitura de temperatura. O modelo físico ainda precisa corresponder ao DHT11 configurado.
- Confirmado no código: a linha `Umidade` repete o valor armazenado a cada segundo; com uma leitura de 25,5 (maior que 13), uma nova consulta ao sensor só ocorre a cada 120 segundos. A linha `Media ate o momento` é impressa quando ocorre essa consulta e calcula uma média de dez posições, inicialmente zeradas.
- A repetição durante esses dois minutos não confirma defeito. O histórico de umidade no hardware mantém dano como hipótese, sem diagnóstico confirmado. Não houve teste físico, compilação ou alteração de firmware nesta etapa.
- Verificação sugerida: comparar a linha `Umidade` após novas consultas, identificadas pela impressão da linha de média. Ainda sem resultado informado.

## Código de teste rápido do DHT — 2026-09-18

- A pedido do usuário, `src/main.cpp` foi substituído por um teste de DHT no pino 7, mantendo o tipo configurado como DHT11. O tipo físico ainda não foi confirmado; para DHT22, alterar `TIPO_DHT`.
- O teste aguarda 2 segundos antes de cada consulta, imprime número da tentativa, umidade em % e temperatura em Celsius no Serial a 9600 baud, e informa falhas (`NaN`). Não calcula médias nem controla o relé.
- Código anterior preservado em `main-antes-teste-dht.cpp.bak`. Durante a edição, uma nova leitura do arquivo mostrou o limite do relé alterado de 13 para 15; essa versão mais recente também foi preservada em `main-antes-teste-dht-limite15.cpp.bak` antes da substituição.
- Compilação `platformio run` para Uno concluída com sucesso: 223 bytes de RAM e 5410 bytes de flash. Não houve upload nem teste físico do sensor.
- Orientação para o teste: usar hardware seco e manter relé/exaustor desconectados, fazendo alterações de ligação com a alimentação desligada. A polaridade do SSR permanece desconhecida; ausência de controle no firmware não garante saída de potência desligada.

## Manutenção do registro

### Parar monitoramento — 2026-09-18

- O usuário relata que, ao clicar em “parar monitoramento”, “ele não faz mais nada”. Ainda não foi esclarecido se isso significa apenas ausência de mensagens, interface sem resposta ou parada percebida no Arduino.
- Conferido `src/main.cpp`: permanece o teste do DHT, com consultas a cada 2 segundos e Serial a 9600 baud; não há comando de parada recebido pela Serial. Não foi confirmado se essa versão foi enviada à placa.
- Orientação: interromper o monitor encerra a visualização das mensagens; para voltar a acompanhar pelo PlatformIO, reabrir o Monitor Serial. Isso, por si só, não comprova travamento do firmware. Nenhuma alteração de código, upload ou teste físico foi realizado nesta etapa.
- Correção do usuário na mesma data: a queixa é no Serial Monitor. A resposta anterior sobre o funcionamento do Arduino não esclareceu o problema relatado. Ainda é necessário identificar o estado do botão após o clique; não há diagnóstico confirmado de falha da interface.

Acrescentar entradas datadas com novas informações, alterações e resultados reais de testes. Separar fatos relatados, observações do código e hipóteses. Quando uma informação for corrigida, atualizar o resumo e registrar a correção. Não registrar sugestões como ações executadas.

## Restauração do código anterior — 2026-09-18

- A pedido do usuário, `src/main.cpp` foi restaurado integralmente a partir de `main-antes-teste-dht-limite15.cpp.bak`, a versão mais recente preservada antes do teste do DHT.
- Restaurados o controle do relé no pino 6, DHT11 no pino 7, LED no pino 13, média de dez leituras e limite `umidade > 15`, com intervalos de 120 segundos acima do limite e 1 segundo nos demais casos.
- Compilação PlatformIO para Uno concluída com sucesso: 334 bytes de RAM e 5662 bytes de flash. Não houve upload nem teste físico.
## Teste periódico do relé e DHT — 2026-09-18

- A pedido do usuário, `src/main.cpp` agora alterna o comando do relé no pino 6 e consulta umidade e temperatura do DHT11 no pino 7 a cada 2000 ms, usando `millis()`. A primeira alternância e leitura ocorrem após 2 segundos. O LED no pino 13 continua piscando.
- O relé alterna independentemente da leitura do sensor, inclusive em caso de `NaN`. O Serial a 9600 baud mostra o nível HIGH/LOW do comando e as leituras ou mensagem de falha.
- A constante `RELE_LIGADO` assume HIGH e pode ser alterada para LOW. A polaridade física do SSR continua sem confirmação; o nível do pino não comprova a condução na saída de potência.
- Antes desta alteração, o arquivo tinha limite `umidade > 17`, uma mudança já presente desde a restauração registrada acima. Essa versão foi preservada em `main-antes-teste-rele-dht-limite17.cpp.bak`.
- Compilação PlatformIO para Uno concluída com sucesso: 239 bytes de RAM e 5722 bytes de flash. Não houve upload nem teste físico.

## Restauração após teste periódico — 2026-09-18

- A pedido do usuário, `src/main.cpp` foi restaurado integralmente do backup `main-antes-teste-rele-dht-limite17.cpp.bak`, correspondente ao código que estava presente antes do teste de alternância a cada 2 segundos. A igualdade com o backup foi verificada por SHA-256.
- Voltaram o DHT11 no pino 7, relé no pino 6, LED no pino 13, média de dez leituras e controle pela condição `umidade > 17`, com intervalos de 120 segundos acima do limite e 1 segundo nos demais casos.
- Correção de precisão do registro anterior: o limite 17 foi observado antes de criar o teste periódico; a restauração anterior havia usado limite 15. Não foi observado o momento exato da alteração de 15 para 17.
- Compilação PlatformIO para Uno concluída com sucesso: 334 bytes de RAM e 5662 bytes de flash. Não houve upload nem teste físico.

## Verificação de proteção contra travamento — 2026-09-18

- Consultados `src/main.cpp`, `lib/PiscaLed/PiscaLed.cpp`, `platformio.ini` e o código local da biblioteca DHT. O firmware do projeto não configura watchdog nem recuperação automática de travamento.
- A temporização com `millis()` mantém o fluxo do loop durante os intervalos entre leituras; não é um mecanismo de reinicialização. O LED indica atividade, mas não recupera a execução.
- A biblioteca DHT instalada possui limite de espera em `expectPulse()` e retorna falha quando esse limite é excedido. Essa proteção é específica da espera pelos pulsos do sensor, não de todo o programa.
- O código principal restaurado permanece sem tratamento explícito de `NaN`. Nenhum código foi alterado; não houve compilação, upload ou teste físico nesta consulta.

## Explicação de watchdog aplicável — 2026-09-18

- Explicado como proposta o uso do watchdog interno do ATmega328P/Uno, por meio de `<avr/wdt.h>`, com prazo de exemplo de aproximadamente 8 segundos e renovação por `wdt_reset()` ao fim de cada passagem completa pelo `loop()`, fora dos blocos temporizados.
- O intervalo atual de 120 segundos entre leituras não impede essa renovação, pois usa `millis()` e o loop continua executando. Leituras constantes ou inválidas não são necessariamente detectadas pelo watchdog se o loop continuar avançando.
- A implementação deve cuidar da inicialização após reset pelo watchdog e do comando inicial do relé. Trata-se de recuperação por reinicialização, sem diagnóstico ou correção da causa de eventual falha elétrica.
- Referência consultada: https://avrdudes.github.io/avr-libc/avr-libc-user-manual/group__avr__watchdog.html . Nenhum firmware foi alterado, compilado ou enviado nesta etapa.

## Watchdog implementado — 2026-09-18

- O usuário autorizou adicionar somente o watchdog. Em `src/main.cpp`, incluído `<avr/wdt.h>`, ativação com `wdt_enable(WDTO_8S)` ao fim do `setup()` e renovação com `wdt_reset()` ao fim de cada passagem pelo `loop()`, fora dos blocos temporizados.
- Incluída rotina de inicialização `.init3` que limpa `MCUSR` e desativa o watchdog antes da inicialização normal da aplicação, para tratar o estado após reset. A presença e sequência dessa rotina foram verificadas no binário com `avr-objdump`.
- Preservadas a lógica do sensor, relé e LED, os pinos, o limite 17, as médias e os intervalos. Nenhuma outra funcionalidade foi adicionada ao firmware.
- Compilação PlatformIO para Uno concluída com sucesso: 334 bytes de RAM e 5708 bytes de flash. Não houve upload nem teste físico de reset. O bootloader efetivamente gravado na placa não foi verificado.

## Ponto de instalação do capacitor — 2026-09-18

- O usuário perguntou onde instalar o capacitor cerâmico mencionado anteriormente. O ponto da recomendação original não consta no histórico disponível; não foi recuperado nem confirmado.
- Orientação dada agora, condicionada a ser um capacitor de 100 nF (104) para desacoplamento e o módulo SSR ser alimentado em 5 V: ligar entre VCC (5 V) e GND do módulo, próximo desses terminais e com pernas curtas, no lado de baixa tensão. Capacitor cerâmico não tem polaridade. Não ligar esse capacitor comum aos terminais de potência do SSR nem à rede elétrica. Fazer a ligação com a alimentação desligada.
- Trata-se de sugestão de desacoplamento, sem confirmação de instalação ou eficácia contra a falha. Nenhum firmware foi alterado.

## Valor 3,5 ainda sem identificação — 2026-09-18

- O usuário relatou que está mostrando apenas 3,5, sem identificar se é a linha de umidade, a média no Serial ou uma medição elétrica. Origem e unidade ainda não confirmadas.
- Consultado `src/main.cpp`: a média continua dividindo por dez desde a primeira amostra, com vetor inicialmente zerado. Exemplo condicional: primeira leitura de 35 gera média de 3,5; não foi confirmado que esse é o caso relatado.
- Observado no código atual `int leituraUmidade = 15`, usado na comparação do relé. Isso difere do limite 17 registrado antes; a alteração já estava presente e não foi feita pelo assistente nesta consulta. Watchdog permanece configurado.
- Solicitada identificação de onde aparece o valor. Nenhuma alteração de firmware ou teste físico realizado.

## Salto de 3,10 para 27,50 no Serial — 2026-09-18

- Após responder “ambos” à pergunta anterior, o usuário enviou uma imagem que confirma `Umidade: 3.10 | Rele: 0`, com médias próximas de 3,2, seguida de média 5,60 e repetições de `Umidade: 27.50 | Rele: 1`. Relata atualização após algum tempo.
- Correção da hipótese anterior: os valores baixos aparecem na leitura direta, portanto não são explicados apenas pela média inicialmente preenchida com zeros.
- Código atual confirmado: acima de 15, o comando vai para HIGH e a próxima consulta fica para 120 segundos depois; o Serial repete o valor armazenado a cada segundo. Isso explica a repetição de 27,50, mas não a origem do salto.
- Consultada a implementação local de `DHT::readHumidity()`: DHT11 usa `data[0] + data[1] * 0.1`; DHT22 usa a palavra de 16 bits multiplicada por 0,1. Hipótese específica a verificar: sensor físico DHT22/AM2302 configurado como DHT11. Como exemplo matemático, bytes de uma leitura DHT22 de 76,9 seriam interpretados como 3,1 no modo DHT11; bytes de 76,7 seriam interpretados como 27,5. A coincidência com a imagem é uma pista, não identificação confirmada do sensor nem medição confirmada de 76,9/76,7%.
- Solicitada confirmação do modelo escrito no sensor antes de mudar a configuração. A biblioteca também reutiliza a leitura anterior em consultas com menos de 2000 ms. Nenhum firmware foi alterado ou enviado, nem teste físico realizado nesta etapa.

## Modelo DHT22 confirmado e configuração corrigida — 2026-09-18

- O usuário confirmou que o último sensor comprado é DHT22, no contexto do sensor atualmente em teste. Isso resolve a identificação pendente do modelo; as referências anteriores a DHT11 descreviam a configuração do firmware, não uma confirmação do hardware.
- Corrigida apenas a declaração em `src/main.cpp`, de `DHT dht(sensor, DHT11)` para `DHT dht(sensor, DHT22)`. Mantidos pino 7, lógica do relé, limite 15, intervalos, média e watchdog.
- A incompatibilidade de configuração está confirmada pelo relato e código. Ela explica matematicamente o padrão 3,10/27,50 observado; a leitura física correta e a resolução após envio ainda dependem de teste na placa.
- Compilação PlatformIO para Uno concluída com sucesso: 334 bytes de RAM e 5708 bytes de flash. Não houve upload nem teste físico. Próximo passo: enviar o firmware corrigido e observar a linha `Umidade`; acima de 15, permanece a consulta a cada 120 segundos.

## Unidade da leitura de umidade — 2026-09-18

- Esclarecido que `readHumidity()` retorna umidade relativa em porcentagem: por exemplo, 76,9 representa 76,9% UR, sem necessidade de multiplicar por 100. O limite atual 15 também representa 15% UR.
- A correção DHT11 para DHT22 ajusta a interpretação dos dados, não a unidade. Seu efeito na placa depende do envio do firmware corrigido, ainda sem confirmação registrada. Nenhum código alterado nesta consulta.

## NaN após reiniciar — 2026-09-19

- O usuário relata `nan` após reiniciar; a imagem mostra repetidamente `Umidade: nan | Rele: 0` e `Media ate o momento: nan`. Tipo e causa da reinicialização não foram confirmados.
- Consultados `src/main.cpp`, `platformio.ini` e a biblioteca DHT instalada. Configuração atual: DHT22 no pino 7, relé no pino 6 e limite 80. O limite 80 já estava presente; não foi alterado pelo assistente.
- Confirmado no código: linha 31 força leitura imediata após setup; linhas 48–54 guardam a leitura sem validar `isnan()` e a incluem na média. A biblioteca retorna NaN quando a aquisição falha. Uma amostra NaN mantém a média inválida até ser substituída no vetor; isso não causa, por si só, novas falhas da leitura direta.
- Com NaN, a comparação da linha 57 é falsa, o comando do relé fica LOW e o intervalo passa a 1000 ms. A biblioteca limita novas aquisições a 2000 ms e reutiliza o resultado anterior entre elas, inclusive falha; portanto, o intervalo de 1 segundo não comprova a causa da persistência.
- A leitura imediata é uma hipótese para a falha inicial, não diagnóstico da persistência. Reiniciar o microcontrolador pelo watchdog não corta a alimentação do sensor; o watchdog é renovado enquanto o loop avança mesmo com NaN.
- Propostos aguardar 2 segundos antes da primeira consulta, usar pelo menos 2 segundos entre tentativas e validar NaN antes de atualizar leitura/média ou tomar decisões pela umidade. Persistência exige investigar comunicação/alimentação do sensor; nenhuma causa física foi confirmada.
- Referência consultada: https://raw.githubusercontent.com/adafruit/DHT-sensor-library/master/DHT.cpp . Nenhum firmware alterado, compilado ou enviado; nenhum teste físico realizado.

## Leitura volta ao mexer nos fios — 2026-09-19

- O usuário relata que a leitura retorna ao mexer nos fios. Ainda não identificou quais fios nem se apenas movimenta ou desconecta/reconecta.
- Esse relato reforça mau contato em conectores, cabos ou soldas como hipótese prioritária, sem confirmar o ponto defeituoso. Se houver interrupção de VCC/GND, a recuperação também pode decorrer de reinicialização do sensor; não confundir as duas situações.
- Orientado revisar encaixes e substituir os cabos do DHT, um por vez e com a alimentação desligada. Como houve relato anterior de umidade na caixa, verificar sinais de oxidação; corrosão continua sem confirmação.
- Consultado o guia da Adafruit sobre ligação do DHT: https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor . O guia recomenda pull-up de 10 kΩ entre DATA e VCC; presença de resistor no sensor/módulo atual ainda desconhecida. Conferência do pull-up é sugestão secundária caso os contatos estejam firmes.
- Nenhum firmware alterado ou teste físico executado pelo assistente.

## Resistor em série com LED — 2026-09-19

- O usuário perguntou em qual perna do LED deve ficar o resistor. Orientado que pode ficar no lado do ânodo ou do cátodo, desde que em série; resistor comum não tem polaridade. Para LED comum com pernas não cortadas, a longa normalmente é o ânodo e a curta é o cátodo.
- Não foram confirmados modelo do LED, valor do resistor ou montagem realizada. Nenhum firmware alterado.

## Reinício após cinco leituras NaN consecutivas — 2026-09-19

- O usuário primeiro solicitou explicação e código na conversa, e corrigiu explicitamente a intenção do assistente de editar sem solicitação. Perguntas como “como fazer?” devem ser respondidas com instruções; não autorizam implementar. Nenhum arquivo foi alterado na tentativa interrompida. Depois de receber os trechos, o usuário autorizou: “ok,, adicione no código”.
- Implementados os três trechos apresentados em `src/main.cpp`: contador global `falhasConsecutivas`, incremento em leitura NaN e zeragem em leitura válida; na quinta falha consecutiva, mensagem Serial, `Serial.flush()`, `wdt_enable(WDTO_15MS)` e espera sem renovar o watchdog para provocar reset.
- Intervalo do ramo abaixo/igual ao limite ou NaN alterado de 1000 para 2000 ms, para não contar resultado reutilizado pela biblioteca DHT como nova aquisição. Mantidos intervalo de 120 segundos acima do limite, leitura inicial imediata e cálculo da média existente, que ainda admite NaN.
- Código observado antes da edição já usava LED no pino 10, DHT22 no pino 7, relé no pino 6 e limite 80; esses valores foram preservados. `platformio.ini` já estava sem o `+` solto visto em uma consulta anterior; não foi alterado nesta implementação.
- Compilação `platformio run` para Uno concluída com sucesso: 376 bytes de RAM e 5986 bytes de flash. Não houve upload nem teste físico do reset ou confirmação de recuperação do sensor. O reset do Arduino não corta a alimentação do DHT; a causa da falha permanece não confirmada.

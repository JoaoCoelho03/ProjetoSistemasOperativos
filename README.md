# 📡 Projeto de Sistemas Operativos — Monitorização da Qualidade do Ar

Este projeto foi desenvolvido no âmbito da unidade curricular de **Sistemas Operativos**, com o objetivo de simular um sistema de **monitorização da qualidade do ar** em salas de aula. A implementação recorre a **programação concorrente e paralela em C**, utilizando técnicas como **multiprocessamento**, **multithreading**, **comunicação interprocessos** e **sincronização com semáforos**.

---

## ✅ Objetivos do Projeto

### Fases Implementadas:

- **Fase 1A-C — Multiprocessamento com Pipes**  
  Utilização de `fork()` e `pipe()` para criar processos filhos que processam ficheiros `.csv` com dados de sensores. Os resultados são enviados ao processo pai, que gera o ficheiro `relatorio.txt`.

- **Fase 1D — Barra de Progresso com Pipes**  
  A barra de progresso é implementada **no processo pai**, que a atualiza com base nos dados recebidos pelos `pipes`, à medida que os processos filhos terminam. Esta barra é atualizada dinamicamente no terminal. **Não são usadas threads nesta fase.**

- **Fase 1E — Comunicação com Unix Domain Sockets**  
  Substituição dos pipes por **Unix Domain Sockets**, mantendo a lógica de multiprocessamento.

- **Fase 2A — Multithreading com Memória Partilhada**  
  Uso de `pthread` para processar ficheiros concorrentes. Os resultados são armazenados em memória partilhada e gravados em `relatorio_threads.txt`.

- **Fase 2B — Barra de Progresso com Threads**  
  Implementada com sincronização por `pthread_mutex` e variável partilhada `sensores_concluidos`.

- **Fase 2C — Sistema Produtor-Consumidor com Semáforos**  
  Utilização de múltiplas threads produtoras e consumidoras com sincronização por `sem_t` e `pthread_mutex`. Os dados processados são guardados no ficheiro `relatorio_prodcons.txt`.

---

## 📂 Estrutura do Projeto

```
├── data/                        # Ficheiros CSV de sensores
├── main.c                      # Versão com pipes entre processos
├── main_socket.c              # Versão com Unix Domain Sockets
├── main_threads.c             # Versão com threads e memória partilhada
├── main_prodcons.c            # Sistema produtor-consumidor
├── process_sensor.c           # Lógica de processamento com fork()
├── process_sensor_threads.c   # Lógica de processamento com threads
├── process_sensor_prodcons.c  # Lógica com produtor/consumidor
├── relatorio.txt              # Saída da versão com pipes
├── relatorio_sockets.txt      # Saída da versão com sockets
├── relatorio_threads.txt      # Saída da versão com threads
├── relatorio_prodcons.txt     # Saída da versão com semáforos
└── README.md                  # Este ficheiro
```

---

## ▶️ Como Executar

### 1. Clonar o repositório:

```bash
git clone https://github.com/JoaoCoelho03/ProjetoSistemasOperativos.git
cd ProjetoSistemasOperativos
```

### 2. Compilar:

```bash
make
```

> Ou usar manualmente:  
> `gcc -o main main.c -lpthread` (e variantes consoante o ficheiro)

### 3. Executar:

- **Fase 1A-C (Pipes):**
  ```bash
  ./main data/
  ```

- **Fase 1E (Sockets):**
  ```bash
  ./main_socket data/
  ```

- **Fase 2A-B (Threads):**
  ```bash
  ./main_threads data/
  ```

- **Fase 2C (Produtor-Consumidor):**
  ```bash
  ./main_prodcons
  ```

---

## 📊 Barra de Progresso

Em `main`, `main_socket` e `main_threads`, uma barra de progresso é exibida dinamicamente no terminal:

```
Progresso: [========>      ] 60%
```

---

## 🧵 Sistema Produtor-Consumidor

- **2 produtoras** simulam sensores fictícios (20 sensores).
- **2 consumidoras** processam os dados e escrevem no relatório.
- Saída no ficheiro `relatorio_prodcons.txt`, com o formato:

```
SensorID;Média;HorasFora
Sensor_0;77.00;5.00
Sensor_1;86.00;2.00
...
```

---

## ⏱ Medição de Desempenho

Ao final da execução do `main_prodcons`, o tempo total de execução é apresentado:

```
Tempo total de execução: 0.721000 segundos
```

---

## 📄 Licença

Este projeto está licenciado sob a **Licença MIT**. Ver ficheiro `LICENSE`.

---

## 👨‍🏫 Agradecimentos

- **Professores:** Pedro Sobral e João Viana  
- **Universidade Fernando Pessoa – Engenharia Informática**  
- Pela orientação e feedback contínuo ao longo do projeto.
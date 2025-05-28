# Projeto de Sistemas Operativos - Monitorização da Qualidade do Ar

Este projeto foi desenvolvido no âmbito da disciplina de **Sistemas Operativos**, com o objetivo de simular um sistema de **monitorização da qualidade do ar** em salas de aula. O sistema utiliza **programação concorrente e paralela em C**, explorando técnicas como **multiprocessamento**, **multithreading**, **comunicação inter-processos** e **sincronização com semáforos**.

---

## ✅ Objetivos do Projeto

### Fases Implementadas:

- **Fase 1A-C - Multiprocessamento com Pipes**  
  Utilização de **`fork()`** e **`pipe()`** para comunicação entre processos. Cada processo filho processa um ficheiro `.csv` com dados de sensores e envia o resultado ao processo pai, que gera o `relatorio.txt`.

- **Fase 1D - Barra de Progresso com Pipes**  
  Implementada com `pthread` no processo pai, atualiza dinamicamente conforme os sensores são processados.

- **Fase 1E - Comunicação com Unix Domain Sockets**  
  Substituição de pipes por **Unix Domain Sockets**, mantendo a mesma lógica de processamento.

- **Fase 2A - Multithreading com Memória Partilhada**  
  Cada thread processa um ficheiro de forma concorrente. Os dados são armazenados num array partilhado, gerando o `relatorio_threads.txt`.

- **Fase 2B - Barra de Progresso com Threads**  
  A barra de progresso foi implementada e sincronizada corretamente com a variável partilhada `sensores_concluidos`.

- **Fase 2C - Sistema Produtor-Consumidor com Semáforos**  
  Implementado com múltiplas threads produtoras e consumidoras, sincronizadas com `sem_t` e `pthread_mutex`.  
  Os resultados são guardados no ficheiro `relatorio_prodcons.txt`, ordenados e formatados.

---

## 📂 Estrutura do Projeto

```
├── data/                        # Ficheiros CSV dos sensores
├── main.c                      # Pipes entre processos
├── main_socket.c              # Comunicação com Unix Domain Sockets
├── main_threads.c             # Versão com threads e memória partilhada
├── main_prodcons.c            # Sistema produtor-consumidor com semáforos
├── process_sensor.c           # Processamento com fork() + pipes
├── process_sensor_threads.c   # Processamento com pthreads
├── process_sensor_prodcons.c  # Processamento com produtor/consumidor
├── relatorio.txt              # Resultado da versão com pipes
├── relatorio_sockets.txt      # Resultado da versão com sockets
├── relatorio_threads.txt      # Resultado da versão com threads
├── relatorio_prodcons.txt     # Resultado da versão com semáforos
└── README.md                  # Este ficheiro
```

---

## ▶️ Como Executar

### 1. Clone o repositório:

```bash
git clone https://github.com/JoaoCoelho03/ProjetoSistemasOperativos.git
cd ProjetoSistemasOperativos
```

### 2. Compilar:

```bash
make  # ou gcc manual com -lpthread, dependendo do ficheiro
```

### 3. Executar:

#### 📌 Pipes (Fase 1A-C)

```bash
./main data/
```

#### 📌 Unix Domain Sockets (Fase 1E)

```bash
./main_socket data/
```

#### 📌 Threads (Fase 2A-B)

```bash
./main_threads data/
```

#### 📌 Produtor-Consumidor com Semáforos (Fase 2C)

```bash
./main_prodcons
```

---

## 📊 Barra de Progresso

Em `main`, `main_socket` e `main_threads`, a barra de progresso aparece no terminal em tempo real:

```
Progresso: [=====>     ] 50%
```

---

## 🧵 Sistema Produtor-Consumidor

- **2 produtoras** geram 20 sensores fictícios.
- **2 consumidoras** processam e guardam os resultados.
- Dados são ordenados e guardados em `relatorio_prodcons.txt` no seguinte formato:

```
SensorID;Média;HorasFora
Sensor_0;77.00;5.00
Sensor_1;86.00;2.00
...
```

---

## ⏱ Medição de Desempenho

O tempo de execução é exibido automaticamente ao final da versão `main_prodcons`:

```
Tempo total de execução: 0.721000 segundos
```

---

## 📄 Licença

Este projeto está licenciado sob a **Licença MIT**. Ver `LICENSE`.

---

## 👨‍🏫 Agradecimentos

- **Professores**: Pedro Sobral e João Viana  
- **Universidade Fernando Pessoa – Engenharia Informática**  
- Orientação e feedback ao longo do desenvolvimento
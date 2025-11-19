# Sistema de Impressão Térmica Elgin

## 📖 Descrição

- Este sistema é uma aplicação em C para controle de impressoras térmicas Elgin. O programa oferece uma interface de linha de comando para configurar, conectar e utilizar diversas funcionalidades das impressoras Elgin.
- Feito por: Dimitri Diaz, Daniel Alencar, Arthur Emiliano, Kauã Felix, Gabriel

## 🎯 Funcionalidades

### ✅ Configuração e Conexão
- Configuração de tipo, modelo e conexão da impressora
- Abertura e fechamento de conexão
- Suporte a USB

### 🖨️ Impressão
- **Texto**: Impressão com formatação (posição, estilo, tamanho)
- **QR Code**: Geração e impressão de códigos QR
- **Código de Barras**: Impressão de códigos de barras (CODE 128)
- **XML SAT**: Impressão de DANFE SAT
- **XML Cancelamento SAT**: Impressão de DANFE de cancelamento

### 🔧 Controles
- **Gaveta**: Abertura de gaveta (Elgin e personalizada)
- **Papel**: Avanço e corte de papel
- **Som**: Emissão de sinais sonoros

## 🚀 Como Usar

### 1. Pré-requisitos
- Windows (DLL específica do Windows)
- Compilador C (DEV C++, Visual Studio, etc.)
- DLL `E1_Impressora01.dll` no diretório do executável
- Impressora Elgin conectada

### 2. Fluxo de Uso
1. **Configure a conexão** (Opção 1)
2. **Abra a conexão** (Opção 2)
3. **Use as funcionalidades** (Opções 3-12)
4. **Saia do sistema** (Opção 0)

## 📋 Menu Principal

```
=== MENU PRINCIPAL ===
-----------------------
1  - Configurar Conexao
2  - Abrir Conexao
3  - Impressao Texto
4  - Impressao QRCode
5  - Impressao Cod Barras
6  - Impressao XML SAT
7  - Impressao XML Canc SAT
8  - Abrir Gaveta Elgin
9  - Abrir Gaveta
10 - Sinal Sonoro
11 - Avanco de Papel
12 - Corte de Papel
0  - Fechar Conexão e Sair
```

📄 Funcionalidades
Impressão
Texto: Posição, estilo, tamanho personalizáveis
QR Code: Dados via teclado, tamanho automático
Código de Barras: CODE 128, dados fixos de exemplo
XML SAT: Lê XMLSAT.xml, imprime DANFE
XML Cancelamento: Lê CANC_SAT.xml, usa assinatura padrão

Controles
Gaveta: Elgin (padrão) ou personalizada
Papel: Avanço 1-10 linhas, corte 0-5 linhas
Som: 3 beeps padrão

## 🔍 Códigos de Erro Comuns

| Código | Descrição |
|--------|-----------|
| 0 | Sucesso |
| -1 | Erro genérico |
| -2 | Parâmetros inválidos |
| -3 | Dispositivo não encontrado |
| -4 | Erro de comunicação |
| -5 | Hardware incompatível |

## ⚠️ Observações Importantes

### Arquivos XML
- Coloque os arquivos `XMLSAT.xml` e `CANC_SAT.xml` no diretório do executável
- Verifique se os XMLs estão no formato correto da SEFAZ SP

### Conexões
- **USB**: Deixe parâmetro vazio para auto-detecção

## 📄 Licença

Este projeto é um exemplo educacional para uso com impressoras Elgin.

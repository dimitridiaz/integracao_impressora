#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ======================= Config DLL ======================= */
static HMODULE g_hDll = NULL;

/* Convenção de chamada (Windows): __stdcall */
#ifndef CALLCONV
#define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
typedef int(CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int(CALLCONV *FechaConexaoImpressora_t)(void);
typedef int(CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int(CALLCONV *Corte_t)(int);
typedef int(CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int(CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int(CALLCONV *AvancaPapel_t)(int);
typedef int(CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int(CALLCONV *AbreGaveta_t)(int, int, int);
typedef int(CALLCONV *SinalSonoro_t)(int, int, int);
typedef int(CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int(CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int(CALLCONV *InicializaImpressora_t)(void);

/* ======================= Ponteiros ======================= */
static AbreConexaoImpressora_t AbreConexaoImpressora = NULL;
static FechaConexaoImpressora_t FechaConexaoImpressora = NULL;
static ImpressaoTexto_t ImpressaoTexto = NULL;
static Corte_t Corte = NULL;
static ImpressaoQRCode_t ImpressaoQRCode = NULL;
static ImpressaoCodigoBarras_t ImpressaoCodigoBarras = NULL;
static AvancaPapel_t AvancaPapel = NULL;
static AbreGavetaElgin_t AbreGavetaElgin = NULL;
static AbreGaveta_t AbreGaveta = NULL;
static SinalSonoro_t SinalSonoro = NULL;
static ImprimeXMLSAT_t ImprimeXMLSAT = NULL;
static ImprimeXMLCancelamentoSAT_t ImprimeXMLCancelamentoSAT = NULL;
static InicializaImpressora_t InicializaImpressora = NULL;

/* ======================= Configuração ======================= */
static int g_tipo = 1;
static char g_modelo[64] = "i9";
static char g_conexao[128] = "USB";
static int g_parametro = 0;
static int g_conectada = 0;

/* ======================= Utilidades ======================= */
#define LOAD_FN(h, name)                                                 \
    do                                                                   \
    {                                                                    \
        name = (name##_t)GetProcAddress((HMODULE)(h), #name);            \
        if (!(name))                                                     \
        {                                                                \
            fprintf(stderr, "Falha ao resolver símbolo %s (erro=%lu)\n", \
                    #name, GetLastError());                              \
            return 0;                                                    \
        }                                                                \
    } while (0)

static void flush_entrada(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

/* ======================= Funções para manipular a DLL ======================= */
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll)
    {
        fprintf(stderr, "Erro ao carregar E1_Impressora01.dll (erro=%lu)\n", GetLastError());
        return 0;
    }

    LOAD_FN(g_hDll, AbreConexaoImpressora);
    LOAD_FN(g_hDll, FechaConexaoImpressora);
    LOAD_FN(g_hDll, ImpressaoTexto);
    LOAD_FN(g_hDll, Corte);
    LOAD_FN(g_hDll, ImpressaoQRCode);
    LOAD_FN(g_hDll, ImpressaoCodigoBarras);
    LOAD_FN(g_hDll, AvancaPapel);
    LOAD_FN(g_hDll, AbreGavetaElgin);
    LOAD_FN(g_hDll, AbreGaveta);
    LOAD_FN(g_hDll, SinalSonoro);
    LOAD_FN(g_hDll, ImprimeXMLSAT);
    LOAD_FN(g_hDll, ImprimeXMLCancelamentoSAT);
    LOAD_FN(g_hDll, InicializaImpressora);

    return 1;
}

static void liberarBiblioteca(void)
{
    if (g_hDll)
    {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ======================= Funções a serem implementadas pelos alunos ======================= */

static void exibirMenu(void)
{
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("Status: %s\n", g_conectada ? "✓ CONECTADO" : "✗ DESCONECTADO");
    printf("-----------------------\n");
    printf("1  - Configurar Conexao\n");
    printf("2  - Abrir Conexao\n");
    printf("3  - Impressao Texto\n");
    printf("4  - Impressao QRCode\n");
    printf("5  - Impressao Cod Barras\n");
    printf("6  - Impressao XML SAT\n");
    printf("7  - Impressao XML Canc SAT\n");
    printf("8  - Abrir Gaveta Elgin\n");
    printf("9  - Abrir Gaveta\n");
    printf("10 - Sinal Sonoro\n");
    printf("11 - Avanco de Papel\n");
    printf("12 - Corte de Papel\n");
    printf("0  - Sair\n");
    printf("-----------------------\n");
    printf("Opcao: ");
}

static void configurarConexao(void)
{
    printf("\n=== CONFIGURACAO ===\n");

    printf("Tipo (1-Termica, 2-Matricial): ");
    scanf("%d", &g_tipo);
    if (g_tipo < 1 || g_tipo > 2)
        g_tipo = 1;

    printf("Modelo (1-i7, 2-i8, 3-i9): ");
    scanf("%d", &g_modelo);
    if (g_modelo < 1 || g_modelo > 3)
        g_modelo = 3;

    printf("Conexao (1-USB, 2-Serial, 3-Ethernet): ");
    scanf("%d", &g_conexao);
    if (g_conexao < 1 || g_conexao > 3)
        g_conexao = 1;

    flush_entrada();
    printf("Parametro (ex: COM1, 192.168.1.100:9100): ");
    fgets(g_parametro, sizeof(g_parametro), stdin);
    g_parametro[strcspn(g_parametro, "\n")] = '\0';

    printf("✓ Configurado: Tipo %d, Modelo %d, Conexao %d\n",
           g_tipo, g_modelo, g_conexao);
    printf("Parametro: %s\n", g_parametro);

    getchar();
}

static void Corte(void)
{
    printf("\n=== CORTE DO PAPEL ===\n");

    // Verifica se há conexão ativa
    if (!g_conectada)
    {
        printf("✗ Erro: Impressora nao conectada.\n");
        printf("Pressione ENTER para continuar...");
        getchar();
        return;
    }

    int avanco;

    printf("Quantas linhas avancar antes do corte? (0-5): ");
    scanf("%d", &avanco);

    // Validação simples
    if (avanco < 0 || avanco > 5)
    {
        avanco = 0;
        printf("Valor invalido. Usando 0 (corte imediato).\n");
    }

    // Executa o corte
    int resultado = Corte(avanco);

    if (resultado == 0)
    {
        printf("✓ Corte realizado com sucesso!\n");
    }
    else
    {
        printf("✗ Erro no corte. Codigo: %d\n", resultado);
        if (resultado == -3)
        {
            printf("  (Impressora pode nao ter guilhotina)\n");
        }
    }

    printf("Pressione ENTER para continuar...");
    flush_entrada();
    getchar();
}

static void AvancaPapel(void)
{
    printf("\n=== AVANCO DE PAPEL ===\n");

    if (!g_conectada)
    {
        printf("✗ Sem conexao!\n");
        getchar();
        return;
    }

    int linhas;

    printf("Linhas para avancar: ");
    scanf("%d", &linhas);

    if (linhas < 1 || linhas > 10)
    {
        linhas = 3;
        printf("Usando 3 linhas.\n");
    }

    int resultado = AvancaPapel(linhas);

    printf(resultado == 0 ? "✓ Papel avancado!\n" : "✗ Erro: %d\n", resultado);

    getchar();
}

static void abrirConexao(void)
{
    printf("\n=== ABRINDO CONEXAO COM A IMPRESSORA ===\n");
    printf("Tipo: %d | Modelo: %s | Conexao: %s | Parametro: %d\n",
           g_tipo, g_modelo, g_conexao, g_parametro);

    // Chama a função AbreConexaoImpressora da DLL
    int resultado = AbreConexaoImpressora(g_tipo, g_modelo, g_conexao, g_parametro);

    // Valida o retorno da função
    if (resultado == 0)
    {
        printf("✓ Conexao estabelecida com sucesso!\n");
        g_conectada = 1; // Marca como conectada
    }
    else
    {
        printf("✗ Erro ao abrir conexao. Codigo de erro: %d\n", resultado);
        g_conectada = 0; // Marca como desconectada

        // Tratamento específico para alguns códigos de erro conhecidos
        switch (resultado)
        {
        case -5:
            printf("  Hardware diferente do esperado (conexao negada)\n");
            break;
        case -1:
            printf("  Erro generico de conexao\n");
            break;
        case -2:
            printf("  Parametros invalidos\n");
            break;
        case -3:
            printf("  Dispositivo nao encontrado\n");
            break;
        case -4:
            printf("  Erro de comunicacao\n");
            break;
        default:
            printf("  Consulte a documentacao para mais detalhes sobre o erro %d\n", resultado);
            break;
        }
    }

    printf("Pressione ENTER para continuar...");
    getchar();
}

static void fecharConexao(void)
{
    printf("\n=== FECHANDO CONEXAO COM A IMPRESSORA ===\n");

    // Verifica se há uma conexão ativa antes de tentar fechar
    if (!g_conectada)
    {
        printf("⚠ Nenhuma conexao ativa para fechar.\n");
        printf("Pressione ENTER para continuar...");
        getchar();
        return;
    }

    printf("Fechando conexao com impressora %s...\n", g_modelo);

    // Chama a função FechaConexaoImpressora da DLL
    int resultado = FechaConexaoImpressora();

    // Valida o retorno da função
    if (resultado == 0)
    {
        printf("✓ Conexao fechada com sucesso!\n");
        g_conectada = 0; // Marca como desconectada
    }
    else
    {
        printf("✗ Erro ao fechar conexao. Codigo de erro: %d\n", resultado);

        // Tratamento específico para alguns códigos de erro conhecidos
        switch (resultado)
        {
        case -1:
            printf("  Erro generico ao fechar conexao\n");
            break;
        case -2:
            printf("  Conexao ja estava fechada\n");
            g_conectada = 0; // Atualiza status mesmo com erro
            break;
        case -3:
            printf("  Erro de comunicacao durante fechamento\n");
            break;
        case -4:
            printf("  Timeout ao fechar conexao\n");
            g_conectada = 0; // Considera desconectada após timeout
            break;
        default:
            printf("  Consulte a documentacao para mais detalhes sobre o erro %d\n", resultado);
            // Em caso de erro desconhecido, assume que a conexão foi perdida
            g_conectada = 0;
            break;
        }
    }

    printf("Status da conexao: %s\n", g_conectada ? "CONECTADA" : "DESCONECTADA");
    printf("Pressione ENTER para continuar...");
    getchar();
}

static void imprimirTexto(void)
{
    printf("\n=== IMPRESSAO DE TEXTO ===\n");

    // Verifica se há uma conexão ativa
    if (!g_conectada)
    {
        printf("✗ Erro: Nenhuma conexao ativa com a impressora.\n");
        printf("Abra uma conexao primeiro.\n");
        printf("Pressione ENTER para continuar...");
        getchar();
        return;
    }

    char texto[512];
    int posicao, stilo, tamanho;

    // Limpa o buffer de entrada
    flush_entrada();

    // Solicita o texto ao usuário
    printf("Digite o texto a ser impresso (max 500 caracteres):\n");
    if (fgets(texto, sizeof(texto), stdin) == NULL)
    {
        printf("✗ Erro ao ler o texto.\n");
        return;
    }

    // Remove a quebra de linha do final, se existir
    size_t len = strlen(texto);
    if (len > 0 && texto[len - 1] == '\n')
    {
        texto[len - 1] = '\0';
    }

    // Solicita a posição
    printf("\nEscolha a posicao do texto:\n");
    printf("0 - Esquerda\n");
    printf("1 - Centro\n");
    printf("2 - Direita\n");
    printf("Posicao: ");
    scanf("%d", &posicao);

    // Valida posição
    if (posicao < 0 || posicao > 2)
    {
        printf("⚠ Posicao invalida. Usando posicao esquerda (0).\n");
        posicao = 0;
    }

    // Solicita o estilo
    printf("\nEscolha o estilo do texto (pode somar valores):\n");
    printf("0  - Fonte A (normal)\n");
    printf("1  - Fonte B\n");
    printf("2  - Sublinhado\n");
    printf("4  - Modo reverso\n");
    printf("8  - Negrito\n");
    printf("Exemplo: 10 = Sublinhado(2) + Negrito(8)\n");
    printf("Estilo: ");
    scanf("%d", &stilo);

    // Valida estilo (aceita valores de 0 a 15)
    if (stilo < 0 || stilo > 15)
    {
        printf("⚠ Estilo invalido. Usando fonte normal (0).\n");
        stilo = 0;
    }

    // Solicita o tamanho
    printf("\nEscolha o tamanho do texto:\n");
    printf("=== ALTURA ===\n");
    printf("0 - 1x | 1 - 2x | 2 - 3x | 3 - 4x\n");
    printf("4 - 5x | 5 - 6x | 6 - 7x | 7 - 8x\n");
    printf("=== LARGURA ===\n");
    printf("16 - 2x | 32 - 3x | 48 - 4x | 64 - 5x\n");
    printf("80 - 6x | 96 - 7x | 112 - 8x\n");
    printf("Para largura+altura, some os valores (ex: 17 = 2x altura + 2x largura)\n");
    printf("Tamanho: ");
    scanf("%d", &tamanho);

    // Valida tamanho básico
    if (tamanho < 0)
    {
        printf("⚠ Tamanho invalido. Usando tamanho normal (0).\n");
        tamanho = 0;
    }

    printf("\n--- CONFIGURACAO ---\n");
    printf("Texto: " % s "\n", texto);
    printf("Posicao: %s\n", (posicao == 0) ? "Esquerda" : (posicao == 1) ? "Centro"
                                                                         : "Direita");
    printf("Estilo: %d", stilo);
    if (stilo > 0)
    {
        printf(" (");
        if (stilo & 1)
            printf("Fonte B ");
        if (stilo & 2)
            printf("Sublinhado ");
        if (stilo & 4)
            printf("Reverso ");
        if (stilo & 8)
            printf("Negrito ");
        printf(")");
    }
    printf("\nTamanho: %d\n", tamanho);

    printf("\nEnviando para impressora...\n");

    // Chama a função ImpressaoTexto da DLL
    int resultado = ImpressaoTexto(texto, posicao, stilo, tamanho);

    // Valida o retorno da função
    if (resultado > 0)
    {
        printf("✓ Texto enviado com sucesso!\n");
        printf("Bytes escritos no buffer: %d\n", resultado);

        // Adiciona avanço de papel e corte no final
        printf("Finalizando impressao...\n");
        AvancaPapel(3); // Avança 3 linhas
        Corte(0);       // Corte total

        printf("✓ Impressao concluida!\n");
    }
    else
    {
        printf("✗ Erro ao enviar texto. Codigo de erro: %d\n", resultado);

        // Tratamento específico para alguns códigos de erro
        switch (resultado)
        {
        case 0:
            printf("  Nenhum dado foi escrito no buffer\n");
            break;
        case -1:
            printf("  Erro generico de escrita\n");
            break;
        case -2:
            printf("  Parametros invalidos\n");
            break;
        case -3:
            printf("  Buffer da impressora cheio\n");
            break;
        case -4:
            printf("  Erro de comunicacao\n");
            break;
        default:
            printf("  Consulte a documentacao para mais detalhes sobre o erro %d\n", resultado);
            break;
        }
    }

    printf("\nPressione ENTER para continuar...");
    flush_entrada();
    getchar();

    // incluir AvancaPapel e Corte no final
}

static void imprimirQRCode(void)
{
    char texto[50];
    printf("insira os dados do QRcode: ");
    scanf("%s", &texto);
    ImpressaoQRCode(texto, 6, 4);

    AvancaPapel(5);
    Corte(0);

    // TODO: solicitar conteúdo do QRCode e chamar ImpressaoQRCode(texto, 6, 4)
    // incluir AvancaPapel e Corte no final
}

// static void imprimirQRCode(void)
// {
//     printf("\n=== QRCODE ===\n");

//     if (!g_conectada) {
//         printf("✗ Sem conexao!\n");
//         getchar();
//         return;
//     }

//     char dados[256];

//     flush_entrada();
//     printf("Conteudo do QRCode: ");
//     fgets(dados, sizeof(dados), stdin);

//     // Remove quebra de linha
//     dados[strcspn(dados, "\n")] = '\0';

//     printf("Imprimindo...\n");

//     // Usa valores padrão: tamanho 4, correção 15%
//     int resultado = ImpressaoQRCode(dados, 4, 2);

//     if (resultado == 0) {
//         printf("✓ QRCode impresso!\n");
//         AvancaPapel(3);
//         Corte(0);
//     } else {
//         printf("✗ Erro: %d\n", resultado);
//     }

//     getchar();
// }

static void imprimirCodigoBarras(void)
{

    ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);

    AvancaPapel(5);
    Corte(0);

    // TODO: usar ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3)
    // incluir AvancaPapel e Corte no final
}

// static void imprimirCodigoBarras(void)
// {
//     printf("\n=== CODIGO DE BARRAS ===\n");

//     if (!g_conectada) {
//         printf("✗ Sem conexao!\n");
//         getchar();
//         return;
//     }

//     char dados[100];

//     flush_entrada();
//     printf("Dados do codigo: ");
//     fgets(dados, sizeof(dados), stdin);
//     dados[strcspn(dados, "\n")] = '\0';

//     printf("Imprimindo...\n");

//     // Usa CODE 128 com valores padrão
//     int resultado = ImpressaoCodigoBarras(8, dados, 100, 2, 2);

//     if (resultado == 0) {
//         printf("✓ Codigo impresso!\n");
//         AvancaPapel(3);
//         Corte(0);
//     } else {
//         printf("✗ Erro: %d\n", resultado);
//         if (resultado == -3) {
//             printf("  Tente: {A + seus dados\n");
//         }
//     }

//     getchar();
// }

static void imprimirXMLSAT(void)
{
    printf("\n=== DANFE SAT ===\n");

    if (!g_conectada)
    {
        printf("✗ Sem conexao!\n");
        getchar();
        return;
    }

    // Lê arquivo padrão
    FILE *arquivo = fopen("./XMLSAT.xml", "r");
    if (!arquivo)
    {
        printf("✗ Arquivo XMLSAT.xml nao encontrado!\n");
        getchar();
        return;
    }

    // Lê conteúdo
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    char *dados = malloc(tamanho + 1);
    if (!dados)
    {
        printf("✗ Erro de memoria!\n");
        fclose(arquivo);
        return;
    }

    fread(dados, 1, tamanho, arquivo);
    dados[tamanho] = '\0';
    fclose(arquivo);

    printf("Imprimindo DANFE...\n");

    // Imprime com layout padrão
    int resultado = ImprimeXMLSAT(dados, 0);

    if (resultado == 0)
    {
        printf("✓ DANFE impresso!\n");
        AvancaPapel(5);
        Corte(0);
    }
    else
    {
        printf("✗ Erro: %d\n", resultado);
    }

    free(dados);
    getchar();
}

static void imprimirXMLCancelamentoSAT(void)
{
    printf("\n=== CANCELAMENTO SAT ===\n");

    if (!g_conectada)
    {
        printf("✗ Sem conexao!\n");
        getchar();
        return;
    }

    // Assinatura padrão
    const char *assinatura =
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";

    // Lê arquivo padrão
    FILE *arquivo = fopen("./CANC_SAT.xml", "r");
    if (!arquivo)
    {
        printf("✗ Arquivo CANC_SAT.xml nao encontrado!\n");
        getchar();
        return;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    char *dados = malloc(tamanho + 1);
    if (!dados)
    {
        printf("✗ Erro de memoria!\n");
        fclose(arquivo);
        return;
    }

    fread(dados, 1, tamanho, arquivo);
    dados[tamanho] = '\0';
    fclose(arquivo);

    printf("Imprimindo cancelamento...\n");

    // Imprime com layout padrão
    int resultado = ImprimeXMLCancelamentoSAT(dados, assinatura, 0);

    if (resultado == 0)
    {
        printf("✓ Cancelamento impresso!\n");
        AvancaPapel(5);
        Corte(0);
    }
    else
    {
        printf("✗ Erro: %d\n", resultado);
    }

    free(dados);
    getchar();
}

static void abrirGavetaElginOpc(void)
{
    printf("\n=== GAVETA ELGIN ===\n");

    if (!g_conectada)
    {
        printf("✗ Sem conexao!\n");
        getchar();
        return;
    }

    printf("Abrindo gaveta...\n");

    int resultado = abrirGavetaElginOpc();

    if (resultado == 0)
    {
        printf("✓ Gaveta aberta!\n");
    }
    else
    {
        printf("✗ Erro: %d\n", resultado);
        if (resultado == -3)
        {
            printf("  (Dispositivo pode nao ter conector RJ-11)\n");
        }
    }

    getchar();
}

static void abrirGavetaOpc(void)
{

    printf("\n=== GAVETA ===\n");

    if (!g_conectada)
    {
        printf("✗ Sem conexao!\n");
        getchar();
        return;
    }

    printf("Abrindo gaveta com parametros padrao...\n");

    // Usa valores padrão: pino 1, tempos 50
    int resultado = AbreGaveta(1, 50, 50);

    if (resultado == 0)
    {
        printf("✓ Gaveta aberta!\n");
    }
    else
    {
        printf("✗ Erro: %d\n", resultado);
        if (resultado == -5)
        {
            printf("  (Dispositivo sem suporte a gaveta)\n");
        }
    }

    getchar();
}

static void emitirSinalSonoro(void)
{
    printf("\n=== SINAL SONORO ===\n");

    if (!g_conectada)
    {
        printf("✗ Sem conexao!\n");
        getchar();
        return;
    }

    printf("Emitindo beep...\n");

    // Usa valores padrão: 3 beeps, 500ms ativo, 300ms intervalo
    int resultado = SinalSonoro(3, 5, 3);

    if (resultado == 0)
    {
        printf("✓ Sinal emitido!\n");
    }
    else
    {
        printf("✗ Erro: %d\n", resultado);
        if (resultado == -5)
        {
            printf("  (Dispositivo pode nao ter buzzer)\n");
        }
    }

    getchar();
}

/* ======================= Função principal ======================= */
int main(void)
{
    if (!carregarFuncoes())
    {
        return 1;
    }

    int opcao = 0;

    printf("Sistema de Impressao Elgin - DLL Carregada!\n");
    printf("Pressione ENTER para continuar...");
    getchar();

    while (1)
    {
        exibirMenu();

        if (scanf("%d", &opcao) != 1)
        {
            flush_entrada();
            printf("\n✗ Entrada invalida! Digite apenas numeros.\n");
            printf("Pressione ENTER para continuar...");
            getchar();
            continue;
        }

        flush_entrada();

        switch (opcao)
        {
        case 1:
            configurarConexao();
            break;
        case 2:
            abrirConexao();
            break;
        case 3:
            imprimirTexto();
            break;
        case 4:
            imprimirQRCode();
            break;
        case 5:
            imprimirCodigoBarras();
            break;
        case 6:
            imprimirXMLSAT();
            break;
        case 7:
            imprimirXMLCancelamentoSAT();
            break;
        case 8:
            abrirGavetaElgin();
            break;
        case 9:
            abrirGaveta();
            break;
        case 10:
            emitirSinalSonoro();
            break;
        case 11:
            avancaPapel();
            break;
        case 12:
            cortePapel();
            break;
        case 0:
            return 0;
        default:
            printf("\n✗ Opcao invalida! Escolha entre 0 e 12.\n");
            printf("Pressione ENTER para continuar...");
            getchar();
            break;
        }
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ======================= Config DLL ======================= */
static HMODULE g_hDll = NULL;

/* Convenção de chamada (Windows): __stdcall */
#ifndef CALLCONV
#  define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
typedef int (CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (CALLCONV *FechaConexaoImpressora_t)(void);
typedef int (CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int (CALLCONV *Corte_t)(int);
typedef int (CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int (CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (CALLCONV *AvancaPapel_t)(int);
typedef int (CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int (CALLCONV *AbreGaveta_t)(int, int, int);
typedef int (CALLCONV *SinalSonoro_t)(int, int, int);
typedef int (CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int (CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (CALLCONV *InicializaImpressora_t)(void);

/* ======================= Ponteiros ======================= */
static AbreConexaoImpressora_t        AbreConexaoImpressora        = NULL;
static FechaConexaoImpressora_t       FechaConexaoImpressora       = NULL;
static ImpressaoTexto_t               ImpressaoTexto               = NULL;
static Corte_t                        Corte                        = NULL;
static ImpressaoQRCode_t              ImpressaoQRCode              = NULL;
static ImpressaoCodigoBarras_t        ImpressaoCodigoBarras        = NULL;
static AvancaPapel_t                  AvancaPapel                  = NULL;
static AbreGavetaElgin_t              AbreGavetaElgin              = NULL;
static AbreGaveta_t                   AbreGaveta                   = NULL;
static SinalSonoro_t                  SinalSonoro                  = NULL;
static ImprimeXMLSAT_t                ImprimeXMLSAT                = NULL;
static ImprimeXMLCancelamentoSAT_t    ImprimeXMLCancelamentoSAT    = NULL;
static InicializaImpressora_t         InicializaImpressora         = NULL;

/* ======================= Configuração ======================= */
static int   g_tipo      = 1;
static char  g_modelo[64] = "i9";
static char  g_conexao[128] = "USB";
static int   g_parametro = 0;
static int   g_conectada = 0;

/* ======================= Utilidades ======================= */
#define LOAD_FN(h, name)                                                         \
    do {                                                                         \
        name = (name##_t)GetProcAddress((HMODULE)(h), #name);                    \
        if (!(name)) {                                                           \
            fprintf(stderr, "Falha ao resolver símbolo %s (erro=%lu)\n",         \
                    #name, GetLastError());                                      \
            return 0;                                                            \
        }                                                                        \
    } while (0)

static void flush_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ======================= Funções para manipular a DLL ======================= */
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll) {
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
    if (g_hDll) {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ======================= Funções a serem implementadas pelos alunos ======================= */

static void exibirMenu(void)
{
    {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("Status: %s\n", g_conectada ? "? CONECTADO" : "? DESCONECTADO");
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
}

static void configurarGaveta(void)
{
    printf("\n===== Configurar Gaveta =====\n");

    printf("Pino da gaveta (1 ou 2): ");
    scanf("%d", &g_pinoGaveta);   flush_entrada();

    printf("Tempo ON (ms): ");
    scanf("%d", &g_tempoON);      flush_entrada();

    printf("Tempo OFF (ms): ");
    scanf("%d", &g_tempoOFF);     flush_entrada();

    printf("\nConfiguração salva com sucesso!\n");
    printf("Pino=%d | ON=%d ms | OFF=%d ms\n\n",
           g_pinoGaveta, g_tempoON, g_tempoOFF);
}


static void abrirConexao(void)
{
    // TODO: chamar AbreConexaoImpressora e validar retorno
}

static void fecharConexao(void)
{
    // TODO: chamar FechaConexaoImpressora e tratar retorno
}

static void imprimirTexto(void)
{
    // TODO: solicitar texto do usuário e chamar ImpressaoTexto
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
}

static void imprimirCodigoBarras(void)
{

ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);

AvancaPapel(5);
Corte(0);
}

static void imprimirXMLSAT(void)
{
    // TODO: ler o arquivo ./XMLSAT.xml e enviar via ImprimeXMLSAT
    // incluir AvancaPapel e Corte no final
}

static void imprimirXMLCancelamentoSAT(void)
{
    // TODO: ler o arquivo ./CANC_SAT.xml e chamar ImprimeXMLCancelamentoSAT
    // incluir AvancaPapel e Corte no final
    
	/*usar assinatura abaixo:
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";
        */
}

static void abrirGavetaElginOpc(void)
{
    AbreGavetaElgin(g_pinoGaveta, g_tempoON, g_tempoOFF);

    printf("\n>>> Gaveta ELGIN acionada!\n");
    printf("(pino=%d, on=%d, off=%d)\n\n",
           g_pinoGaveta, g_tempoON, g_tempoOFF);
}

static void abrirGavetaOpc(void)
{
    AbreGaveta(g_pinoGaveta, g_tempoON, g_tempoOFF);

    printf("\n>>> Gaveta GENÉRICA acionada!\n");
    printf("(pino=%d, on=%d, off=%d)\n\n",
           g_pinoGaveta, g_tempoON, g_tempoOFF);
}

static void emitirSinalSonoro(void)
{
    // TODO: chamar SinalSonoro(4, 50, 5)
}

/* ======================= Função principal ======================= */
int main(void)
{
    if (!carregarFuncoes())
    {
        return 1;
    }

    int opcao = 0;

    while (1)
    {
        exibirMenu();

        if (scanf("%d", &opcao) != 1)
        {
            flush_entrada();
            printf("\n Entrada invalida! Digite apenas numeros.\n");
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
            printf("\n? Opcao invalida! Escolha entre 0 e 12.\n");
            printf("Pressione ENTER para continuar...");
            getchar();
            break;
        }
    }

    return 0;
}


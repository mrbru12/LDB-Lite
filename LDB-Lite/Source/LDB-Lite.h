#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>

#define SINGLE_DEC_PFX '^'
#define ARRAY_DEC_PFX '>'
#define VAR_CONTENT_INIT '='
#define ARRAY_CONTENT_SEP '/'
#define LDBL_FILE_EXT ".ldbl"

/* EU DECIDI USAR O std::map PARA O LDB-Lite PRINCIPALMENTE PARA APRENDER MAIS SOBRE ELE E TESTÁ-LO, ELE TEM UMA EFICIÊNCIA
 * MUITO BOA, PARTICULARMENTE, QUANDO VOCÊ USA A CLASSE LDBL::Doc POIS ALI VOCÊ VAI MAPEAR TODO O ARQUIVO SOMENTE UMA VEZ
 * E VAI PODER USAR OS MÉTODOS QUANTAS VEZES QUISER SEM GERAR UMA GRANDE MUDANÇA NO MAP INICIAL. JÁ NO CASO DOS MÉTODOS
 * AVULSOS DE INPUT E OUTPUT VAI SER UM POUCO MAIS INEFICIENTE, POIS TODA A VEZ QUE VOCÊ INVOCAR O MÉTODO UM std::map NOVO
 * DO ARQUIVO VAI SER CRIADO E TODAS AS LINHAS DO ARQUIVO VÃO SER MAPEADAS. UMA DAS PRINCIPAIS VANTAGENS DO std::map, QUE
 * SERIA COMO UM DICIONÁRIO EM OUTRAS LINGUAGENS, É A POSSIBILIDADE DE EXTRAIR UM VALOR SEM PRECISAR ITERAR POR TODO O ARRAY,
 * SETANDO UMA KEY PARA ESSE DETERMINADO VALOR, ISSO OTIMIZA MUITO O PROCESSAMENTO NO CASO DO LDB-Lite, POIS APENAS COM O NOME
 * DA VARIÁVEL EM FORMATO std::string VOCÊ CONSEGUE ACESSAR O CONTEÚDO CORRESPONDENTE A ELA NO std::map!
*/

// <TODO>
// -> Fazer um sistema de exceptions e checar se a extensão dos arquivos é a mesma da padrão LDBL;
// -> Documentar muito melhor, principalmente na descrição dos métodos e funções;
// -> Refatorar e otimizar o código, principalmente fazendo inicialização de variáveis;
// -> Talvez começa a usa o camel case assim na nomeação: void ExemploLoko();
// </TODO>

namespace LDBL
{
    enum class VarMode { Single, Array };
     
    struct VarInfo // EM UM STRUCT AS SENTENÇAS SÃO PUBLICAS POR DEFAULT!
    {
        std::string name; // , value;
        std::vector<std::string> content;
        VarMode mode;

        VarInfo();
        VarInfo(std::string varName, std::vector<std::string> varContent, VarMode varMode = VarMode::Single);
    };

    namespace In
    {
		std::vector<std::string> listVars(std::vector<std::string>* lines);
        std::vector<std::string> listVars(std::string path);
		
        VarInfo getVar(std::vector<std::string>* lines, std::string name);
        VarInfo getVar(std::string path, std::string name);

        // std::string getSingle(std::string path, std::string name);

        // std::vector<std::string> getArray(std::string path, std::string name);
    };

    namespace Out
    {
        void setVar(std::vector<std::string>* lines, VarInfo info, bool overwrite = true);
        void setVar(std::string path, VarInfo info, bool overwrite = true);
        void setVar(std::string path, std::string name, std::string value, bool overwrite = true);
        void setVar(std::string path, std::string name, std::vector<std::string> content, bool overwrite = true);

        // void setSingle(std::string path, std::string name, std::string value, bool overwrite = true);

        // void setArray(std::string path, std::string name, std::vector<std::string> content, bool overwrite = true);

        void eraseVar(std::vector<std::string>* lines, std::string name);
        void eraseVar(std::string path, std::string name);
    };

    class Doc
    {
        std::ofstream outFileStream;
        std::vector<std::string>* lines;
        std::map<std::string, VarInfo>* varMap;

    public:
        Doc(std::string path);
        ~Doc();

		std::vector<std::string> listVars();
		
        VarInfo getVar(std::string name);

        // std::string getSingle(std::string name);

        // std::vector<std::string> getArray(std::string name);

        void setVar(VarInfo info, bool overwrite = true);
        void setVar(std::string name, std::string value, bool overwrite = true);
        void setVar(std::string name, std::vector<std::string> content, bool overwrite = true);

        // void setSingle(std::string name, std::string value, bool overwrite = true);

        // void setArray(std::string name, std::vector<std::string> content, bool overwrite = true);

        void eraseVar(std::string name);

        /* class In
        {
            In() {} // Construtor privado é como se fosse classe static no c#

        public:
            static std::vector<std::string> getVarNames();

            static VarInfo getVar(std::string name);

            static std::string getSingle(std::string name);

            static std::vector<std::string> getArray(std::string name);
        };

        class Out
        {
            Out() {}

        public:
            static void setVar(VarInfo info, bool overwrite = true);
            // void setVar(std::string name, std::string value, bool overwrite = true);
            // void setVar(std::string name, std::vector<std::string> content, bool overwrite = true);

            static void setSingle(std::string name, std::string value, bool overwrite = true);

            static void setArray(std::string name, std::vector<std::string> content, bool overwrite = true);

            static void eraseVar(std::string name);
        }; */
    };
}

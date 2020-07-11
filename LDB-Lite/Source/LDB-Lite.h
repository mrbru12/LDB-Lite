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

/* EU DECIDI USAR O std::map PARA O LDB-Lite PRINCIPALMENTE PARA APRENDER MAIS SOBRE ELE E TEST�-LO, ELE TEM UMA EFICI�NCIA
 * MUITO BOA, PARTICULARMENTE, QUANDO VOC� USA A CLASSE LDBL::Doc POIS ALI VOC� VAI MAPEAR TODO O ARQUIVO SOMENTE UMA VEZ
 * E VAI PODER USAR OS M�TODOS QUANTAS VEZES QUISER SEM GERAR UMA GRANDE MUDAN�A NO MAP INICIAL. J� NO CASO DOS M�TODOS
 * AVULSOS DE INPUT E OUTPUT VAI SER UM POUCO MAIS INEFICIENTE, POIS TODA A VEZ QUE VOC� INVOCAR O M�TODO UM std::map NOVO
 * DO ARQUIVO VAI SER CRIADO E TODAS AS LINHAS DO ARQUIVO V�O SER MAPEADAS. UMA DAS PRINCIPAIS VANTAGENS DO std::map, QUE
 * SERIA COMO UM DICION�RIO EM OUTRAS LINGUAGENS, � A POSSIBILIDADE DE EXTRAIR UM VALOR SEM PRECISAR ITERAR POR TODO O ARRAY,
 * SETANDO UMA KEY PARA ESSE DETERMINADO VALOR, ISSO OTIMIZA MUITO O PROCESSAMENTO NO CASO DO LDB-Lite, POIS APENAS COM O NOME
 * DA VARI�VEL EM FORMATO std::string VOC� CONSEGUE ACESSAR O CONTE�DO CORRESPONDENTE A ELA NO std::map!
*/

// <TODO>
// -> Fazer um sistema de exceptions e checar se a extens�o dos arquivos � a mesma da padr�o LDBL;
// -> Documentar muito melhor, principalmente na descri��o dos m�todos e fun��es;
// -> Refatorar e otimizar o c�digo, principalmente fazendo inicializa��o de vari�veis;
// -> Talvez come�a a usa o camel case assim na nomea��o: void ExemploLoko();
// </TODO>

namespace LDBL
{
    enum class VarMode { Single, Array };
     
    struct VarInfo // EM UM STRUCT AS SENTEN�AS S�O PUBLICAS POR DEFAULT!
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
            In() {} // Construtor privado � como se fosse classe static no c#

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

const char* keywordss[19] = {"new", "int", "text", "size", "subs", "locate", "insert", "override", "read", "write", "from", "to", "input", "output", "asText", "asString","prompt"};
const char* fonksiyonlar[7] = {"size", "subs", "locate", "insert", "override","asText", "asString"};
char* bufferlist[50] = {0};
char* gecicibufferlist[5] = {0};
char* bufferlValue[50] = {0};
int numTextVariables = 0;
int intValue[50] = {0};
const char* commands[4] = {"read","write","input","output"};
char* bufferCopy;

void performInput( char* ,  char* );
void performWrite( char* ,  char* );
int performRead(char* );
int performAsText( char* );
char* performAsString(char* );
char* performOverride( char* , int ,  char* );
char* performInsert( char* , int ,  char* );
int performLocate( char* ,  char* , int );
char* performSubs( char* , int , int );
void performSubtraction( char* ,  char* ,  char* );
void stringAddition( char* ,  char* ,  char* );
void performSub( char* ,  char* ,  char* );
void performAddition( char* ,  char* ,  char* );
int getBufferIndex(char*);
int isIdfValid(char*);
bool lex_find_keyword(char*);
bool isOperator(char*);
bool lex_find_combine_operator(char, char);

char fpeek(FILE *);
char* is_char_str(char);
char* is_two_char_str(char, char);

int main(){
    char ch, buffer[50] = {0};
    char buffer2[50] = {0};
    char buffer3[50] = {0};
    char buffer4[50] = {0};
    char* str_temp= NULL;
    char* str_temp2=  NULL;
    int i;
    int tekrar = 0;
    int index = 0;
    int index2 = 0;
    int index3 = 0;
    int index4 = 0;
    int sayi = 0;
    FILE* input_file, *output_file;
    input_file = fopen("myProg.tj", "r");
    output_file = fopen("code.lex", "w");
    // check if file is actually opened
    if(input_file == NULL || output_file == NULL){
        printf("ERROR WHILE OPENING THE FILE\n");
        exit(1);
    }
    // reading a file with a loop
    while((ch = (char)fgetc(input_file)) != EOF){
        // ýnt constants part
        bool flag = true;
        bool durum = true;
        bool durum2 = true;
        bool keyw_mu = false;
        bool string_mi = false;
        if((isalnum(ch) || ch == '_')){
            // ýdentifier or keyword part
            buffer[index++] = ch;
            if(isalnum(fpeek(input_file)) || fpeek(input_file) == '_'){
            }
            else{
                buffer[index] = '\0';
                index = 0;
                i = 0;
                // keyword method and writing
                if (lex_find_keyword(buffer)) {
                    fprintf(output_file, "Keyword(%s)\n", buffer);
                    if(strcmp(keywordss[0], buffer) == 0){   // keywords[0] == "new"
                        if(fpeek(input_file) == ' '){
                            fgetc(input_file);              // boþluk charýný ekarte eder.
                        }
                        while((ch = (char)fgetc(input_file)) != ' '){
                            buffer[index++] = ch;
                        }
                        buffer[index] = '\0';
                        index = 0;
                        if(strcmp(keywordss[1], buffer) == 0 || strcmp(keywordss[2], buffer) == 0 ){
                            fprintf(output_file, "Keyword(%s)\n", buffer);

                            while((ch = (char)fgetc(input_file)) != ';'){
                                buffer[index++] = ch;
                            }
                            buffer[index] = '\0';
                            index = 0;

                            fprintf(output_file, "Identifier(%s)\n", buffer);
                            if (ch == ';'){
                                fprintf(output_file, "EndOfLine\n");
                            }
                            bufferCopy = strdup(buffer);
                            bufferlist[tekrar++] = bufferCopy;
                            numTextVariables++;
                        }
                    }
                    for (int j = 0; j < 4; ++j) {
                        if(strcmp(commands[j],buffer) == 0){
                            if(fpeek(input_file) == ' '){
                                fgetc(input_file);              // boþluk charýný ekarte eder.
                            }
                            ch = (char)fgetc(input_file);
                            while(ch != ';' && ch != ' '){
                                buffer2[index2++] = ch;
                                ch = (char)fgetc(input_file);
                            }
                            buffer2[index2] = '\0';
                            index2 = 0;
                            fprintf(output_file, "Identifier(%s)\n", buffer2);
                            if (ch == ';')
                                fprintf(output_file, "EndOfLine\n");
                            if(strcmp(commands[3],buffer) != 0){
                                if(fpeek(input_file) != ';')
                                    ch = (char)fgetc(input_file);
                                while(ch != ';' && ch != ' '){
                                    buffer3[index3++] = ch;
                                    ch = (char)fgetc(input_file);
                                }
                                buffer3[index3] = '\0';
                                index3 = 0;
                                fprintf(output_file, "Keyword(%s)\n", buffer3);
                                if (ch == ';')
                                    fprintf(output_file, "EndOfLine\n");
                                if(fpeek(input_file) != ';')
                                    ch = (char)fgetc(input_file);
                                while(ch != ';' && ch != ' '){
                                    buffer4[index4++] = ch;
                                    ch = (char)fgetc(input_file);
                                }
                                buffer4[index4] = '\0';
                                index4 = 0;
                                fprintf(output_file, "Identifier(%s)\n", buffer4);
                                if (ch == ';')
                                    fprintf(output_file, "EndOfLine\n");
                            }
                            if(ch == ';')
                                fgetc(input_file);
                            char* bufferCopy2 = strdup(buffer2);
                            sayi = getBufferIndex(bufferCopy2);
                            int sayi2 = getBufferIndex(bufferCopy2);

                            if(strcmp(keywordss[8], buffer) == 0 && strcmp(keywordss[10],buffer3) == 0){              //read durumu için
                                performRead(buffer4);
                            }
                            if(strcmp(keywordss[9], buffer) == 0 && strcmp(keywordss[11],buffer3) == 0){              //write durumu için
                                performWrite(buffer2,buffer4);
                            }
                            if(strcmp(keywordss[12], buffer) == 0 && strcmp(keywordss[16],buffer3) == 0){             //input durumu için
                                performInput(buffer2,buffer4);
                            }
                            if(strcmp(keywordss[13], buffer) == 0){                                                   //output durumu için
                                if(sayi != -1 && intValue[sayi] != '\0' && bufferlist[sayi] != NULL){
                                    printf("%d", intValue[sayi]);
                                    printf("\n");
                                }
                                else if(sayi2 != -1 && bufferlValue[sayi2] != NULL && bufferlist[sayi2] != NULL){
                                    printf("%s", bufferlValue[sayi2]);
                                    printf("\n");
                                }
                                else
                                    printf("%s%s", buffer2, " tanimlanmadi!");
                            }
                        }
                    }
                }
                    // check if identifier is valid
                    // identifier method checking and writing
                else if((i = isIdfValid(buffer)) == 1){
                    bufferCopy = strdup(buffer);
                    gecicibufferlist[0] = bufferCopy;

                    fprintf(output_file, "Identifier(%s)\n", buffer);

                    if(fpeek(input_file) == ' '){
                        fgetc(input_file);
                    }
                }
                if(i == -1)
                    fprintf(output_file, "ERROR: <%s> IS TOO LONG\n", buffer);
            }
        }
        else{
            if(ch == '/' && fpeek(input_file) == '*'){
                fgetc(input_file);
                while((ch = (char)fgetc(input_file)) != EOF){
                    if(ch == '*' && fpeek(input_file) == '/'){
                        fgetc(input_file);
                        break;
                    }
                }
                // if the comment line is not closed error message is writing
                if(ch == EOF){
                    fprintf(output_file, "ERROR: COMMENT NOT CLOSED\n");
                }
            }
                // operator method and writing part
            else if(lex_find_combine_operator(ch, fpeek(input_file))){       // bunun içine girerse atama iþlemleri baþlayacak

                fprintf(output_file, "Operator(%s)\n", ":=");
                fgetc(input_file);                          // ch2 yi atlamak için
                while(fpeek(input_file) == ' '){
                    fgetc(input_file); // boþluk tuþu varsa atlayacak
                }
                while (fpeek(input_file) != ';') {
                    while(fpeek(input_file) == ' '){
                        fgetc(input_file);              // boþluðu atladý
                    }
                    if(fpeek(input_file) == '"'){
                        string_mi = true;
                        fgetc(input_file);
                        while (fpeek(input_file) != '"'){
                            durum = false;
                            ch = (char)fgetc(input_file);
                            buffer2[index2++] = ch;
                        }
                        buffer2[index2] = '\0';
                        index2 = 0;
                        fprintf(output_file, "StringConst(%s)\n", buffer2);
                        fgetc(input_file);
                    }

                    else if (fpeek(input_file) != '+' && fpeek(input_file) != '-'){
                        while(fpeek(input_file) != ';' && fpeek(input_file) != '+' && fpeek(input_file) != '-' && fpeek(input_file) != '('){
                            ch = (char)fgetc(input_file);
                            buffer2[index2++] = ch;
                            while(fpeek(input_file) == ' '){
                                fgetc(input_file);              // boþluðu atladý
                            }
                        }
                        buffer2[index2] = '\0';
                        index2 = 0;

                    }
                    char*  bufferCopy2 = strdup(buffer2);

                    if(fpeek(input_file) == '('){
                        fgetc(input_file);
                        while((fpeek(input_file)) != ')' && fpeek(input_file) != ',' && fpeek(input_file) != ' '){
                            ch = (char)fgetc(input_file);
                            buffer2[index2++] = ch;
                        }
                        buffer2[index2] = '\0';
                        index2 = 0;
                        if(fpeek(input_file) != ','){
                            fgetc(input_file);
                        }
                        else if(fpeek(input_file) == ','){
                            ch = (char)fgetc(input_file);
                        }
                        while(fpeek(input_file) == ' ')
                            fgetc(input_file);
                        if(ch  == ','){
                            while(fpeek(input_file) == ' ')
                                fgetc(input_file);

                            while((ch = (char)fgetc(input_file)) != ','){
                                buffer3[index3++] = ch;
                            }
                            buffer3[index3] = '\0';
                            index3 = 0;
                            if(ch  == ',') {
                                while (fpeek(input_file) == ' ')
                                    fgetc(input_file);
                            }
                                while((ch = (char)fgetc(input_file)) != ')'){
                                buffer4[index4++] = ch;
                            }
                            buffer4[index4] = '\0';
                            index4 = 0;
                        }
                        for (int j = 0; j < 7; ++j) {
                            if (strcmp(fonksiyonlar[j], bufferCopy2) == 0)
                                keyw_mu = true;
                        }
                    }
                    if(fpeek(input_file) == '+'){
                        fgetc(input_file);
                        fprintf(output_file, "Operator(%s)\n", "+");
                        while(fpeek(input_file) == ' '){
                            fgetc(input_file);              // boþluðu atladý
                        }
                        if(fpeek(input_file) == '"'){
                            durum2 = false;
                            fgetc(input_file);
                            while (fpeek(input_file) != '"'){
                                ch = (char)fgetc(input_file);
                                buffer3[index3++] = ch;
                            }
                            buffer3[index3] = '\0';
                            index3 = 0;
                            fgetc(input_file);
                            fprintf(output_file, "StringConst(%s)\n", buffer3);
                        }
                        else{
                            while (fpeek(input_file) != ';'){
                                ch = (char)fgetc(input_file);
                                buffer3[index3++] = ch;
                            }
                            buffer3[index3] = '\0';
                            index3 = 0;
                        }
                        char*  bufferCopy3 = strdup(buffer3);
                        char a[500] = {0};
                        char b[500] = {0};
                        if (intValue[getBufferIndex(bufferCopy2)] !=0 && getBufferIndex(bufferCopy2) != -1){
                            sprintf(a,"%d",intValue[getBufferIndex(bufferCopy2)]);
                        }
                        if (intValue[getBufferIndex(bufferCopy3)] !=0 && getBufferIndex(bufferCopy3) != -1){
                            sprintf(b,"%d",intValue[getBufferIndex(bufferCopy3)]);
                        }
                        if(isdigit(buffer2[0]) || isdigit(a[0]) || isdigit(b[0])){
                            if(isdigit(buffer2[0]) && isdigit(buffer3[0])){
                                performAddition(gecicibufferlist[0], bufferCopy2,bufferCopy3);
                            }
                            else if (!isdigit(buffer2[0]) && isdigit(buffer3[0])){
                                char int_to_str[500];
                                sprintf(int_to_str,"%d",(int)intValue[getBufferIndex(bufferCopy2)]);
                                performAddition(gecicibufferlist[0],int_to_str, bufferCopy3);
                            }
                            else if (isdigit(buffer2[0]) && !isdigit(buffer3[0])){
                                char int_to_str[500];
                                sprintf(int_to_str,"%d",intValue[getBufferIndex(bufferCopy3)]);
                                performAddition(gecicibufferlist[0],bufferCopy2, int_to_str);
                            }
                            else if (!isdigit(buffer2[0]) && !isdigit(buffer3[0])){
                                char int_to_str[500];
                                char int_to_str2[500];
                                sprintf(int_to_str,"%d",intValue[getBufferIndex(bufferCopy2)]);
                                sprintf(int_to_str2,"%d",intValue[getBufferIndex(bufferCopy3)]);
                                performAddition(gecicibufferlist[0],int_to_str, int_to_str2);
                            }
                        }
                        else {
                            if(durum == false && durum2 == false){
                                stringAddition(gecicibufferlist[0], bufferCopy2,bufferCopy3);
                            }
                            else if(durum == true && durum2 == false){
                                char* string = bufferlValue[getBufferIndex(bufferCopy2)];
                                stringAddition(gecicibufferlist[0],string,bufferCopy3);
                            }
                            else if(durum == false && durum2 == true){
                                char* string = bufferlValue[getBufferIndex(bufferCopy3)];
                                stringAddition(gecicibufferlist[0],bufferCopy2,string);
                            }
                            else{
                                char* string = bufferlValue[getBufferIndex(bufferCopy2)];
                                char* string2 = bufferlValue[getBufferIndex(bufferCopy3)];
                                stringAddition(gecicibufferlist[0],string,string2);
                            }
                        }
                    }
                    else if(fpeek(input_file) == '-'){
                        fgetc(input_file);
                        fprintf(output_file, "Operator(%s)\n", "-");
                        while(fpeek(input_file) == ' '){
                            fgetc(input_file);              // boþluðu atladý
                        }
                        if(fpeek(input_file) == '"'){
                            durum2 = false;
                            fgetc(input_file);
                            while (fpeek(input_file) != '"'){
                                ch = (char)fgetc(input_file);
                                buffer3[index3++] = ch;
                            }
                            buffer3[index3] = '\0';
                            index3 = 0;
                            fgetc(input_file);
                            fprintf(output_file, "StringConst(%s)\n", buffer3);
                        }
                        else{
                            while (fpeek(input_file) != ';'){
                                ch = (char)fgetc(input_file);
                                buffer3[index3++] = ch;
                            }
                            buffer3[index3] = '\0';
                            index3 = 0;
                        }
                        char*  bufferCopy3 = strdup(buffer3);

                        char a[500] = {0};
                        char b[500] = {0};
                        if (intValue[getBufferIndex(bufferCopy2)] !=0 && getBufferIndex(bufferCopy2) != -1){
                            sprintf(a,"%d",intValue[getBufferIndex(bufferCopy2)]);
                        }
                        if (intValue[getBufferIndex(bufferCopy3)] !=0 && getBufferIndex(bufferCopy3) != -1){
                            sprintf(b,"%d",intValue[getBufferIndex(bufferCopy3)]);
                        }
                        if(isdigit(buffer2[0]) || isdigit(a[0]) || isdigit(b[0])){
                            if(isdigit(buffer2[0]) && isdigit(buffer3[0])){
                                performSub(gecicibufferlist[0], bufferCopy2,bufferCopy3);
                            }
                            else if (!isdigit(buffer2[0]) && isdigit(buffer3[0])){
                                char int_to_str[500];
                                sprintf(int_to_str,"%d",(int)intValue[getBufferIndex(bufferCopy2)]);
                                performSub(gecicibufferlist[0],int_to_str, bufferCopy3);
                            }
                            else if (isdigit(buffer2[0]) && !isdigit(buffer3[0])){
                                char int_to_str[500];
                                sprintf(int_to_str,"%d",intValue[getBufferIndex(bufferCopy3)]);
                                performSub(gecicibufferlist[0],bufferCopy2, int_to_str);
                            }
                            else if (!isdigit(buffer2[0]) && !isdigit(buffer3[0])){
                                char int_to_str[500];
                                char int_to_str2[500];
                                sprintf(int_to_str,"%d",intValue[getBufferIndex(bufferCopy2)]);
                                sprintf(int_to_str2,"%d",intValue[getBufferIndex(bufferCopy3)]);
                                performSub(gecicibufferlist[0],int_to_str, int_to_str2);
                            }
                        }
                        else {
                            if(durum == false && durum2 == false){
                                performSubtraction(gecicibufferlist[0], bufferCopy2,bufferCopy3);
                            }
                            else if(durum == true && durum2 == false){
                                char* string = bufferlValue[getBufferIndex(bufferCopy2)];
                                performSubtraction(gecicibufferlist[0],string,bufferCopy3);
                            }
                            else if(durum == false && durum2 == true){
                                char* string = bufferlValue[getBufferIndex(bufferCopy3)];
                                performSubtraction(gecicibufferlist[0],bufferCopy2,string);
                            }
                            else{
                                char* string = bufferlValue[getBufferIndex(bufferCopy2)];
                                char* string2 = bufferlValue[getBufferIndex(bufferCopy3)];
                                performSubtraction(gecicibufferlist[0],string,string2);
                            }
                        }
                    }
                    else if (keyw_mu == true){
                        if(strcmp(keywordss[3], bufferCopy2) == 0){                       // size fonksiyonu
                            intValue[getBufferIndex(buffer)] = (int)strlen(bufferlValue[getBufferIndex(buffer2)]);
                        }
                        if(strcmp(keywordss[4], bufferCopy2) == 0){    // subs fonksiyonu
                            int begin = atoi(buffer3);
                            int end = atoi(buffer4);
                            bufferlValue[getBufferIndex(buffer)] = performSubs(buffer2, begin, end);
                        }
                        if(strcmp(keywordss[5], bufferCopy2) == 0){              // locate
                            int start = atoi(buffer4);
                            intValue[getBufferIndex(buffer)] = performLocate(buffer2,buffer3,start);
                        }
                        if(strcmp(keywordss[6], bufferCopy2) == 0){              //insert
                            int location = atoi(buffer3);
                            bufferlValue[getBufferIndex(buffer)] = performInsert(buffer2,location,buffer4);
                        }
                        if(strcmp(keywordss[7], bufferCopy2) == 0){              //override
                            int location = atoi(buffer3);
                            bufferlValue[getBufferIndex(buffer)] = performOverride(buffer2,location,buffer4);
                        }
                        if(strcmp(keywordss[14], bufferCopy2) == 0){             //AsText
                            intValue[getBufferIndex(buffer)] = performAsText(buffer2);
                        }
                        if(strcmp(keywordss[15], bufferCopy2) == 0){             // AsString
                            bufferlValue[getBufferIndex(buffer)] = performAsString(buffer2);
                        }
                    }
                    else{
                        int indeks = getBufferIndex(gecicibufferlist[0]);
                        if(string_mi == false && bufferlist[indeks] != NULL && indeks != -1){
                            intValue[indeks] = (int)strtol(bufferCopy2, NULL, 10) ;
                            fprintf(output_file, "Integer(%d)\n", intValue[indeks]);
                        }
                        else if (bufferlist[indeks] != NULL && indeks != -1){
                            bufferlValue[indeks] = bufferCopy2 ;
                        }
                    }
                }
            }
            else if(isOperator(is_char_str(ch))){
                fprintf(output_file, "Operator(%c)\n", ch);
            }
                // END OF LINE
            else if(ch == ';'){
                fprintf(output_file, "EndOfLine\n");
            }

        }
    }
    fclose(input_file);
    fclose(output_file);
    return 0;
}

//finds if this method is identifier and checks for errors
int isIdfValid(char* str){
    // if string is valid return 1 or is string is invalid return -1
    // checking 30 characters
    if( strlen(str) > 30){
        return -1;
    }
    if( str[0] > 'z' || str[0] < 'a'){
        return -2;
    }
    return 1;
}

// checking keyword and return bool value
bool lex_find_keyword(char* str){
    char keywords[18][30] = {"new", "int", "text", "size", "subs", "locate", "insert", "override", "read", "write", "from", "to", "input", "output", "asText", "asString"};
    int i ;
    bool result = false;
    for(i = 0; i < 18; ++i){
        if(strcmp(keywords[i], str) == 0){
            result = true;
            break;
        }
    }
    return result;
}

char* is_char_str(char chr){
    char* temp = (char*) malloc(2);
    temp[0] = chr;
    temp[1] = '\0';
    return temp;
}

// checking operator and return bool value
bool isOperator(char* str){
    char operators[7][3] = {"+","-",":="};
    int i ;
    bool result = false;

    for(i = 0; i < 7; ++i){
        if(strcmp(operators[i], str) == 0){
            result = true;
            break;
        }
    }
    return result;
}

char* is_two_char_str(char chr1, char chr2){
    char* temp = (char*) malloc(sizeof(char) * 3);
    temp[0] = chr1;
    temp[1] = chr2;
    temp[2] = '\0';
    return temp;
}

// controls operators formed from the merger of two operators
bool lex_find_combine_operator(char chr1, char chr2){
    char* temp = is_two_char_str(chr1, chr2);
    bool state;
    state = isOperator(temp);
    free(temp);
    return state;
}
char* lex_find_combine_operator2(char chr1, char chr2){
    char* temp = is_two_char_str(chr1, chr2);
    free(temp);
    return temp;
}

// it helps to see the next character
char fpeek(FILE *stream){
    char c;
    c = (char)fgetc(stream);
    ungetc(c, stream);
    return c;
}
int getBufferIndex(char* identifier) {
    for (int i = 0; i < numTextVariables; i++) {
        if(bufferlist[i] != NULL){
            if (strcmp(identifier, bufferlist[i]) == 0) {
                return i;
            }
        }
        else{
        }
    }
    return -1;  // Text variable not found
}
void performAddition( char* identifier,  char* operand1,  char* operand2) {
    int intIndex = getBufferIndex((char*)identifier);
    int op1;
    int op2;
    if (intIndex == -1) {
        printf("Error: Variable '%s' is not declared.\n", identifier);
        return;
    }
    op1 = (int)strtol(operand1,NULL,10);
    op2 = (int)strtol(operand2,NULL,10);
    intValue[intIndex] = op1 + op2;
}
void performSub( char* identifier,  char* operand1,  char* operand2) {
    int intIndex = getBufferIndex((char*)identifier);
    int op1;
    int op2;

    if (intIndex == -1) {
        printf("Error: Variable '%s' is not declared.\n", identifier);
        return;
    }
    op1 = (int)strtol(operand1,NULL,10);
    op2 = (int)strtol(operand2,NULL,10);
    intValue[intIndex] = op1 - op2;
}
void stringAddition( char* identifier,  char* operand1,  char* operand2){
    int intIndex = getBufferIndex((char*)identifier);
    char* op1;
    char* op2;
    if (intIndex == -1) {
        printf("Error: Variable '%s' is not declared.\n", identifier);
        return;
    }
    op1 = operand1;
    op2 = operand2;
    size_t len1 = strlen(op1);
    size_t len2 = strlen(op2);

    char* birlesikString = (char*)malloc((len1 + len2 + 1) * sizeof(char)); // Bellek tahsis ediliyor
    strcpy(birlesikString, op1); // Ýlk string kopyalanýyor
    strcat(birlesikString, op2); // Ýkinci string birleþtiriliyor

    bufferlValue[intIndex] = birlesikString;

}
void performSubtraction(char* identifier, char* operand1, char* operand2 ){
    int textIndex = getBufferIndex(identifier);

    if (textIndex == -1) {
        printf("Error: Variable '%s' is not declared.\n", identifier);
        return;
    }

    char* op1 = strdup(operand1);
    char* op2 = strdup(operand2);

    if (strlen(op2) > strlen(op1)) {
        printf("Error: Length of the second string is greater than the first string.\n");
        bufferlValue[textIndex] = NULL;
        free(op1);
        free(op2);
        return;
    }

    char* substring = strstr(op1, op2);

    if (substring != NULL) {
        size_t op2Length = strlen(op2);
        size_t substringLength = strlen(substring);
        memmove(substring, substring + op2Length, substringLength - op2Length + 1);
        bufferlValue[textIndex] = realloc(bufferlValue[textIndex], strlen(op1) + 1);
        strcpy(bufferlValue[textIndex], op1);
    } else {
        printf("Error: Subtrahend not found in Minuend.\n");
        bufferlValue[textIndex] = NULL;
    }

    free(op1);
    free(op2);
}
char* performSubs( char* identifier, int begin, int end) {
    int textIndex = getBufferIndex(identifier);
    if (textIndex == -1) {
        printf("Error: Variable '%s' is not declared.\n", identifier);
        return NULL;
    }
    int size = (int)strlen(bufferlValue[textIndex]);
    if (begin < 0 || end >= size || begin > end) {
        printf("Error: Invalid begin or end index for subs function.\n");
        return NULL;
    }
    char* substring = malloc((end - begin + 2) * sizeof(char));
    strncpy(substring, bufferlValue[textIndex] + begin, end - begin + 1);
    substring[end - begin + 1] = '\0';
    return substring;
}
int performLocate( char* bigText,  char* smallText, int start) {
    int textIndex = getBufferIndex(bigText);
    int textIndex2 = getBufferIndex(smallText);


    char* substring = strstr(bufferlValue[textIndex] + start, bufferlValue[textIndex2]);
    if (substring != NULL) {
        return (int)(substring - bufferlValue[textIndex]);
    } else {
        return 0;
    }
}
char* performInsert( char* myText, int location,  char* insertText) {
    int textIndex = getBufferIndex(myText);
    int textIndex2 = getBufferIndex(insertText);

    int size = (int)strlen(bufferlValue[textIndex]);
    if (location < 0 || location > size) {
        printf("Error: Invalid location for insert function.\n");
        return NULL;
    }
    char* result = malloc((size + strlen(bufferlValue[textIndex2]) + 1) * sizeof(char));
    strncpy(result, bufferlValue[textIndex], location);
    result[location] = '\0';
    strcat(result, bufferlValue[textIndex2]);
    strcat(result, bufferlValue[textIndex] + location);
    return result;
}
char* performOverride( char* myText, int location,  char* ovrText) {
    int textIndex = getBufferIndex(myText);
    int textIndex2 = getBufferIndex(ovrText);
    int size = (int)strlen(bufferlValue[textIndex]);

    if (location < 0 || location >= size) {
        printf("Error: Invalid location for override function.\n");
        return NULL;
    }
    int i, j;
    for (i = location, j = 0; j < strlen(bufferlValue[textIndex2]); i++, j++) {
        if ( i >= strlen(bufferlValue[textIndex])) {
            break; // Ýþlem myText'in boyutunu aþýyorsa döngüyü sonlandýr
        }
        bufferlValue[textIndex][i] = bufferlValue[textIndex2][j];
    }
    return bufferlValue[textIndex];
}

// Function to perform the asString function
char* performAsString(char* identifier) {
    int textIndex = getBufferIndex(identifier);
    char* result = malloc(16 * sizeof(char));
    sprintf(result, "%d", intValue[textIndex]);
    return result;
}

// Function to perform the asText function
int performAsText( char* myString) {
    int textIndex = getBufferIndex(myString);

    return atoi(bufferlValue[textIndex]);
}
int performRead(char* filename) {
    FILE *file;
    char string[1000] = {0};
    // Open the file for reading
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Read the string from the file
    if (fgets(string, 1000, file) != NULL) {
        // Remove the newline character at the end, if present
        int len = (int)strlen(string);
        string[len] = '\0';
        // Print the string
        printf("String read from the file: %s\n", string);
    } else {
        printf("Failed to read the string from the file.\n");
    }
    // Close the file
    fclose(file);
    return 0;
}

void performWrite(char* myText, char* filename) {
    int textIndex = getBufferIndex(myText);
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Failed to open file '%s'.\n", filename);
        return;
    }
    fprintf(file, "%s", bufferlValue[textIndex]);
    fclose(file);
}

// Function to receive input from keyboard
void performInput( char* identifier,  char* promptText) {
    int textIndex = (int)getBufferIndex(identifier);

    if (textIndex == -1) {
        printf("Error: Variable '%s' is not declared.\n", identifier);
        return;
    }

    char buffer[101] = {0};
    printf("%s: ", promptText);
    fgets(buffer, sizeof(buffer), stdin);
    bufferlValue[textIndex] = realloc(bufferlValue[textIndex], strlen(buffer) + 1);
    strcpy(bufferlValue[textIndex],buffer);
    buffer[strlen(buffer)-1] = '\0';
}
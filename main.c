#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

int main()
{
    char fpeek(FILE *);

    char ch;
    int intlength=0;
    int wordorkey=0;
    int wordlength=0;
    char integer[10];
    char word[20];
    char keywords[18][20]={"BREAK","CASE","CHAR","CONST","CONTINUE","DO","ELSE","ENUM","FLOAT","FOR","GOTO","IF","INT","LONG","RECORD","RETURN","STATIC","WHILE"};
    char string[500];

    FILE*dosyaR;//dosya islemleri
    FILE*dosyaW;
    dosyaR=fopen("code.psi","r");//okunacak dosya
    dosyaW=fopen("code.lex","w"); //yazilacak dosya
    fseek(dosyaR, 0, SEEK_END);//get file size.
    int size = ftell(dosyaR);
    fseek(dosyaR, 0, SEEK_SET);

    ch=fgetc(dosyaR);//ilk chari aliyoruz
    if(dosyaR != NULL)//eger dosya var ise
    {
        while(!feof(dosyaR))
        {
            if(ch == ' ' || ch=='\n' || ch=='\t')//bosluklari saymamamiz gerek.
            {
                ch=fgetc(dosyaR);
            }

            if(ch=='*' || ch=='/' || ch=='+' || ch=='-' || ch==':')//operator durumu.
            {
                if(ch=='*')//* operator'u var ise yazdiriyoruz.
                {
                    fputs("Operator(*)\n",dosyaW);
                }
                if(ch=='/')
                {
                    fputs("Operator(/)\n",dosyaW);
                }
                if(ch=='+')
                {
                    ch=fgetc(dosyaR);
                    if(ch == '+')//++ operator'u var ise yazdiriyoruz.
                    {
                        fputs("Operator(++)\n" , dosyaW);
                    }
                    else//+ operator'u durumu.
                    {
                        fputs("Operator(+)\n" , dosyaW);
                        continue;
                    }
                }
                if(ch=='-')
                {
                    ch=fgetc(dosyaR);
                    if(ch=='-')//-- operator'u var mi diye bakiyoruz.
                    {
                        fputs("Operator(--)\n",dosyaW);
                    }
                    else//- operator'u durumu.
                    {
                        fputs("Operator(-)\n" ,dosyaW);
                        continue;
                    }
                }}
                if(ch==':')//:= operator durumunu inceliyoruz.
                {
                    if (fgetc(dosyaR) == '=')//eger : dan sonra = geliyorsa bu operatordur.
                    {
                        fputs("Operator(:=)\n", dosyaW);
                    }

                }
            if(ch=='0'||ch=='1'||ch=='2'||ch=='3'||ch=='4'||ch=='5'||ch=='6'||ch=='7'||ch=='8'||ch=='9')//integer durumu
            {
                bool integ=true;
                integer[intlength]=ch;
                ch=fgetc(dosyaR);
                while(ch=='0'||ch=='1'||ch=='2'||ch=='3'||ch=='4'||ch=='5'||ch=='6'||ch=='7'||ch=='8'||ch=='9')
                {
                    intlength++;
                    integer[intlength]=ch;
                    ch=fgetc(dosyaR);
                }
                if(intlength>10)//uzunlugu 10dan fazla olamaz
                {
                    fputs("ERROR : Maximum integer size is 10 digits\n ",dosyaW);
                    return 0;
                }
                else
                {
                    fputs("IntConst(",dosyaW);
                    fputs(integer, dosyaW);
                    fputs(")\n",dosyaW);
                }
                memset(integer, 0, sizeof(integer));
                intlength=0;
                continue;

            }
            if(ch==';')//End Of Line ise.
            {
                fputs("EndofLine\n",dosyaW);

            }

            if(ch=='('|| ch==')'|| ch=='[' || ch==']'||ch=='{'||ch=='}')//brackets kontrolu.
            {

                //comment kontrolu
                if (ch == '(' && fpeek(dosyaR) == '*') {
                   bool flag=false;

                    fgetc(dosyaR);
                    while ((ch = fgetc(dosyaR)) != EOF) {
                        if (ch == '*' && fpeek(dosyaR) == ')') {
                            flag=true;
                            fgetc(dosyaR);
                            break;
                        }
                    }
                    //comment kapanmamissa hata verir
                    if(!flag){

                        fputs("ERROR : a comment cannot terminate before the file end",dosyaW);
                                return 0;
                    }

                    ch = fgetc(dosyaR);
                }

                else if (ch == '(' && fpeek(dosyaR) != '*'){

                    fputs("LeftPar\n", dosyaW);
                }
                if(ch==')')
                {
                    fputs("RightPar\n", dosyaW);
                }
                if(ch=='[')
                {
                    fputs("LeftSquareBracket\n", dosyaW);
                }
                if(ch==']')
                {
                    fputs("RightSquareBracket\n", dosyaW);
                }
                if(ch=='{')
                {
                    fputs("LeftCurlyBracket\n", dosyaW);
                }
                if(ch=='}')
                {
                    fputs("RightCurlyBracket\n", dosyaW);
                }

            }
            if(ch=='"')//strings kontrolu
            {
                int counter=0;
                ch=fgetc(dosyaR);
                while(ch!='"')
                {
                    string[counter]=ch;
                    counter++;
                    ch=fgetc(dosyaR);
                    //string kapanmamissa hata mesaji verir
                    if(ch=='\n')
                    {
                        fputs("ERROR :a string constant cannot terminate before the file end\n",dosyaW);
                        return 0;
                    }
                }

                if(ch=='"'){

                string[counter]='\0';//bu iki satiri yapmazsak K\ gibi bir durum söz konusu kaliyordu.
                string[counter+1]='0';
                fputs("String Constants (" ,dosyaW );
                fputs(string,dosyaW);
                fputs(")\n" ,dosyaW);}
            }




            if(('a' <= ch && ch <='z') || ('A' <= ch && ch <= 'Z')  )//identifier ve keywords kotnrolu.
            {

                wordorkey=0;
                wordlength=0;
                bool lenght=true;
                //identifierler sayi,harf ve underscore den olusur
                while(('a' <= ch && ch <='z')|| ('A' <= ch && ch <= 'Z')|| (ch=='_')|| isdigit(ch) )
                {
                    if(wordlength>20)
                    {
                        lenght=false;
                    }

                    if(('a' <= ch && ch <='z'))//kucuk karakter de olsa buyuge cevirilir.
                    {
                        ch=toupper(ch);
                    }

                    word[wordlength]=ch;
                    wordlength++;
                    ch=fgetc(dosyaR);
                }

                //keyword durumu
                for(int x=0;x<18;x++)
                {
                    if(strcmp(word,keywords[x]) == 0)//kelime eger keyword ise bu if'e girer ve wordorkey degiskeni 1 olur.
                    {

                        for(int a=0;a<wordlength;a++){
                            word[a]=tolower(word[a]);
                        }

                        fprintf(dosyaW,"Keyword(%s)\n", word);
                        wordorkey=1;
                        break;
                    }
                }

                //identifier uzunluk kontrolu
                if(!lenght){
                fputs("ERROR : Maximum identifier size is 20 characters.  \n",dosyaW);

                }


                else if(wordorkey==0)//eger word keyword degilse identifier olarak bastiririz.
                {

                    fputs("Identifier(",dosyaW);
                    fputs(word,dosyaW);
                    fputs(")\n",dosyaW);
                }
                memset(word, 0, sizeof(word));
                wordlength=0;
                continue;
            }

            ch=getc(dosyaR);
        }//if
    }//while
}//main




char fpeek(FILE *stream) {
    char c;

    c = fgetc(stream);
    ungetc(c, stream);
    return c;
}

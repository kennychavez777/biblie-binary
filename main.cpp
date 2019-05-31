#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <locale.h>
#include <wchar.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

#define SIZE 32000

using namespace std;

struct Book {
    string nameBook;
    int chapter;
    int verse;
    int hash_;
    string text;
} book;

struct BookC {
    char nameBook [120];
    int chapter;
    int verse;
    int hash_;
    char text[2000];
} bookc;

FILE *alias;
long int dir_fisica;

void showMenu(){
    system("CLS");
    cout << "--------------------------MENÚ------------------------" << endl;
    cout << "1. Cargar nuevo archivo de texto con contenido bíblico" << endl;
    cout << "2. Ver contenido disperso en pantalla" << endl;
    cout << "3. Terminar sesion" << endl;
    cout << "\nOpcion: ";
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

unsigned int hash_function(const std::string& str)
{
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;

    for(std::size_t i = 0; i < str.length(); i++)
    {
        hash = hash * a + str[i];
        a    = a * b;
    }

    return (hash % SIZE);
 };

string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void loadContent(){
    Book data[5000];
    string filename,bookname, word, line, token, text;
    string hash_name,cap,vers;
    std::vector<string> allText;
    bool flagDigit;
    cin.ignore();

    system ("CLS");
    cout << "INGRESA DATOS" << endl;
    cout << "Libro: ";
    getline(cin, bookname);
    cout << "Archivo: ";
    getline(cin, filename);

    ifstream file(filename.c_str());

    while(getline(file, line)){
        std::istringstream s1(line);
        while(getline(s1, token, ' ')){
            allText.push_back(trim(token));
        }
    }

    int count1 = 0, count2 = 0;
    for(std::vector<int>::size_type i = 0; i != allText.size(); i++) {
        std::istringstream ss(allText[i]);

        while(getline(ss, word, ':')){
            if(count1 == 2){
                count1 = 0;
                text = "";
                count2++;
            }

            if(is_number(word)){
                count1++;
                if(count1 == 1){
                    int x = 0;
                    stringstream cap(word);
                    cap >> x;
                    data[count2].nameBook = bookname;
                    data[count2].chapter = x;
                }else{
                    int y = 0;
                    stringstream verse(word);
                    verse >> y;
                    data[count2].verse = y;
                }
            }

            if(count1 == 0){
                text = text + word + " ";
                data[count2-1].text = text;
            }
        }
    }

    file.close();

    for(int a = 0; a < (sizeof(data)/sizeof(*data)); a++){
        if(data[a].nameBook != ""){
            std::stringstream cap_n;
            std::stringstream vers_n;
            cap_n << data[a].chapter;
            vers_n << data[a].verse;

            hash_name = data[a].nameBook + cap_n.str() + vers_n.str();
            data[a].hash_ = hash_function(hash_name);
            cout << "\t => " <<data[a].nameBook << " " << data[a].chapter << ":" << data[a].verse << " | Dirección de memoria: " << hash_function(hash_name)<< endl;
        }
    }

    alias=fopen("books.dat","ab");

    if(alias==NULL)
        alias=fopen("books.dat","wb");

    for(int a = 0; a < (sizeof(data)/sizeof(*data)); a++){
        if(data[a].nameBook != ""){
            strcpy(bookc.nameBook, data[a].nameBook.c_str());
            bookc.chapter = data[a].chapter;
            bookc.verse = data[a].verse;
            strcpy(bookc.text, data[a].text.c_str());
            bookc.hash_ = data[a].hash_;

            fwrite(&bookc,sizeof(bookc),1,alias);
        }
    }

    fclose(alias);
    system ("PAUSE");
}

void readFile(){
    system("CLS");
    alias = fopen("books.dat","rb");
    fread(&bookc,sizeof(bookc),1,alias);

    cout << "\n---------------------------------------------------------------------------------------------------------------";
    cout << "\nHASH       LIBRO           CAPITULO        VERSO       TEXTO";
    cout << "\n---------------------------------------------------------------------------------------------------------------";

    while (!feof(alias)){
        std::string text(bookc.text);
        text = text.substr(0,50);
        printf("\n\r%3d\t%7s\t%12d\t%10d\t%53s",bookc.hash_, bookc.nameBook, bookc.chapter,bookc.verse,text.c_str());
        fread(&bookc,sizeof(bookc),1,alias);
    }

    cout << "\n---------------------------------------------------------------------------------------------------------------";
    cout << endl;
    fclose(alias);

    system("pause");
}


int main()
{
    int opt;
    setlocale(LC_ALL, "");

    while(opt != 3){
        showMenu();
        cin >> opt;

            switch(opt){
                case 1:
                    loadContent();
                    break;
                case 2:
                    readFile();
                    break;
                case 3:
                    cout << "\n\nSaliendo...\n";
                    break;
                default:
                    cout << "\nError: La opcion seleccionada no existe.\n";
                    break;
            }
    }

    return 0;
}

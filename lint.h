#include <vector>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Lint 
{
private:
    vector<char> _number;   //хранение числа, 1 элемент массива - 1 байт - 1 символ
	char _sign; //знак числа
	   
	Lint mult10(int degree);    //дописывает нули в конец числа, degree - количство нулей(степень десятки)
	Lint pow(Lint degree);  //функция возведения в степень
			    		
    unsigned long long _gg;
public:
    string read(char* c);   //чтение из файла                               
    void write(string str, char* c); //запись в файл                         
    Lint binread(char* c);  //бинарное чтение из файла                                     
    void binwrite(Lint &result, char* c);    //бинарная запись в файл                   
	
	string __str__();
	string __repr__();
	
	Lint(); //конструктор по-умолчанию, создает число 0
	Lint(string number);   //создание числа из строки
	Lint(char sign, int length, vector<char> number);   //создание числа из парметров: знак, длина, цифры
	
	Lint operator+(Lint add);
	Lint operator-(Lint sub);
	Lint operator*(Lint mult);
	Lint operator/(Lint div);
	Lint operator%(Lint div);
	Lint operator^(Lint degree);

	bool operator>(Lint right);
	bool operator<(Lint right);
	bool operator==(Lint right);
	bool operator!=(Lint right);                       

	Lint trim();    //удаление ведущий нулей
    string toString();   //преобразование числа в строку
    bool isNegative();  //проверка знака числа
    
    Lint powmod(Lint a, Lint k, Lint n);	
};

#include <cstdio>
#include <stack>

#include "lex.hpp"

typedef unsigned char byte;
using namespace std;

typedef vector<tableEl> alternative;

// На протяжении всй программы: 0 - терминальный, 1 - нетерминальный

// Структура для нетерминального символа
//Символ, количество альтернатив, номер первой в списке правил
struct infNet{
    tableEl Net;
    int CountAlt, firstN;

    infNet(tableEl _Net, int _CountAlt, int _firstN)
        : Net(_Net)
        , CountAlt(_CountAlt)
        , firstN(_firstN)
    {
    }

    infNet()
    {
    }
};

//Структура для элемента массива L1
//флаг терминал/нетерминал, символ, номер текущей альтернативы, количество альтернатив
//если символ терминал, то последние 2 поля не используются
struct elemL1{
    tableEl sym;
    int nAlt, countAlt;

    elemL1(tableEl _sym, int _nAlt, int _countAlt)
        : sym(_sym)
        , nAlt(_nAlt)
        , countAlt(_countAlt)
    {
    }

    elemL1()
    {
    }
};

//Структура для элемента массива L2
//флаг терминал/нетерминал, символ
struct elemL2 {
	tableEl sym;

    elemL2(tableEl _sym)
        : sym(_sym)
    {
    }

    elemL2()
    {
    }
};

//Ищет нетерминал
//На вход принимает массив нетерминалов, размер массива и символ нетерминала
infNet findNet(infNet* Ns, int count, tableEl NT)
{
    for (int i = 0; i < count; ++i) {
        if ((NT.lexType == 'n')&&(Ns[i].Net.lexValue == NT.lexValue)) {
            return Ns[i];
        }
    }

    return infNet({'n', ""}, 0, 0);
}

//Возвращает количество альтернатив для нетерминала
//Входные данные как для findNet
int countAlt(infNet* Ns, int count, tableEl NT)
{
    infNet n = findNet(Ns, count, NT);
    return n.CountAlt;
}

//Возвращает правую часть правила для нетерминала NT
vector<tableEl>& getAlt(infNet* Ns, int count, tableEl NT, int nAlt, vector<alternative>& alts)
{
    infNet n = findNet(Ns, count, NT);
    return alts[ n.firstN + nAlt ];
}

//Кладёт правую часть правила в L2
void fillAlt(stack<elemL2>& L2, vector<tableEl>&  alt)
{
    for(int i = alt.size(); i > 0; --i){
        L2.push(elemL2(alt[i-1]));
    }
}

//sym - нетерминал, для которого удаляется альтернатива
//nAlt - нормер альтернативы, которую нужно удалить
//NS - массив, где содержатся определения всех правил
//count - размер NS
//alts - массив всех правых частей
void removeAlt(stack<elemL2>& L2, tableEl sym, int nAlt, infNet* Ns, int count, vector<alternative>&  alts)
{
    vector<tableEl>& old_alt = getAlt(Ns, count, sym, nAlt, alts);
    for (int i = 0; i < old_alt.size(); ++i) {
        L2.pop();
    }
}

int main(){
//    char *str="!a*b*b*a*a!";//входная строка
	vector<tableEl> str = makeConvolution("prog_example.txt");
//    char *LR="ABBTTMMM";//левые части правил
 //   tableEl *RR[] = { "!B!", "T+B", "T", "M", "M*T", "a", "b", "(B)" };  // правые части правил

	vector<alternative> RR;

	RR.resize(43);
	//P 0-0
	RR[0] = alternative{tableEl('s', "void"), tableEl('s', "main"), tableEl('z', "("), tableEl('z', ")"), tableEl('z', "{"), tableEl('n', "L"), tableEl('z', "}") };
	//L 1-2
	RR[1] = alternative{tableEl('n', "O"), tableEl('n', "L")};
	RR[2] = alternative{tableEl('n', "O")};
	//0 3-6
	RR[3] = alternative{tableEl('n', "I")};
	RR[4] = alternative{tableEl('n', "W")};
	RR[5] = alternative{tableEl('n', "A"), tableEl('z', ";")};
	RR[6] = alternative{tableEl('n', "D"), tableEl('z', ";")};
	//I 7-12
	RR[7] = alternative{tableEl('s', "if"), tableEl('z', "("), tableEl('n', "C"), tableEl('z', ")"), tableEl('z', "{"), tableEl('n', "L"), tableEl('z', "}"), tableEl('s', "else"), tableEl('z', "{"), tableEl('n', "L"), tableEl('z', "}")};
	RR[8] = alternative{tableEl('s', "if"), tableEl('z', "("), tableEl('n', "C"), tableEl('z', ")"), tableEl('n', "O"), tableEl('s', "else"), tableEl('z', "{"), tableEl('n', "L"), tableEl('z', "}")};
	RR[9] = alternative{tableEl('s', "if"), tableEl('z', "("), tableEl('n', "C"), tableEl('z', ")"), tableEl('z', "{"), tableEl('n', "L"), tableEl('z', "}"), tableEl('s', "else"), tableEl('n', "O")};
	RR[10] = alternative{tableEl('s', "if"), tableEl('z', "("), tableEl('n', "C"), tableEl('z', ")"), tableEl('n', "O"), tableEl('s', "else"), tableEl('n', "O")};
	RR[11] = alternative{tableEl('s', "if"), tableEl('z', "("), tableEl('n', "C"), tableEl('z', ")"), tableEl('z', "{"), tableEl('n', "L"), tableEl('z', "}") };
	RR[12] = alternative{tableEl('s', "if"), tableEl('z', "("), tableEl('n', "C"), tableEl('z', ")"), tableEl('n', "O")};
	//W 13-14
	RR[13] = alternative{tableEl('s', "while"), tableEl('z', "("), tableEl('n', "C"), tableEl('z', ")"), tableEl('z', "{"), tableEl('n', "L"), tableEl('z', "}") };
	RR[14] = alternative{tableEl('s', "while"), tableEl('z', "("), tableEl('n', "C"), tableEl('z', ")"), tableEl('n', "O"),};
	//A 15-16
	RR[15] = alternative{tableEl('i', ""), tableEl('z', "="), tableEl('c', "")};
	RR[16] = alternative{tableEl('i', ""), tableEl('z', "="), tableEl('n', "X")};
	//D 17-20
	RR[17] = alternative{tableEl('s', "int"), tableEl('n', "A"), tableEl('z', ","), tableEl('n', "H")};
	RR[18] = alternative{tableEl('s', "int"), tableEl('i', ""), tableEl('z', ","), tableEl('n', "H")};
	RR[19] = alternative{tableEl('s', "int"), tableEl('n', "A")};
	RR[20] = alternative{tableEl('s', "int"), tableEl('i', "")};
	//C 21-24
	RR[21] = alternative{tableEl('n', "M"), tableEl('z', "&"), tableEl('n', "C")};
	RR[22] = alternative{tableEl('n', "M"), tableEl('z', "|"), tableEl('n', "C")};
	RR[23] = alternative{tableEl('z', "!"), tableEl('n', "C")};
	RR[24] = alternative{tableEl('n', "M")};
	//M 25-30
	RR[25] = alternative{tableEl('n', "X"), tableEl('z', ">"), tableEl('n', "X")};
	RR[26] = alternative{tableEl('n', "X"), tableEl('z', "<"), tableEl('n', "X")};
	RR[27] = alternative{tableEl('n', "X"), tableEl('z', ">="), tableEl('n', "X")};
	RR[28] = alternative{tableEl('n', "X"), tableEl('z', "<="), tableEl('n', "X")};
	RR[29] = alternative{tableEl('n', "X"), tableEl('z', "!="), tableEl('n', "X")};
	RR[30] = alternative{tableEl('n', "X"), tableEl('z', "=="), tableEl('n', "X")};
	//X 31-36
	RR[31] = alternative{tableEl('z', "("), tableEl('n', "X"), tableEl('z', ")")};
	RR[32] = alternative{tableEl('n', "T"), tableEl('z', "+"), tableEl('n', "X")};
	RR[33] = alternative{tableEl('n', "T"), tableEl('z', "-"), tableEl('n', "X")};
	RR[34] = alternative{tableEl('n', "T"), tableEl('z', "*"), tableEl('n', "X")};
	RR[35] = alternative{tableEl('n', "T"), tableEl('z', "/"), tableEl('n', "X")};
	RR[36] = alternative{tableEl('n', "T")};
	//T 37-38
	RR[37] = alternative{tableEl('i', "")};
	RR[38] = alternative{tableEl('c', "")};
    //H 39-42
    RR[39] = alternative{tableEl('n', "A"), tableEl('z', ","), tableEl('n', "H")};
	RR[40] = alternative{tableEl('i', ""), tableEl('z', ","), tableEl('n', "H")};
	RR[41] = alternative{tableEl('n', "A")};
	RR[42] = alternative{tableEl('i', "")};

    const int m = 12; // количество нетерминалов

    infNet inf[m] = {
        infNet(tableEl('n', "P"), 1, 0),
        infNet(tableEl('n', "L"), 2, 1),
        infNet(tableEl('n', "O"), 4, 3),
        infNet(tableEl('n', "I"), 6, 7),
        infNet(tableEl('n', "W"), 2, 13),
        infNet(tableEl('n', "A"), 2, 15),
        infNet(tableEl('n', "D"), 4, 17),
        infNet(tableEl('n', "C"), 4, 21),
        infNet(tableEl('n', "M"), 6, 25),
        infNet(tableEl('n', "X"), 6, 31),
        infNet(tableEl('n', "T"), 2, 37),
        infNet(tableEl('n', "H"), 4, 39)
    };


    //использовать стеки-шаблоны из STL
    stack<elemL1> L1;
    struct elemL1 x;// элементы

    stack<elemL2> L2;
    struct elemL2 y; //стеков

    char sos = 'q';//состояние алгоритма {‘q’,’b’,’t’,'e'} - e - ошибка
    tableEl S('n', "P");//начальный символ грамматики
    int i = 0; //счётчик для входной строки str
    int count1;
    char sym1;
    //j,k не нужны, т.к. используется шаблок стек, в нём это делается автоматически

    elemL1 tmpL1;
    elemL2 tmpL2;

    /*
    Функции работы со стеком:
    push() - добавить элемент
    pop() - удалить верхний элемент
    top() - получить верхний элемент
    size() - размер стека
    empty() - true, если стек пуст
    */

    // Шаг 0, в L2 содержится начальный символ грамматики
    L2.push(elemL2(S));

    ofstream fout;
    fout.open("result.txt");
    while (sos != 't' && sos != 'e') {

    if(!L2.empty() && !L1.empty()){
       fout <<  "(" << sos << ", " << i << ", " << L1.top().sym.lexType << " " << L1.top().sym.lexValue << ", " << L2.top().sym.lexType << " " << L2.top().sym.lexValue << ")" << endl ;
    }

        switch (sos)
        {
        case 'q':
            tmpL2 = L2.top();

            // Шаг 1
            if (tmpL2.sym.lexType == 'n') {
                // Если символ на вершине L2 -- нетерминал,
                // применяем первую альтернативу
                L2.pop();//Удаляем из стека верхний элемент

                int count = countAlt(inf, m, tmpL2.sym); //Количество альтернатив для текущего нетерминала
                L1.push(elemL1(tmpL2.sym, 0, count));//Заносим его в вершину стека L1
                fillAlt(L2, getAlt(inf, m, tmpL2.sym, 0, RR));//Заносим правую часть правила в L2
            } else if (str[i].lexType == tmpL2.sym.lexType && (str[i].lexValue == tmpL2.sym.lexValue || str[i].lexType == 'i' || str[i].lexType == 'c')) {//Если в стеке терминал и он совпадает с текущим символом строки
                // Шаг 2
                L2.pop();//Удаляем его из стека L2

                L1.push(elemL1(tmpL2.sym, 0, 0));//Заносим в L1
                ++i;
            } else {
                // Шаг 4
                sos = 'b';//Иначе переходим в режим возврата
            }

            // Шаг 3
            if (i == str.size() && L2.empty()) { //Если строка закончилась и стек L2 пуст, то удачное завершение программы
                sos = 't';
            }

            // Шаг 3'
            if ( (i == str.size() && !L2.empty()) ||
                 (i < str.size() && L2.empty()) )
            {
                sos = 'b';
            }

            break;//Прерывается switch, снова попадаем в цикл

        case 't':
            break;

        case 'b':
            tmpL1 = L1.top();

            if (tmpL1.sym.lexType == 'n') {//Если в L1 был нетерминал
                // Шаг 6

                if (tmpL1.nAlt+1 < tmpL1.countAlt) {//Если есть ещё альтернативы
                    // Шаг 6а
                    L1.top().nAlt++;

                    removeAlt(L2, tmpL1.sym, tmpL1.nAlt, inf, m, RR);
                    vector<tableEl>&  new_alt = getAlt(inf, m, tmpL1.sym, tmpL1.nAlt+1, RR);
                    fillAlt(L2, new_alt);
                    sos = 'q';
                } else if (i == 0 && tmpL1.sym.lexValue == inf[0].Net.lexValue && tmpL1.sym.lexType == 'n') {//Дошли до начального символа грамматики, нельзя больше применить ни одной альтернативы
                    // Шаг 6б
                    sos = 'e';
                } else {
                    // Шаг 6в
                    removeAlt(L2, tmpL1.sym, tmpL1.nAlt, inf, m, RR);//Удалили символ из L2
                    L2.push(elemL2(tmpL1.sym));//Положили на его место последний символ из L1
                    L1.pop();
                    sos = 'b';
                }

            } else {//Терминальный символ, переложили его обратно в L2 и сдвинули указатель на строке
                // Шаг 5
                --i;
                L1.pop();
                L2.push(elemL2(tmpL1.sym));
            }

            break;
        }
    }

    printf("State: %c, L1 ~ %d, L2 ~ %d\n", sos, L1.size(), L2.size());

    if (sos == 'e') {
        printf("Error! Can't parse input\n");
    } else {
        stack<elemL1> reverse;
        while (!L1.empty()) {
            tmpL1 = L1.top();
            reverse.push(tmpL1);
            L1.pop();
        }

		//printf("String: %s\n", str);
        while (!reverse.empty()) {
            tmpL1 = reverse.top();
            reverse.pop();

            if (tmpL1.sym.lexType == 'n') {
                infNet n = findNet(inf, m, tmpL1.sym);
                int alt = tmpL1.nAlt;
                int rule = n.firstN + alt;

                //printf("%c (%d) -> (%d) %s\n", tmpL1.sym, alt, rule, RR[rule]);
                //cout << tmpL1.sym.lexTyp
            }
        }
    }
	char k;
	scanf("%c", &k);

	fout.close();
    return 0;
}

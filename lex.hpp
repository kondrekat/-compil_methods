#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;


struct tableEl{
    char lexType;
    string lexValue;

    tableEl(char _lT, const string _lV)
    : lexType(_lT)
    , lexValue(_lV)
    {
    }

    tableEl(){}
};
/* i - èäåíòèôèêàòîðû, s  - ñëóæåáíûå ñëîâà, c - êëíñòàíòû, z - âåðíûå çíàêè, e - îøèáêà */

//Ãëîáàëüíûå ïåðåìåííûå, ñîäåðæàíèå äîïóñòèìûå ñèìâîëû
vector<char> validChars = {'+', '-', ';', ',', '*', '/', '{', '}', '(', ')'};
vector<char> validChars1 = {'=', '<', '>', '!'};
vector<char> validConst = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
vector<char> validID(2*('z'-'a')+1);
vector<char> whiteSpace = {' ', '\n', '\r', '\t'};
vector<string> words = {"if", "else", "main", "void", "int", "for", "while"};

//Èíèöèàëèçèðóåò validID

void validIDintializer(){
    int i;


    for(i = 0; i < 'z'-'a'; i++){
        validID.push_back('a' + i);
    }

    for(i = 0; i < 'z'-'a'; i++){
        validID.push_back('A' + i);
    }

    validID.push_back('_');
}
enum listOfStates{cons, sign, id, alpha, error, long_sign, long_sign1};
char toLexType(int s)
{
    class mapping
    {
    private:
        typedef std::map<int, char> map_type;
        map_type _mapping;
        char _default;

        void add(const int& s, char t)
        {
            _mapping[s] = t;
            _default = 'e';
        }

        mapping()
        {
            add(cons, 'c');
            add(sign, 'z');
            add(id, 'i');
            add(alpha, 'a');
            add(long_sign, 'z');
            add(long_sign1, 'z');
        }

        public: static char get(const int& s)
        {
            static mapping m;
            map_type::const_iterator iter = m._mapping.find(s);
            if (iter == m._mapping.end()) {
                return m._default;
            } else {
                return iter->second;
            }
        }
    };

    return mapping::get(s);
}

template<typename T>
bool isElem(const vector<T>& v, T c){
    for(int i = 0; i < v.size(); i++){
        if(v[i] == c)
            return true;
    }
    return false;
}

listOfStates nextStep(char inpChar, listOfStates state, bool& flag){
    switch(state){
    case cons:{
        if(isElem(validConst, inpChar)){
            flag = false;
            return cons;
        }
        if(isElem(validChars, inpChar)){
            flag = true;
            return sign;
        }
        if(isElem(validChars1, inpChar)){
            flag = true;
            return long_sign;
        }
        if(isElem(whiteSpace, inpChar)){
            flag = true;
            return alpha;
        }
        return error;
        break;
    }
    case sign:{
        if(isElem(validConst, inpChar)){
            flag = true;
            return cons;
        }
        if(isElem(validChars, inpChar)){
            flag = true;
            return sign;
        }
        if(isElem(validChars1, inpChar)){
            flag = false;
            return long_sign;
        }
        if(isElem(validID, inpChar)){
            flag = true;
            return id;
        }
        if(isElem(whiteSpace, inpChar)){
            flag = true;
            return alpha;
        }
        return error;
        break;
    }
    case id:{
        if(isElem(validID, inpChar)){
            flag = false;
            return id;
        }
        if(isElem(validChars, inpChar)){
            flag = true;
            return sign;
        }
        if(isElem(validChars1, inpChar)){
            flag = true;
            return long_sign;
        }
        if(isElem(whiteSpace, inpChar)){
            flag = true;
            return alpha;
        }
        return error;
        break;
    }
    case long_sign1:{
        if(isElem(validID, inpChar)){
            flag = true;
            return id;
        }
        if(isElem(validChars, inpChar)){
            flag = true;
            return sign;
        }
        if(isElem(validChars1, inpChar)){
            flag = true;
            return long_sign;
        }
        if(isElem(validConst, inpChar)){
            flag = true;
            return cons;
        }
        if(isElem(whiteSpace, inpChar)){
            flag = true;
            return alpha;
        }
        return error;
        break;
    }
    case alpha:{
        if(isElem(validID, inpChar)){
            flag = false;
            return id;
        }
        if(isElem(validChars, inpChar)){
            flag = false;
            return sign;
        }
        if(isElem(validChars1, inpChar)){
            flag = false;
            return long_sign;
        }
        if(isElem(validConst, inpChar)){
            flag = false;
            return cons;
        }
        if(isElem(whiteSpace, inpChar)){
            flag = true;
            return alpha;
        }
        return error;
        break;
    }
    case long_sign:{
        if(inpChar == '='){
            flag = false;
            return long_sign1;
        }
        if(isElem(validConst, inpChar)){
            flag = true;
            return cons;
        }
        if(isElem(validChars, inpChar)){
            flag = true;
            return sign;
        }
        if(isElem(validID, inpChar)){
            flag = true;
            return id;
        }
        if(isElem(whiteSpace, inpChar)){
            flag = true;
            return alpha;
        }
        return error;
        break;
    }
    default:
        return error;
    }

    }


vector<tableEl> makeConvolution(string fileName)
{

    validIDintializer();
    ifstream file(fileName);
    string s;
    listOfStates state = alpha;
    vector<tableEl> convolution;
    tableEl tmpEl;
    string tmpStr = "";
    int counter1 = 0;
    bool flag;
    while(getline(file, s)){
            while(counter1 < s.length()){
                //std::cout << "Considering character '" << s[counter1] << "'" << std::endl;
                tmpEl.lexType = toLexType(state);
                state = nextStep(s[counter1], state, flag);
                if(state == error){
                    break;
                }
                if(flag){
                        //if(s[counter1] != ' ')
                           // tmpStr += s[counter1];
                        tmpEl.lexValue = tmpStr;
                        tmpStr = "";
                        if(!isElem(whiteSpace, s[counter1]))
                            tmpStr.push_back(s[counter1]);
                        //std::cout << "Adding: [" << tmpEl.lexType << ": " << tmpEl.lexValue << "]" << std::endl;
                        if(tmpEl.lexType != 'a'){
                            convolution.push_back(tmpEl);
                        }
                }
                else{
                    tmpStr.push_back(s[counter1]);
                }
                counter1++;
            }
        counter1 = 0;
    }
    file.close();
    int i;
    for(auto& x: convolution) {
        if((x.lexType == 'i')&&(isElem(words, x.lexValue)))
            x.lexType = 's';
    }

    for(i = 0; i < convolution.size(); i++){
        cout << i << " " << convolution[i].lexType << " " << convolution[i].lexValue << endl;
    }
    return convolution;
}


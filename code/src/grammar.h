#include<iostream>
#include "lexical.h"

using namespace token;
using namespace idtable;
namespace quadruple
{
enum Operation
{
    ASN = 0,
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    JLT = 5, // Jump-LessThan
    JLE = 6, // Jump-LessOrEqual
    JGT = 7, // Jump-GreaterThan
    JGE = 8, // Jump-GreaterOrEqual
    JE = 9, // Jump-Equal
    J = 10, // Jump
    JNE = 11, // Jump-NotEqual
};

// Declaration of class Equ.
class Equ
{
public:
    string Op; // Operator string, equal to the enum Operation.
    string StrLeft; // Frist value.
    string StrRight; // Second value.
    string result; // Target of the quadruple.

    // Summary: default constructor.
    Equ(string,string,string,string);
};

class Quadruple
{
private:
    static int count;
    int Idx;
    Operation Opt;
    int Src;
    int Dst;
    int Addr;
    // Summary: return integer value of the given string.
    int Convert(string);
public:
    // Summary: default constructor.
    Quadruple();

    // Summary: constructor of Quadruple, convert Equ object to Quadruple object.
    Quadruple(quadruple::Equ);

    // Summary: return index of current quadruple.
    int GetIndex();

    // Summary: return operation of current quadruple.
    Operation GetOperation();

    // Summary: return source target of current quadruple.
    int GetSrc();

    // Summary: return destination target of current quadruple.
    int GetDst();

    // Summary: return entry address of current quadruple.
    int GetAddr();
};
int quadruple::Quadruple::count = 0; // Initialize the static data member of Quadruple.

class BasicBlockQuadruple
{
private:
    Quadruple qple;
    static int count;
    int idx;
    bool isEntrance;
public:
    // Summary: constructor of BasicBlockQuadruple, assign value to its private member.
    BasicBlockQuadruple(Quadruple,bool);

    // Summary: return the index of the BasicBlockQuadruple.
    int GetIndex();

    // Summary: return the quadruple object.
    Quadruple GetQuadruple();

    // Summary: return if the current object an entrance of a basic block.
    bool IsEntrance();

    // Summary: set isEntrance property to true when the quadruple is referenced in a J** quadruple.
    void SetBasic();
};
int BasicBlockQuadruple::count = 0;





quadruple::Equ::Equ(string a,string b,string c,string d):Op(a),StrLeft(b),StrRight(c),result(d) {};

int quadruple::Quadruple::Convert(string src)
{
    const char* strv = src.data();
    int size = strlen(strv);
    int result = 0;
    for(int index(0); index<size; index++)
    {
        result = result * 10 + ((int)strv[index]-48);
    }
    return result;
}

// Summary: default constructor of Quadruple.
quadruple::Quadruple::Quadruple()
{
    Idx = -1;
    Src = -1;
    Dst = -1;
    Opt = Operation::ADD; // In default, assign an ADD operator to the Op value.
}

// Summary: constructor of Quadruple, convert Equ object to Quadruple object.
quadruple::Quadruple::Quadruple(quadruple::Equ equ)
{
    Idx = Quadruple::count;
    Opt = (quadruple::Operation)Convert(equ.Op);
    Src = Convert(equ.StrLeft);
    Dst = Convert(equ.StrRight);
    Addr = Convert(equ.result);
    Quadruple::count++;
}

// Summary: return index of current quadruple.
int quadruple::Quadruple::GetIndex()
{
    return Idx;
}

// Summary: return the quadruple object.
quadruple::Quadruple quadruple::BasicBlockQuadruple::GetQuadruple()
{
    return qple;
}

// Summary: return operation of current quadruple.
quadruple::Operation quadruple::Quadruple::GetOperation()
{
    return Opt;
}

// Summary: return source target of current quadruple.
int quadruple::Quadruple::GetSrc()
{
    return Src;
}

// Summary: return destination target of current quadruple.
int quadruple::Quadruple::GetDst()
{
    return Dst;
}

// Summary: return entry address of current quadruple.
int quadruple::Quadruple::GetAddr()
{
    return Addr;
}



// Summary: constructor of BasicBlockQuadruple, assign value to its private member.
quadruple::BasicBlockQuadruple::BasicBlockQuadruple(Quadruple qpl,bool is_entrance = false)
{
    qple = qpl;
    idx = BasicBlockQuadruple::count;
    BasicBlockQuadruple::count++;
    isEntrance = is_entrance;
}

// Summary: return the index of the BasicBlockQuadruple.
int quadruple::BasicBlockQuadruple::GetIndex()
{
    return idx;
}

// Summary: return if the current object an entrance of a basic block.
bool quadruple::BasicBlockQuadruple::IsEntrance()
{
    return isEntrance;
}

// Summary: set isEntrance property to true when the quadruple is referenced in a J** quadruple.
void quadruple::BasicBlockQuadruple::SetBasic()
{
    isEntrance = true;
}

}

using namespace quadruple;

typedef struct S {
    vector<int> next;
};

typedef struct E {
    vector<int> falseExit;
    vector<int> trueExit;
};



class Test
{
private:
    int i = 0;
    int ti = 1; // 用来生成临时变量,tn
    string tt = "";

public:
    vector<E>es;
    vector<Equ>equs;
    vector<Token> tokens;  // 关键字
    vector<Symbol> symbols;// 自定义变量和常数
    string error = "";

    Test(vector<Token> t, vector<Symbol> s)//传入词法分析产生的token文件和符号表文件
    {
        tokens = t;
        symbols = s;
        checkMainProgram();// 执行主要函数
    };

    // Summary: return internal quadruples - equs;
    vector<Equ> GetEqus()
    {
        return equs;
    }

    //取token数组的下一个
    void Next()
    {
        if (i < tokens.size() - 1)
        {
            i++;
        }
    }
    //取token数组的上一个
    void Before()
    {
        if (i > 0)
        {
            i--;
        }
    }

    // 创建临时变量NewTemp()并添加到symble中
    string NewTemp() {
		stringstream ss;
		ss << "T" <<ti;
        
        ti++;
        //Symbol s(SymbolType::IDENTIFIER,temp);
        //symbols.push_back(s);
        return ss.str();
    }

    // 回填函数BackPatch(int addr, int addr2)
    // 把addr所链接的每个四元式的第四分量都改写为地址addr2
    void BackPatch(int addr, int addr2) {
        equs[addr].result = "" + addr2;
    }

    // 产生四元式
    void gen(string op, string strLeft, string strRight, string result) {
        Equ fp = Equ(op, strLeft, strRight, result);
        // 将新生成的四元式表项fp添加到四元式列表equs中
        equs.push_back(fp);
    }

    // 主要函数checkMainProgram()
    // 〈程序〉→ program〈标识符〉〈程序体〉
    void checkMainProgram() {
        if (tokens[i].GetTag() == 29)// 如果是program，判断下一个单词
        {
            Next();
            if (tokens[i].GetTag() == 2)// 如果是标识符，判断程序体
            {

                Next();
                checkProgramBody();// 程序体
            }
            else {
                error = "该程序缺少标示符";
            }
        }
        else {
            error = "该程序缺少关键字program";
        }
        //for (Equ equ : equs)
        // {
        //	 cout << "("<<equ.Op<<","<<equ.StrLeft<<","<<equ.StrRight<<","<<equ.result<<")"<< endl;
        // }
        cout << error << endl;
    }

    // 程序体checkProgramBody()
    // 〈程序体〉→〈变量说明〉〈复合句〉
    // 〈变量说明〉→ var〈变量定义〉|ε
    // 〈复合句〉→ {〈语句表〉}
    void checkProgramBody() {
        if (tokens[i].GetTag() == 32)// 如果是var，指向下一个单词
        {
            Next();
            checkVar();// 执行变量定义
        }
        else if (tokens[i].GetTag() == 27)// 如果是{，指向下一个单词
        {
            Next();
            checkComplex();// 执行语句表
        }
        else// 否则报错
        {
            error = "程序缺少var或{";
        }
    }

    // 变量定义checkVar()
    // 〈变量定义〉→〈标识符表〉：〈类型〉；｜〈标识符表〉：〈类型〉；〈变量定义〉
    // 〈标识符表〉→〈标识符〉，〈标识符表〉｜〈标识符〉
    // 〈程序体〉→〈变量说明〉〈复合句〉
    // 〈变量说明〉→ var〈变量定义〉|ε
    void checkVar()
    {
        if (IsIdlist())// 若该字符为标识符表，则判断下一个字符，若为冒号，继续判断类型定义是否正确
        {
            Next();
            if (tokens[i].GetTag() == 34) // 冒号
            {
                Next();
                // 类型为bool、number
                if (tokens[i].GetTag() == 4 || tokens[i].GetTag() == 3)
                {
                    // // 定义j，指向前面的标示符
                    // int j = i;
                    // j = j - 2;
                    // // 类型定义正确，在符号表中记录该标识符的类型
					
                    // symbols[tokens[j].GetAddress()].SetType(tokens[i].GetTag()==2?SymbolType::IDENTIFIER:SymbolType::CONST);
                    // j--;
                    // // 若标识符后面有逗号，表示同时定义了几个相同类型的变量，把它们都添加到符号表中
                    // while (tokens[j].GetTag() == 33 && j > 0) {
                    //     j--;
                    //     symbols[tokens[j].GetAddress()].SetType(tokens[i].GetTag()==2?SymbolType::IDENTIFIER:SymbolType::CONST);
                    //     j--;
                    // }
                    Next();
                    if (tokens[i].GetTag() == 24) // 如果是分号，判断下一个单词，若为{，执行复合句；否则继续循环执行变量定义
                    {
                        Next();
                        if (tokens[i].GetTag()== 27) // {
                        {
                            Next();
                            checkComplex();// 执行语句表
                        }
                        else {
                            checkVar();// 继续循环执行
                        }
                    }
                    else {
                        error = "变量定义后面缺少；";
                    }
                }
                else {
                    error = "变量定义缺少类型或类型定义错误";
                    return;
                }
            }
            else {
                error = "标识符后面缺少冒号";
            }
        }
        else {
            error = "不是标识符，出错";
        }
    }

    // 递归：判断是不是标识符表 IsIdlist
    //〈标识符表〉→〈标识符〉,〈标识符表〉｜〈标识符〉
    bool IsIdlist()
    {
        if (tokens[i].GetTag() == 2)//标识符
        {
            //若是标识符，判断下一个字符，如果是逗号，继续判断下一个字符，如果不是逗号，指向前一个字符，返回true，否则返回false——此方法用来判断是否将几个变量定义为同一个类型
            Next();
            if (tokens[i].GetTag() == 33)//逗号
            {
                Next();
                return IsIdlist();//下一个字符若为逗号，则继续循环执行判断是否为标识符表
            }
            else
            {
                //指向前一个字符，为标识符，返回true
                Before();
                return true;
            }
        }
        else
        {
            return false;
        }
    }

    // 复合句ComSent()
    // 〈复合句〉→ {〈语句表〉}
    void checkComplex()
    {
        SentenceList();// 执行语句表
        if (error == "") {
            if (tokens[i].GetTag() == 28 && tokens[i + 1].GetTag() == 24) // ; 而且含有};
            {
            }
            else {
                error = "复合句末尾缺少}";
            }
        }
    }

    // 验证：〈语句表〉→〈执行句〉;〈语句表〉｜〈执行句〉
    // 语句表SentenceList()
    // 〈语句表〉→〈执行句〉；〈语句表〉｜〈执行句〉
    void SentenceList()
    {
        S s =  S();
        ExecuteSentence(s);// 执行执行句
        if (error == "") {
            Next();
            if (tokens[i].GetTag() == 24) // 若为分号，继续循环执行语句表
            {
                Next();
                SentenceList();
            }
			Next(); // YZD
        }
    }

    // 执行句ExecuteSentence()
    // 〈执行句〉→〈简单句〉｜〈结构句〉
    // 〈简单句〉→〈赋值句〉
    // 〈赋值句〉→〈变量〉=〈表达式〉
    // 〈结构句〉→〈复合句〉｜〈if句〉｜〈WHILE句〉
    // 〈复合句〉→ {〈语句表〉}
    void ExecuteSentence(S s)
    {
        if (tokens[i].GetTag() == 2)// 如果是标识符，为简单句
        {
            Next();
            AssignSentence();// 执行赋值句
        }
        else if (tokens[i].GetTag() == 27 || tokens[i].GetTag() == 7
                 || tokens[i].GetTag() == 8)// 如果是{或while或if
        {
			printf("testing tokens[%d]=%d\n",i,tokens[i].GetTag());
            StructSentence(s);// 执行结构句
        }
        else// 否则返回到前一个字符
        {
            Before();
        }
    }


    // 赋值句AssignSentence()
    // 〈赋值句〉→〈变量〉=〈表达式〉
    void AssignSentence() {
        if (tokens[i].GetTag() == 14)// 如果为=
        {
			Token temp = tokens[i - 1];// temp记录上一个token文件项的名字(标示符)
            Next();
            Expression(); // 执行表达式
			stringstream ss;
			ss.clear();
			ss<< temp.GetAddress();
            gen("0", tt, "555", ss.str());// 生成四元式，即temp=tt
        }
        else {
            error = "赋值句变量后缺少=";
        }
    }



    //  表达式Expression()
    // 〈表达 式〉→〈算术表达式〉｜〈布尔表达式〉
    // 〈算术表达式〉→〈算术表达式〉＋〈项〉｜〈算术表达式〉－〈项〉｜〈项〉
    // 〈布尔表达式〉→〈布尔表达式〉or〈布尔项〉｜〈布尔项〉
    void Expression() {
        if (tokens[i].GetTag() == 5)
        {
            // 如果是false或true或单词为保留字且在符号表中的类型为bool型
            E e = E();
            BoolExpression(e);// 执行布尔表达式
        }
        else if (tokens[i].GetTag() == 6) {
            // 如果是false或true或单词为保留字且在符号表中的类型为bool型
            E e = E();
            BoolExpression(e);// 执行布尔表达式
        }
        else if ((tokens[i].GetAddress() != -1) && (symbols[tokens[i].GetAddress()-1].GetType() == 4)) {
            // 如果是false或true或单词不为保留字且在符号表中的类型为bool型
            E e = E();
            BoolExpression(e);// 执行布尔表达式
        }
        else {
            ArithmeticExpression();// 执行算术表达式
        }
    }

    // 布尔表达式BoolExpression()
    // 〈布尔表达式〉→〈布尔表达式〉or〈布尔项〉｜〈布尔项〉
    // 〈布尔项〉→〈布尔项〉and〈布尔因子〉｜〈布尔因子〉
    // 〈布尔因子〉→ not〈布尔因子〉｜〈布尔量〉
    void BoolExpression(E e) {
        E e1 = E();
        BoolItem(e1);// 执行布尔项
        if (error == "") {
            Next();
            if (tokens[i].GetTag() == 22)// 如果为or
            {
                int m = equs.size();// m记录四元式表项的数量值，即地址M.quad
                E e2 =  E();
                Next();
                BoolExpression(e2);// 执行布尔表达式
                //E->E1 or ME2
                //即e.trueExit={e1.trueExit,e2.trueExit};即e.falseExit={e2.falseExit}
                //将e1中赋值给e
                e.trueExit.assign(e1.trueExit.begin(),e1.trueExit.end());
                //将e2加入到e后面
                e.trueExit.insert(e.trueExit.end(),e2.trueExit.begin(),e2.trueExit.end());

                e.falseExit = e2.falseExit;
                for (int k : e.falseExit)
                {
                    BackPatch(k, m);// 用m=M.quad回填k=e1.falseExit
                }
            }
            else// 如果不是or，则e.trueExit=e1.trueExit;e.falseExit=e1.falseExit
            {
                e = e1;
                Before();
            }
        }
        else {
            e = e1;
        }
    }

    // 布尔项BoolItem(ref E e)
    // 〈布尔项〉→〈布尔项〉and〈布尔因子〉｜〈布尔因子〉
    void BoolItem(E e) {
        E e1 =  E();
        BoolFactor(e1);// 执行布尔因子
        if (error == "") {
            Next();
            if (tokens[i].GetTag() == 21)// 如果是and
            {
                Next();
                //E->E1 and ME2
                int m = equs.size();
                E e2 =  E();
                BoolItem(e2);// 执行布尔项
                e.trueExit = e2.trueExit; // 即e.trueExit=e2.trueExit

                //将e1中赋值给e
                e.falseExit.assign(e1.falseExit.begin(), e1.falseExit.end());
                //将e2加入到e后面
                e.falseExit.insert(e.falseExit.end(), e2.falseExit.begin(), e2.falseExit.end());
                // 即e.falseExit={e1.falseExit,e2.falseExit}

                for (int k : e.trueExit) {
                    BackPatch(k, m);// 执行回填函数
                }
            }
            else// 如果不是and,则e.trueExit=e1.trueExit,e.falseExit=e1.falseExit
            {
                e = e1;
                Before();
            }
        }
    }

    // 布尔因子BoolFactor()
    // 〈布尔因子〉→not〈布尔因子〉｜〈布尔量〉
    // 〈布尔量〉→〈布尔常数〉｜〈标识符〉｜（〈布尔表达式〉）｜〈关系表达式〉
    void BoolFactor(E e) {
        if (tokens[i].GetTag() == 23)// 如果是not
        {
            Next();
            //E->not E1
            E e1 =  E();
            BoolFactor(e1);// 执行布尔因子
            e.trueExit = e1.falseExit;// 即e.trueExit=e1.trueExit;e.falseExit=e1.falseExit
            e.falseExit = e1.trueExit;
        }
        else// 不是not
        {
            E e1 =  E();
            BoolValue(e1);// 执行布尔量
            e = e1;
        }
    }

    // 布尔量BoolValue()
    //〈布尔量〉→〈布尔常数〉｜〈标识符〉｜（〈布尔表达式〉）｜〈关系表达式〉
    //〈关系表达式〉→〈标识符〉〈关系运算符〉〈标识符〉
    //〈关系运算符〉→〈|〈＝ | ＝| 〉＝| 〉|〈〉
    void BoolValue(E e) {
        // 如果是true或false，则e.trueExit=equs.size()，e.falseExit=equs.size()+1
        if (tokens[i].GetTag() == 5 || tokens[i].GetTag() == 6)
        {
            // equs.size()指向下一条将要产生但尚未产生的四元式的编号，其初值为1，每执行一次emit过程，equs.size()将会自动加1
            e.trueExit.push_back(equs.size());
            e.falseExit.push_back(equs.size() + 1);

            // tt = tokens[i].GetName();// tt记录名字
			int addrs = AddressRegisteredSymbol(tokens[i].GetName(),SymbolType::CONST);
			stringstream ss;
			ss.clear();
			ss << addrs;
			tt = ss.str();
        }
        else if (tokens[i].GetTag() == 2)// 如果是标识符
        {
            Next();
            if (tokens[i].GetTag() == 15 || tokens[i].GetTag() == 16
                    || tokens[i].GetTag() == 17 || tokens[i].GetTag() == 18
                    || tokens[i].GetTag() == 19 || tokens[i].GetTag() == 20) {// 如果为<、<=、=、>=、>、<>，即关系表达式
                Next();
                if (tokens[i].GetTag() == 2)// 如果是标识符,则e.trueExit=equs.Count，e.falseExit=equs.Count+1
                {
                    e.trueExit.push_back(equs.size());
                    e.falseExit.push_back(equs.size() + 1);

                    if (tokens[i-1].GetName().compare("<")==0)
                        gen("5" , tokens[i-2].GetName(),
                            tokens[i].GetName(), "0");// 生成四元式，即a<b的四元式为(j<,a,b,0)
                    else if (tokens[i-1].GetName().compare("<=") == 0)
                        gen("6", tokens[i-2].GetName(),
                            tokens[i].GetName(), "0");// 生成四元式，即a<b的四元式为(j<=,a,b,0)
                    else if (tokens[i-1].GetName().compare(">") == 0)
                        gen("7", tokens[i-2].GetName(),
                            tokens[i].GetName(), "0");// 生成四元式，即a<b的四元式为(j>,a,b,0)
                    else if (tokens[i-1].GetName().compare(">=") == 0)
                        gen("8", tokens[i-2].GetName(),
                            tokens[i].GetName(), "0");// 生成四元式，即a<b的四元式为(j>=,a,b,0)
                    else if (tokens[i-1].GetName().compare("==") == 0)
                        gen("9", tokens[i-2].GetName(),
                            tokens[i].GetName(), "0");// 生成四元式，即a<b的四元式为(j==,a,b,0)
                    else if (tokens[i-1].GetName().compare("!=") == 0)
                        gen("11", tokens[i-2].GetName(),
                            tokens[i].GetName(), "0");// 生成四元式，即a<b的四元式为(j!=,a,b,0)
                    gen("10", "555", "555", "0");
                }
                else {
                    Before();
                }
            }
            else// 只有一个标识符，说明该标识符为布尔型，则e.trueExit=equs.size()，e.falseExit=equs.size()+1
            {
                Before();
                e.trueExit.push_back(equs.size());
                e.falseExit.push_back(equs.size() + 1);
                gen("jnz", tokens[i].GetName(), "555", "0");// 生成四元式,即E—>a的四元式为(jnz,a,_,0)
                gen("10", "555", "555", "0");
                Next();
            }

        }
        else if (tokens[i].GetTag() == 25)// 如果为（-->（〈布尔表达式〉）
        {
			
			Next();
            E e1 =  E();// 定义E—>(E1)
            BoolExpression(e1);// 执行布尔表达式
            e.trueExit = e1.trueExit;
            e.falseExit = e1.falseExit;
			Next();
            if (tokens[i].GetTag() == 26)// 如果为），返回
            {
                return;
            }
        }
    }

    // 算术表达式ArithmeticExpression()
    // 〈算术表达式〉→〈算术表达式〉＋〈项〉｜〈算术表达式〉－〈项〉｜〈项〉
    // 〈项〉→〈项〉＊〈因子〉｜〈项〉／〈因子〉｜〈因子〉
    // 〈因子〉→〈算术量〉｜（〈算术表达式〉）
    // 〈算术量〉→〈标识符〉｜〈整数〉｜〈实数〉
    void ArithmeticExpression() {
        Item();// 执行 项
        if (error == "") {
            Next();
            if (tokens[i].GetTag() == 10 || tokens[i].GetTag() == 11)// 如果为+或-
            {
                string temp[] = { tokens[i-1].GetName(),tokens[i].GetName() };// temp记录运算符和它前面的变量名字
				Token a0 = tokens[i-1];
				Token a1 = tokens[i];
                if (tokens[i - 1].GetTag() == 26)// 符号为）
                {
                    temp[0] = tt;
                }
                Next();
                ArithmeticExpression();// 执行算术表达式
                // if (temp[1].compare("+") == 0)
                //     gen("1", temp[0], tt, NewTemp());// 生成四元式，即x=y+z的四元式为(+,y,z,T1)
                // else if (temp[1].compare("-") == 0)
                //     gen("2", temp[0], tt, NewTemp());// 生成四元式，即x=y-z的四元式为(-,y,z,T1)
                // tt = "T" + (ti - 1); //TODO
				int addrs  = 555; //
				int addr1 = 555; // 
                if (temp[1].compare("+") == 0)
                {
				stringstream ss;
				ss.clear();
					string ntmp = NewTemp();
					add_symbol(Symbol(SymbolType::IDENTIFIER,ntmp));
					addrs = AddressRegisteredSymbol(ntmp,SymbolType::IDENTIFIER); // Find temp symbol address.
					addr1 = a0.GetAddress();
					ss<<addr1;
					stringstream sss;
					sss.clear();
					sss<<addrs;
					gen("1", ss.str(), tt, sss.str()); // 生成四元式，即x=y+z的四元式为(+,y,z,T1)
					ss.clear();
				}
                else  if (temp[1].compare("-") == 0)
                {
				stringstream ss;
				ss.clear();
					string ntmp =NewTemp();
					add_symbol(Symbol(SymbolType::IDENTIFIER,ntmp));
					addrs = AddressRegisteredSymbol(ntmp,SymbolType::IDENTIFIER);
					addr1 = a0.GetAddress();
					ss<<addr1;
					stringstream sss;
					sss.clear();
					sss<<addrs;
					gen("2", ss.str(), tt, sss.str()); // 生成四元式，即x=y-z的四元式为(-,y,z,T1)
					ss.clear();
				}
				stringstream ss;
				ss << addrs;
				tt = ss.str();




            }
            else {
                Before();
                return;
            }
        }
        else {
            return;
        }
    }

    // 项Item()
    // 〈项〉→〈项〉＊〈因子〉｜〈项〉／〈因子〉｜〈因子〉
    void Item() {
        Factor();// 执行因子
        if (error == "") {
            Next();
            if (tokens[i].GetTag() == 12 || tokens[i].GetTag() == 13)// 如果为*或/
            {
                string temp[] = { tokens[i-1].GetName(), tokens[i].GetName() };// temp记录运算符和它前面的变量名字
				Token a0 = tokens[i-1];
                if (tokens[i - 1].GetTag() == 26)// token文件的上一表项为）
                {
                    temp[0] = tt;
                }
                Next();
                Item();// 执行项
				int addrs  = 555; //
				int addr1 = 555; // 
                if (temp[1].compare("*") == 0)
                {
				stringstream ss;
				ss.clear();
					string ntmp = NewTemp();
					add_symbol(Symbol(SymbolType::IDENTIFIER,ntmp));
					addrs = AddressRegisteredSymbol(ntmp,SymbolType::IDENTIFIER); // Find temp symbol address.
					addr1 = a0.GetAddress();
					ss<<addr1;
					stringstream sss;
					sss.clear();
					sss<<addrs;
					gen("3", ss.str(), tt, sss.str()); // 生成四元式，即x=y*z的四元式为(*,y,z,T1)
					ss.clear();
				}
                else  if (temp[1].compare("/") == 0)
                {
				stringstream ss;
				ss.clear();
					string ntmp =NewTemp();
					add_symbol(Symbol(SymbolType::IDENTIFIER,ntmp));
					addrs = AddressRegisteredSymbol(ntmp,SymbolType::IDENTIFIER);
					addr1 = a0.GetAddress();
					ss<<addr1;
					stringstream sss;
					sss.clear();
					sss<<addrs;
					gen("4", ss.str(), tt, sss.str()); // 生成四元式，即x=y/z的四元式为(/,y,z,T1)
					ss.clear();
				}
				stringstream ss;
				ss << addrs;
				tt = ss.str();
            }
            else {
                Before();
            }
        }
        else {
        }
    }

    // 因子Factor()
    // 〈因子〉→〈算术量〉｜（〈算术表达式〉）
    void Factor() {
        if (tokens[i].GetTag() == 25)// 如果为（
        {
            Next();
            ArithmeticExpression();// 执行算数表达式
            Next();
            if (tokens[i].GetTag() == 26)// 如果为）
            {
            }
            else {
                error = "因子中算数表达式缺少）";
            }
        }
        else {
            CalQua();// 执行算术量
        }
    }

    // 算术量CalQua()
    // 〈算术量〉→〈标识符〉｜〈整数〉｜〈实数〉
    void CalQua() {
        if (tokens[i].GetTag() == 2 || tokens[i].GetTag() == 1)// 标识符、整数、实数
        {
            //tt = tokens[i].GetName();// 记录变量名
			int addrs = AddressRegisteredSymbol(tokens[i].GetName(),SymbolType::CONST);
			stringstream ss;
			ss.clear();
			ss << addrs;
			tt = ss.str();
        }
        else {
            error = "算术量出错";
        }
    }

    // 结构句StructSentence(S s)
    // 〈结构句〉→〈复合句〉｜〈if句〉｜〈WHILE句〉
    // 〈复合句〉→ {〈语句表〉}
    void StructSentence(S s) {
        if (tokens[i].GetTag() == 27)// 如果是{
        {
            Next();
            checkComplex();// 执行复合句
        }
        else if (tokens[i].GetTag() == 8)// if
        {
            Next();
            IfSentence(s);// 执行if语句
        }
        else if (tokens[i].GetTag() == 7)// while
        {
            Next();
            WhileSentence(s);// 执行while语句
        }
    }

    // if语句IfSent(S s)
    //〈if句〉→if〈布尔表达式〉then〈执行句〉|
    // if〈布尔表达式〉then〈执行句〉else〈执行句〉
    void IfSentence(S s) {
        E e =  E();
        BoolExpression(e);// 执行布尔表达式
        if (error == "") {
            Next();
			 Next(); // YZD
            if (tokens[i].GetTag() == 31)// then
            {
                int m1 = equs.size();
                S s1 = S();
                Next();
                ExecuteSentence(s1);// 执行执行句
                Next();
                if (tokens[i].GetTag() == 9)// 如果是else，即控制语句产生式为S—>if E then
                    // M1S1N else
                    // M2S2，则s.next={s1.next,n.next,s2.next},并回填e.t和e.falseExit
                {
                    S n =  S();// 若N—>ε,n.next=equs.Count,并生成四元式(j,_,_,0)
                    n.next.push_back(equs.size());
                    gen("10", "555", "555", "0");
                    S s2 = S();
                    int m2 = equs.size();
                    Next();
                    ExecuteSentence(s2);// 执行执行句
                    s.next = s1.next;

                    //将 n.next中赋值给 s.next
                    s.next.assign(n.next.begin(), n.next.end());
                    //将 s2.next加入到 s.next后面
                    s.next.insert(s.next.end(), s2.next.begin(), s2.next.end());



                    for (int k : e.trueExit) {
                        BackPatch(k, m1);// 执行回填函数
                    }
                    for (int k : e.falseExit) {
                        BackPatch(k, m2);
                    }
                }
                else// 即控制语句产生式为S—>if E then MS1,则s.next={e.falseExit,s1.next},并回填e.trueExit
                {
                    s.next = e.falseExit;

                    //将 s1.next加入到 s.next后面
                    s.next.insert(s.next.end(), s1.next.begin(), s1.next.end());

                    for (int k : e.trueExit) {
                        BackPatch(k, m1);
                    }
                    Before();
                }
            }
            else {
                error = "if...then语句缺少then";
            }
        }
        else {
            error = "if语句布尔表达式出错";
        }
    }

    // while语句WhileSent(ref S s)
    // 〈while句〉→while〈布尔表达式〉do〈执行句〉
    void WhileSentence(S s) {
        int m1 = equs.size();
        E e =  E();
        BoolExpression(e);// 执行布尔表达式
        Next();
        if (tokens[i].GetTag() == 30)// 如果是do，即控制语句产生式为S—>while M1E do
            // M2S1,则s.next=e.falseExit,生成四元式(j,_,_,m1.q),并回填e.t和s1.next
        {
            int m2 = equs.size();
            S s1 = S();
            Next();
            ExecuteSentence(s1);// 执行执行句
            s.next = e.falseExit;
            gen("10", "555", "555", "" + m1);// 生成四元式
            for (int k : e.trueExit) {
                BackPatch(k, m2);
            }
            for (int k : s1.next) {
                BackPatch(k, m1);
            }
        }

    }

	// Summary: Find address of registered symbol and return -1 if none exists.
	// Return: Address of matched symbol or -1 for no match.
	int AddressRegisteredSymbol(string name,SymbolType type)
	{
		for (int index = 0; index < symbols.size(); index++)
        {
			if (memcmp((symbols[index].GetName()).data(), name.data(), name.length()) == 0 && symbols[index].GetType()==type)
            {
				return index;
			}
		}
		return -1;
	}

	void add_symbol(Symbol sym)
	{
		symbols.push_back(sym);
	}
	vector<Symbol> ReturnSymbols()
	{
		return symbols;
	}
};
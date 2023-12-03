
//                                真值表计算器 终版 利用递归和二叉树


#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>
using namespace std;
#define assert(condition, message) \
    do { \
        if (!(condition)) { \
            cerr << "Error: " << (message) << std::endl; \
            exit(1); \
        } \
    } while (false)


enum {
	Not = 5, And = 4, Or = 3, Yunhan = 2, Shuang = 1
};


int combination = 0;  //组合数
vector<int>f_value = {}; //真值表
class Variables
{
public:
	char name = 'p';
	bool value = false;
	Variables(char name)
	{
		this->name = name;
		value = true;
	}
	Variables(bool value)
	{
		this->value = value;
	}
	void change_value(int n)
	{
		if (n == 1)
			value = true;
		else if (n == 0)
			value = false;
	}
	bool operator==(const Variables& other)
	{
		if (this->name == other.name)
			return true;
		return false;
	}
};
struct bina_tree //二叉树
{
	bina_tree* leftchild;
	bina_tree* rightchild;
	char data;
};
vector<Variables> var = {};  //变量列表
bool var_check(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;
	return false;
}

int op_check(char c)
{
	switch (c) {
	case '|': return Or;
	case '&': return And;
	case '!': return Not;
	case '^': return Yunhan;
	case '~': return Shuang;
	default:return 0;
	}
}
int op_compare(char c1, char c2)
{
	int x = op_check(c1), y = op_check(c2);
	if (x > y) return 1;
	else return 0;
}
bool in_check(string input)//1.exit 2.括号相等 3.变量合法性 4.联结词 
{
	if (input == "exit")
	{
		cout << "感谢您使用本程序，我们下次再见！" << endl;
		exit(1);
	}
	else
	{
		int left_bracket = 0, right_bracket = 0, len = input.size();
		int last = len - 1;
		for (int i = 0;i < len - 1;i++)
		{
			if (i != len - 1)
			{
				int first = input[i], second = input[i + 1];
				assert(var_check(first) || op_check(first) || first == '(' || first == ')', "非法输入.");

				if (i == 0)
					assert(first != '~' && first != '^' && first != '|' && first != '&' && first != ')', "首位字符不合规.");
				if (first == '(')
				{
					left_bracket++;
					assert(second != '~' && second != '^' && second != '&' && second != '|' && second != ')', "命题公式逻辑错误.");
				}
				if (second == ')')
				{
					right_bracket++;
					assert(first != '~' && first != '^' && first != '|' && first != '&' && first != '(', "命题公式逻辑错误.");
				}
				if (var_check(first))
					assert(!var_check(second) && second != '!', "变量格式有误.");
				if (op_check(first) && first != '!')
					assert(var_check(second) || second == '(' || second == '!', "联结词非法使用！");
				if (first == '!')
					assert(var_check(second), "联结词非法使用！");
			}

		}

		assert(var_check(input[last]) || input[last] == ')', "末位字符不合规.");
		assert(left_bracket == right_bracket, "左右括号数量不等");
	}
	return true;
};
void var_save(string input) //保存变量
{

	var.clear();
	for (int i = 0;i < input.size();i++)
	{

		if (var_check(input[i]))
		{
			Variables var0 = Variables(input[i]);
			if (find(var.begin(), var.end(), var0) == var.end()) //未定义var的等于等于判断!!!!!
				var.push_back(Variables(input[i]));   //默认赋值全假
		}
		else
			continue;
	}
	combination = pow(2, var.size());
}


void build_tree(string s, bina_tree*& father, int begin, int end) //建树
//保持每棵树中括号相等，公式合规
{
	father->leftchild = new bina_tree;
	father->rightchild = new bina_tree;
	if (end == begin)
	{
		father->leftchild = nullptr;
		father->rightchild = nullptr;
		father->data = '?';
		return;
	}
	if (end - begin == 1)
	{

		father->leftchild = nullptr;
		father->rightchild = nullptr;
		father->data = s[begin];

		return;
	}
	else
	{
		int d_bracket = 0;
		int no = -1;
		char preop = '~';
		for (int i = begin;i < end;i++)
		{
			if (s[i] == '(')
				d_bracket++;
			else if (s[i] == ')')
				d_bracket--;
			//判断括号是否匹配
			if (d_bracket == 0 && op_check(s[i]) && (no == -1 || op_compare(s[i], preop)))
			{
				no = i;
				preop = s[i];
			}
		}
		if (no == -1)
		{
			build_tree(s, father, begin + 1, end - 1);

		}
		else {
			build_tree(s, father->leftchild, begin, no);
			build_tree(s, father->rightchild, no + 1, end);
			father->data = s[no];

		}
		return;

	}

}



bool single_operator(bool n1, char op, bool n2)//单个运算符计算
{
	if (op == '!') return (!n2);
	else if (op == '&') return (n1 && n2);
	else if (op == '|') return (n1 || n2);
	else if (op == '^') return ((!n1) || n2);
	else if (op == '~') return (n1 == n2);
}
bool recursive(bina_tree* root)   //递归计算真值
{
	char item;
	item = root->data;
	if (root->data == '?') return false;
	if (var_check(item))
	{
		for (int i = 0;i < var.size();i++)
		{
			if (var[i].name == item)
				return var[i].value;
		}
	}
	return single_operator(recursive(root->leftchild), item, recursive(root->rightchild));

}

void tran_bit(int n) //十进制转二进制 +给变量赋值
{
	int t = 0;
	int m = var.size();
	vector<int>binary(m);
	fill(binary.begin(), binary.end(), 0);
	while (n)
	{
		t = n % 2;
		n /= 2;
		m--;
		binary[m] = t;
	}
	for (int i = 0;i < var.size();i++)  //赋值
	{
		var[i].change_value(binary[i]);
	}
	return;
}
void print_table(string s1) //打印变量和公式(表头)
{
	cout << "真值表： " << endl;
	for (int i = 0;i < var.size();i++)
	{
		cout << var[i].name << "  ";
	}
	cout << "   " << s1 << endl;
}
void cal_print_value(bina_tree* T)   //打印真值
{
	for (int n = 0;n < combination;n++)
	{
		tran_bit(n);
		for (int i = 0;i < var.size();i++)
		{
			cout << var[i].value << "  ";
		}
		bool final_value = recursive(T);
		if (final_value)
		{
			cout << "    1";
			f_value.push_back(1);
		}
		else
		{
			cout << "    0";
			f_value.push_back(0);
		}

		cout << endl;

	}
}

bool isAllOnes(const std::vector<int>& vec, int begin = 0)
{
	for (int i = begin; i < vec.size(); i++) {
		if (vec[i] != 1) {
			return false;
		}
	}
	return true;
}
bool isAllZeros(const std::vector<int>& vec, int begin = 0)
{
	for (int i = begin; i < vec.size(); i++) {
		if (vec[i] != 0) {
			return false;
		}
	}
	return true;
}
void normal_form(char c)
{
	if (c == 'y' || c == 'Y')
	{
		cout << "主析取范式：";
		if (isAllZeros(f_value))
		{
			cout << "空公式";
		}//判断是否为空公式！
		for (int i = 0;i < f_value.size();i++)
		{
			if (f_value[i] == 1)
			{
				tran_bit(i);
				cout << "(";
				for (int j = 0;j < var.size();j++)
				{
					if (var[j].value == 1 && j != var.size() - 1)
						cout << var[j].name << "&";
					else if (var[j].value == 0 && j != var.size() - 1)
						cout << "!" << var[j].name << "&";
					else if (var[j].value == 1 && j == var.size() - 1)
						cout << var[j].name;
					else if (var[j].value == 0 && j == var.size() - 1)
						cout << "!" << var[j].name;
				}
				if (isAllZeros(f_value, i + 1))
				{
					cout << ")";
				}
				else
				{
					cout << ")|";
				}
			}
		}

		cout << endl;
		cout << "主合取范式：";
		if (isAllOnes(f_value))
		{
			cout << "空公式";
		}
		for (int i = 0;i < f_value.size();i++)
		{
			if (f_value[i] == 0)
			{
				tran_bit(i);
				cout << "(";
				for (int j = 0;j < var.size();j++)
				{
					if (var[j].value == 0 && j != var.size() - 1)
						cout << var[j].name << "|";
					else if (var[j].value == 1 && j != var.size() - 1)
						cout << "!" << var[j].name << "|";
					else if (var[j].value == 0 && j == var.size() - 1)
						cout << var[j].name;
					else if (var[j].value == 1 && j == var.size() - 1)
						cout << "!" << var[j].name;
				}
				if (isAllOnes(f_value, i + 1))
				{
					cout << ")";
				}
				else
				{
					cout << ")&";
				}
			}
		}
		//判断是否为空公式！

		cout << endl;

	}
	else if (c == 'n' || c == 'N');
	else
	{
		cout << "输入错误！" << endl;
	};

}
int main()
{
	cout << "联结词： ！:非   &:与   |：或   ^：蕴含   ~：双条件" << endl;
	cout << "注意：变量用单独的小写或者大写字母表示！请使用英语输入法！输入exit退出" << endl;
	while (1)
	{
		f_value.clear();
		cout << "请输入命题公式：(请尽量多添加括号，尤其是在！联结词前，如(!p))" << endl;
		string s1;
		cin >> s1;
		bina_tree* t = new bina_tree;
		if (!t)
		{
			cout << "error!二叉树构建失败！" << endl;
			exit(1);
		}
		in_check(s1);
		var_save(s1);
		build_tree(s1, t, 0, s1.size());
		print_table(s1);
		cal_print_value(t);
		cout << "真值表计算完毕！ " << endl;
		cout << "是否要计算主析取、主和取范式？(y/n)"<<endl;
		char c;
		cin >> c;
		normal_form(c);
	}
	return 0;
}

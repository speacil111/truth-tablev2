
//                                ��ֵ������� �հ� ���õݹ�Ͷ�����


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


int combination = 0;  //�����
vector<int>f_value = {}; //��ֵ��
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
struct bina_tree //������
{
	bina_tree* leftchild;
	bina_tree* rightchild;
	char data;
};
vector<Variables> var = {};  //�����б�
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
bool in_check(string input)//1.exit 2.������� 3.�����Ϸ��� 4.����� 
{
	if (input == "exit")
	{
		cout << "��л��ʹ�ñ����������´��ټ���" << endl;
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
				assert(var_check(first) || op_check(first) || first == '(' || first == ')', "�Ƿ�����.");

				if (i == 0)
					assert(first != '~' && first != '^' && first != '|' && first != '&' && first != ')', "��λ�ַ����Ϲ�.");
				if (first == '(')
				{
					left_bracket++;
					assert(second != '~' && second != '^' && second != '&' && second != '|' && second != ')', "���⹫ʽ�߼�����.");
				}
				if (second == ')')
				{
					right_bracket++;
					assert(first != '~' && first != '^' && first != '|' && first != '&' && first != '(', "���⹫ʽ�߼�����.");
				}
				if (var_check(first))
					assert(!var_check(second) && second != '!', "������ʽ����.");
				if (op_check(first) && first != '!')
					assert(var_check(second) || second == '(' || second == '!', "����ʷǷ�ʹ�ã�");
				if (first == '!')
					assert(var_check(second), "����ʷǷ�ʹ�ã�");
			}

		}

		assert(var_check(input[last]) || input[last] == ')', "ĩλ�ַ����Ϲ�.");
		assert(left_bracket == right_bracket, "����������������");
	}
	return true;
};
void var_save(string input) //�������
{

	var.clear();
	for (int i = 0;i < input.size();i++)
	{

		if (var_check(input[i]))
		{
			Variables var0 = Variables(input[i]);
			if (find(var.begin(), var.end(), var0) == var.end()) //δ����var�ĵ��ڵ����ж�!!!!!
				var.push_back(Variables(input[i]));   //Ĭ�ϸ�ֵȫ��
		}
		else
			continue;
	}
	combination = pow(2, var.size());
}


void build_tree(string s, bina_tree*& father, int begin, int end) //����
//����ÿ������������ȣ���ʽ�Ϲ�
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
			//�ж������Ƿ�ƥ��
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



bool single_operator(bool n1, char op, bool n2)//�������������
{
	if (op == '!') return (!n2);
	else if (op == '&') return (n1 && n2);
	else if (op == '|') return (n1 || n2);
	else if (op == '^') return ((!n1) || n2);
	else if (op == '~') return (n1 == n2);
}
bool recursive(bina_tree* root)   //�ݹ������ֵ
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

void tran_bit(int n) //ʮ����ת������ +��������ֵ
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
	for (int i = 0;i < var.size();i++)  //��ֵ
	{
		var[i].change_value(binary[i]);
	}
	return;
}
void print_table(string s1) //��ӡ�����͹�ʽ(��ͷ)
{
	cout << "��ֵ�� " << endl;
	for (int i = 0;i < var.size();i++)
	{
		cout << var[i].name << "  ";
	}
	cout << "   " << s1 << endl;
}
void cal_print_value(bina_tree* T)   //��ӡ��ֵ
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
		cout << "����ȡ��ʽ��";
		if (isAllZeros(f_value))
		{
			cout << "�չ�ʽ";
		}//�ж��Ƿ�Ϊ�չ�ʽ��
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
		cout << "����ȡ��ʽ��";
		if (isAllOnes(f_value))
		{
			cout << "�չ�ʽ";
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
		//�ж��Ƿ�Ϊ�չ�ʽ��

		cout << endl;

	}
	else if (c == 'n' || c == 'N');
	else
	{
		cout << "�������" << endl;
	};

}
int main()
{
	cout << "����ʣ� ��:��   &:��   |����   ^���̺�   ~��˫����" << endl;
	cout << "ע�⣺�����õ�����Сд���ߴ�д��ĸ��ʾ����ʹ��Ӣ�����뷨������exit�˳�" << endl;
	while (1)
	{
		f_value.clear();
		cout << "���������⹫ʽ��(�뾡����������ţ��������ڣ������ǰ����(!p))" << endl;
		string s1;
		cin >> s1;
		bina_tree* t = new bina_tree;
		if (!t)
		{
			cout << "error!����������ʧ�ܣ�" << endl;
			exit(1);
		}
		in_check(s1);
		var_save(s1);
		build_tree(s1, t, 0, s1.size());
		print_table(s1);
		cal_print_value(t);
		cout << "��ֵ�������ϣ� " << endl;
		cout << "�Ƿ�Ҫ��������ȡ������ȡ��ʽ��(y/n)"<<endl;
		char c;
		cin >> c;
		normal_form(c);
	}
	return 0;
}

#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include <conio.h>
#include <fstream>
using namespace std;

string tolower(string str) { //функция для приведения русских слов к нижнему регистру
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == 'Ё')
			str[i] += 16;
		else if (str[i] <= -33 && str[i] >= -64)
			str[i] += 32;
	}
	return str;
}

string first_upper(string str) { //функция для приведения первой буквы слова к верхнему регистру
	if (str[0] == 'ё')
		str[0] -= 16;
	else
		str[0] -= 32;
	return str;
}

string erase_spaces(string name) { //функция для стирания пробелов в начале и конце слова
	while (name.size() != 1 && (name[0] == ' ' || name[0] == '\t')) {
		name.erase(0, 1);
	}
	while (name.size() != 1 && (name[name.size() - 1] == ' ' || name[name.size() - 1] == '\t')) {
		name.erase(name.size() - 1, 1);
	}
	return name;
}

bool test_str(string name) {//функция для проверки входящей строки на русское слово
	if (name.empty()) {
		return false;
	}
	if (name.size() <= 1) {
		return false;
	}
	if (name.size() > 15) {
		return false;
	}
	for (int i = 0; i < name.size(); i++) {
		if ((name[i] > -65 && name[i] < 0) || (name[i] == -88) || (name[i] == -72) || (name[i] == 32));
		else {
			return false;
		}
	}
	return true;
}

bool test_float(string name) { //функция для проверки входящей строки на число
	int flag = 0;
	if (name.size() <= 3) {
		return false;
	}
	for (int i = 0; i < name.size(); i++) {
		if (i == name.size() - 3) {
			if (name[i] == '.')
				flag++;
		}
		else if (name[i] <= '9' && name[i] >= '0')
			flag++;
	}
	if (flag == name.size())
		return true;
	return false;
}

string input_str(string name) {//функция для ввода строки нужного формата (1<str<16)
	while (1) {
		getline(cin, name);
		if (name.empty()) {
			cout << "Ничего не было введено\n";
			continue;
		}
		name = erase_spaces(name);
		if (name.size() <= 1) {
			cout << "Название было введено некорректно\n";
			continue;
		}
		if (name.size() > 15) {
			cout << "Размер строки превышает 15 символов\n";
			continue;
		}
		for (int i = 0; i < name.size(); i++) {
			if ((name[i] > -65 && name[i] < 0) || (name[i] == -88) || (name[i] == -72) || (name[i] == 32));
			else {
				cout << "Название было введено не кириллицей\n";
				break;
			}
			name = first_upper(tolower(name));
			return name;
		}
	}
}

float input_float(string x) { //функция для ввода положительного числа
	while (1) {
		getline(cin, x);
		if (x.empty()) {
			cout << "Ничего не было введено\n";
			continue;
		}
		x = erase_spaces(x);
		if (x.size() <= 3) {
			cout << "\nЧисло введено в неверном формате\n";
			continue;
		}
		int flag = 0;
		for (int i = 0; i < x.size(); i++) {
			if (i == x.size() - 3) {
				if (x[i] == '.')
					flag++;
			}
			else if (x[i] <= '9' && x[i] >= '0')
				flag++;
		}
		if (flag != x.size()) {
			cout << "Цена была введена некорректно\n";
			continue;
		}
		if (stof(x) < 0) {
			cout << "Введено число, меньше либо равное нулю\n";
			continue;
		}
		return stof(x);
	}
}



struct PRICE {
	PRICE* prev;
	PRICE* next;
	string name;
	string shop;
	float cost;
	PRICE() : name(""), shop(""), cost(0), prev(nullptr), next(nullptr) {};
	PRICE(string _name, string _shop, float _cost) : name(_name), shop(_shop), cost(_cost), prev(nullptr), next(nullptr) {};
	void print_elem();
};

void PRICE::print_elem() { //функция для вывода одного элемента БД
	cout << "Товар: " << setw(15) << name << ", магазин: " << setw(15) << shop << ", цена : " << setw(15) << cost << endl;
}



class BD {
private:
	PRICE* first;
	PRICE* last;
public:
	BD() : first(nullptr), last(nullptr) {};
	~BD();
	bool is_empty();
	int get_size();
	void download();
	void upload();
	void print_BD();
	void add(string name, string shop, float cost);
	PRICE* update(PRICE* cur);
	void sort();
	BD* search_list(BD* search);
	PRICE* split(string str, char delim = ';');
	void push(PRICE* cur);
	void search();
	bool search_to_add(PRICE* new_node);
	void del(PRICE* cur);
	void swap(PRICE* elem1, PRICE* elem2);
	void clear_all();
};

BD::~BD() {
	while (first) {
		last = first->next;
		delete first;
		first = last;
	}
}

bool BD::is_empty() { //проверка, пуст ли список
	return first == nullptr;
}

int BD::get_size() { //получить размер списка
	PRICE* cur = first;
	int size = 0;
	while (cur) {
		size++;
		cur = cur->next;
	}
	return size;
}

void BD::add(string name, string shop, float cost) { //добавить элемент в конец списка (создав новый)
	PRICE* cur = new PRICE(name, shop, cost);
	if (is_empty()) {
		first = cur;
		last = cur;
		return;
	}
	if (search_to_add(cur)) {
		cout << "Введенный элемент уже есть в базе данных\n";
		return;
	}
	last->next = cur;
	cur->prev = last;
	last = cur;
}

void BD::push(PRICE* cur) { //добавить входящий элемент в конец списка
	if (is_empty()) {
		first = cur;
		last = cur;
		return;
	}
	if (search_to_add(cur)) {
		return;
	}
	last->next = cur;
	cur->prev = last;
	last = cur;
}

void BD::print_BD() { //напечатать список
	PRICE* cur = first;
	cout << "+----------------------------------------------+" << endl;
	cout << "|     Товар     |    Магазин    |     Цена     |" << endl;
	cout << "+----------------------------------------------+" << endl;
	while (cur) {
		cout  <<'|'<< setw(15) << cur->name <<'|'<< setw(15) << cur->shop <<'|'<< setw(14) << cur->cost <<'|'<< endl;
		cout << "+----------------------------------------------+" << endl;
		cur = cur->next;
	}
}


void BD::search() { //удаление, изменение по товару или магазину
	if (is_empty()) {
		cout << "База данных пуста\n";
		return;
	}
	string name, shop;
	float cost = 0;
	int flag = 0, flag1 = 0;
	PRICE* cur = first;
	cout << "0 - поиск по названию товара\n1 - поиск по названию магазина\n2 - поиск по цене\nдругой символ - выход\n";
	switch (_getch()) {
	case '0':
		cout << "Введите название товара\n";
		name = tolower(input_str(name));
		while (cur) {
			if (tolower((cur->name)) == name) {
				flag++;
			}
			cur = cur->next;
		}
		if (flag == 1) {
			cout << "\nНайденный элемент:\n";
			cur = first;
			while (cur) {
				if (tolower((cur->name)) == name) {
					cur->print_elem();
					cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
					switch (_getch()) {
					case '0': {
						cout << "\nВыбрано удаление элемента\n";
						del(cur);
						return;
						break;
					}
					case '1':
						cout << "\nВыбрано изменение элемента\n";
						cur = update(cur);
						return;
						break;
					default:
						break;
					}
				}
				cur = cur->next;
			}

		}
		else if (flag > 1) {
			cout << "\nНайдено несколько элементов:\n";
			cur = first;
			while (cur) {
				if (tolower((cur->name)) == name) {
					cur->print_elem();
				}
				cur = cur->next;
			}
			cout << "\nУточните поиск:\n0 - поиск по названию магазина\n1 - поиск по цене\nдругой символ - выход\n";
			switch (_getch()) {
			case '0':
				cout << "\nВведите название магазина:\n";
				shop = tolower(input_str(shop));
				cur = first;
				while (cur) {
					if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop)) {
						flag1++;
					}
					cur = cur->next;
				}
				if (flag1 == 1) {
					cout << "\nНайденный элемент:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop)) {
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}

				}
				else if (flag1 > 1) {
					cout << "\nНайдено несколько элементов:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop)) {
							cur->print_elem();
						}
						cur = cur->next;
					}
					cout << "\nУточните поиск по цене\n";
					cost = input_float(name);
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop) && (cur->cost == cost)) {
							cout << "\nНайденный элемент: \n";
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}
				}
				else if (!flag1) {
					cout << "\nНужный элемент не был найден\n";
				}
				break;
			case '1':
				cout << "\nВведите цену:\n";
				cost = input_float(name);
				cur = first;
				while (cur) {
					if ((tolower(cur->name) == name) && (cur->cost == cost)) {
						flag1++;
					}
					cur = cur->next;
				}
				if (flag1 == 1) {
					cout << "\nНайденный элемент:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (cur->cost == cost)) {
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}

				}
				else if (flag1 > 1) {
					cout << "\nНайдено несколько элементов:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (cur->cost == cost)) {
							cur->print_elem();
						}
						cur = cur->next;
					}
					cout << "\nУточните поиск по названию магазина:\n";
					shop = input_float(shop);
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop) && (cur->cost == cost)) {
							cout << "\nНайденный элемент: \n";
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}
				}
				else if (!flag1) {
					cout << "\nНужный элемент не был найден\n";
				}
				break;
			}
		}

		break;
	case '1':
		cout << "Введите название магазина\n";
		shop = tolower(input_str(shop));
		while (cur) {
			if (tolower((cur->shop)) == shop) {
				flag++;
			}
			cur = cur->next;
		}
		if (flag == 1) {
			cout << "\nНайденный элемент:\n";
			cur = first;
			while (cur) {
				if (tolower((cur->shop)) == shop) {
					cur->print_elem();
					cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
					switch (_getch()) {
					case '0': {
						cout << "\nВыбрано удаление элемента\n";
						del(cur);
						return;
						break;
					}
					case '1':
						cout << "\nВыбрано изменение элемента\n";
						cur = update(cur);
						return;
						break;
					default:
						break;
					}
				}
				cur = cur->next;
			}

		}
		else if (flag > 1) {
			cout << "\nНайдено несколько элементов:\n";
			cur = first;
			while (cur) {
				if (tolower((cur->shop)) == shop) {
					cur->print_elem();
				}
				cur = cur->next;
			}
			cout << "\nУточните поиск:\n0 - поиск по названию товара\n1 - поиск по цене\nдругой символ - выход\n";
			switch (_getch()) {
			case '0':
				cout << "\nВведите название товара:\n";
				name = tolower(input_str(name));
				cur = first;
				while (cur) {
					if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop)) {
						flag1++;
					}
					cur = cur->next;
				}
				if (flag1 == 1) {
					cout << "\nНайденный элемент:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop)) {
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}

				}
				else if (flag1 > 1) {
					cout << "\nНайдено несколько элементов:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop)) {
							cur->print_elem();
						}
						cur = cur->next;
					}
					cout << "\nУточните поиск по цене\n";
					cost = input_float(name);
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop) && (cur->cost == cost)) {
							cout << "\nНайденный элемент: \n";
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}
				}
				else if (!flag1) {
					cout << "\nНужный элемент не был найден\n";
				}
				break;
			case '1':
				cout << "\nВведите цену:\n";
				cost = input_float(name);
				cur = first;
				while (cur) {
					if ((tolower(cur->shop) == shop) && (cur->cost == cost)) {
						flag1++;
					}
					cur = cur->next;
				}
				if (flag1 == 1) {
					cout << "\nНайденный элемент:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->shop) == shop) && (cur->cost == cost)) {
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}

				}
				else if (flag1 > 1) {
					cout << "\nНайдено несколько элементов:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->shop) == shop) && (cur->cost == cost)) {
							cur->print_elem();
						}
						cur = cur->next;
					}
					cout << "\nУточните поиск по названию товара:\n";
					name = input_float(name);
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop) && (cur->cost == cost)) {
							cout << "\nНайденный элемент: \n";
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}
				}
				else if (!flag1) {
					cout << "\nНужный элемент не был найден\n";
				}
				break;
			}
		}

		break;
	case '2':
		cout << "Введите цену\n";
		cost = input_float(name);
		while (cur) {
			if (cur->cost == cost) {
				flag++;
			}
			cur = cur->next;
		}
		if (flag == 1) {
			cout << "\nНайденный элемент:\n";
			cur = first;
			while (cur) {
				if (cur->cost == cost) {
					cur->print_elem();
					cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
					switch (_getch()) {
					case '0': {
						cout << "\nВыбрано удаление элемента\n";
						del(cur);
						return;
						break;
					}
					case '1':
						cout << "\nВыбрано изменение элемента\n";
						cur = update(cur);
						return;
						break;
					default:
						break;
					}
				}
				cur = cur->next;
			}

		}
		else if (flag > 1) {
			cout << "\nНайдено несколько элементов:\n";
			cur = first;
			while (cur) {
				if (cur->cost == cost) {
					cur->print_elem();
				}
				cur = cur->next;
			}
			cout << "\nУточните поиск:\n0 - поиск по названию товара\n1 - поиск по названию магазина\nдругой символ - выход\n";
			switch (_getch()) {
			case '0':
				cout << "\nВведите название товара:\n";
				name = tolower(input_str(name));
				cur = first;
				while (cur) {
					if ((tolower(cur->name) == name) && (cur->cost == cost)) {
						flag1++;
					}
					cur = cur->next;
				}
				if (flag1 == 1) {
					cout << "\nНайденный элемент:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (cur->cost == cost)) {
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}

				}
				else if (flag1 > 1) {
					cout << "\nНайдено несколько элементов:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (cur->cost == cost)) {
							cur->print_elem();
						}
						cur = cur->next;
					}
					cout << "\nУточните поиск по названию магазина\n";
					shop = tolower(input_str(name));
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop) && (cur->cost == cost)) {
							cout << "\nНайденный элемент: \n";
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}
				}
				else if (!flag1) {
					cout << "\nНужный элемент не был найден\n";
				}
				break;
			case '1':
				cout << "\nВведите название магазина:\n";
				shop = tolower(input_float(name));
				cur = first;
				while (cur) {
					if ((tolower(cur->shop) == shop) && (cur->cost == cost)) {
						flag1++;
					}
					cur = cur->next;
				}
				if (flag1 == 1) {
					cout << "\nНайденный элемент:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->shop) == shop) && (cur->cost == cost)) {
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}

				}
				else if (flag1 > 1) {
					cout << "\nНайдено несколько элементов:\n";
					cur = first;
					while (cur) {
						if ((tolower(cur->shop) == shop) && (cur->cost == cost)) {
							cur->print_elem();
						}
						cur = cur->next;
					}
					cout << "\nУточните поиск по названию товара:\n";
					name = input_float(name);
					cur = first;
					while (cur) {
						if ((tolower(cur->name) == name) && (tolower(cur->shop) == shop) && (cur->cost == cost)) {
							cout << "\nНайденный элемент: \n";
							cur->print_elem();
							cout << "Хотите удалить или обновить?\n0 - удалить\n1 - обновить\nдругой символ - нет\n";
							switch (_getch()) {
							case '0': {
								cout << "\nВыбрано удаление элемента\n";
								del(cur);
								return;
								break;
							}
							case '1':
								cout << "\nВыбрано изменение элемента\n";
								cur = update(cur);
								return;
								break;
							default:
								break;
							}
						}
						cur = cur->next;
					}
				}
				else if (!flag1) {
					cout << "\nНужный элемент не был найден\n";
				}
				break;
			}
		}

		break;
	}
	if (!flag)
		cout << "Нужный элемент не был найден\n";
}


BD* BD::search_list(BD* search) {
	if (is_empty()) {
		cout << "База данных пуста\n";
		return search;
	}
	string name;
	int flag = 0;
	float cost = 0;
	PRICE* cur = first;
	cout << "0 - поиск по названию товара\n1 - поиск по названию магазина\n2 - поиск по цене\nдругой символ - выход\n";
	switch (_getch()) {
	case '0':
		cout << "Введите название товара\n";
		name = tolower(input_str(name));
		while (cur) {
			if (tolower((cur->name)) == name) {
				flag++;
				search->add(cur->name, cur->shop, cur->cost);
			}
			cur = cur->next;
		}
		break;
	case '1':
		cout << "Введите название магазина\n";
		name = tolower(input_str(name));
		while (cur) {
			if (tolower(cur->shop) == name) {
				flag++;
				search->add(cur->name, cur->shop, cur->cost);
			}
			cur = cur->next;
		}
		break;
	case '2':
		cout << "Введите цену товара\n";
		cost = input_float(name);
		while (cur) {
			if (cur->cost == cost) {
				flag++;
				search->add(cur->name, cur->shop, cur->cost);
			}
			cur = cur->next;
		}
		break;
	default:
		break;
	}
	if (!flag) {
		cout << "Не найдено элемента с заданными параметрами\n";
	}
	return search;
}


bool BD::search_to_add(PRICE* new_node) { //проверка, имеется ли элемент с таким же именем товара, магазина и ценой
	bool flag = 0;
	PRICE* cur = first;
	while (cur) {
		if (tolower(new_node->shop) == (tolower(cur->shop)) && (tolower(new_node->name) == (tolower(cur->name))) && ((new_node->cost) == cur->cost)) {
			flag++;
			return flag;
		}
		cur = cur->next;
	}
	return flag;
}

void BD::del(PRICE* cur) { //удаление элемента (принимает указатель на него)
	if (get_size() == 1) {
		delete cur;
		first = nullptr;
		last = nullptr;
		return;
	}
	if (cur->next == nullptr) {
		last = cur->prev;
		last->next = nullptr;
		delete cur;
		return;
	}
	if (cur->prev == nullptr) {
		first = cur->next;
		first->prev = nullptr;
		delete cur;
		return;
	}
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	delete cur;
}

PRICE* BD::update(PRICE* cur) { //изменить элемент
	string name, shop;
	float cost = 0;
	cout << "Введите название товара:\n";
	name = input_str(name);
	cout << "Введите название магазина:\n";
	shop = input_str(shop);
	cout << "Введите цену товара:\n";
	cost = input_float(name);
	if ((name == cur->name) && (shop == cur->shop) && (cost == cur->cost)) {
		cout << "\nЭлемент имеет такие же поля, обновления не произошло\n";
		return cur;
	}
	PRICE* temp = new PRICE(name, shop, cost);
	if (search_to_add(temp)) {
		cout << "\nТакой элемент уже есть в списке\n";
	}
	else {
		cur->cost = cost;
		cur->name = name;
		cur->shop = shop;
	}
	delete temp;
	return cur;
}

void BD::sort() { //сортировка пузырьком по товару, магазину, цене
	if (is_empty()) {
		cout << "База данных пуста\n";
		return;
	}
	int size = get_size();
	if (size == 1)
		return;
	PRICE* cur;
	cout << "0 - сортировка по названию товара\n1 - сортировка по названию магазина\n2 - сортировка по цене\nдругой символ - выход\n";
	switch (_getch()) {
	case '0':
		cout << "\nВыбрана сортировка по названию товара\n";
		for (int i = 0; i < size; i++) {
			cur = first;
			for (int j = 0; j < size - 1; j++) {
				if (cur->name[0] > cur->next->name[0]) {
					swap(cur, cur->next);
				}
				else
					cur = cur->next;
			}
		}
		break;
	case '1':
		cout << "\nВыбрана сортировка по названию магазина\n";
		for (int i = 0; i < size; i++) {
			cur = first;
			for (int j = 0; j < size - 1; j++) {
				if (cur->shop[0] > cur->next->shop[0]) {
					swap(cur, cur->next);
				}
				else
					cur = cur->next;
			}
		}
		break;
	case '2':
		cout << "\nВыбрана сортировка по цене\n";
		for (int i = 0; i < size; i++) {
			cur = first;
			for (int j = 0; j < size - 1; j++) {
				if (cur->cost > cur->next->cost) {
					swap(cur, cur->next);
				}
				else
					cur = cur->next;
			}
		}
		break;
	default:
		break;
	}
}

void BD::swap(PRICE* elem1, PRICE* elem2) { //поменять два соседних элемента списка местами
	PRICE* temp1 = elem1;
	PRICE* temp2 = elem2;
	if (elem1->prev == nullptr) { //первый и второй
		elem2->next->prev = temp1;
		elem1->next = elem2->next;
		elem2->prev = nullptr;
		elem1->prev = temp2;
		elem2->next = temp1;
		first = elem2;
		return;
	}
	if (elem2->next == nullptr) { //предпоследний и последний
		elem1->prev->next = temp2;
		elem2->prev = elem1->prev;
		elem1->next = nullptr;
		elem2->next = temp1;
		elem1->prev = temp2;
		last = elem1;
		return;
	}
	elem1->prev->next = temp2; //другая ситуация
	elem2->prev = elem1->prev;
	elem2->next->prev = temp1;
	elem1->next = elem2->next;
	elem2->next = temp1;
	elem1->prev = temp2;
	return;
}

void BD::clear_all() { //очистить все аналогично деструктору
	while (first) {
		last = first->next;
		delete first;
		first = last;
	}
}

PRICE* BD::split(string str, char delim) { //преобразовать строку в элемент структуры
	int counter = 0;
	string temp;
	PRICE* tmp = new PRICE();
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == delim) {
			if (!temp.size()) {
				return tmp;
			}
			temp = erase_spaces(temp);
			switch (counter) {
			case 0:
				if (test_str(temp))
					tmp->name = first_upper(tolower(temp));
				break;
			case 1:
				if (test_str(temp))
					tmp->shop = first_upper(tolower(temp));
				break;
			case 2:
				if (i != str.size() - 1) {
					return tmp;
				}
				if (test_float(temp))
					tmp->cost = stof(temp);
				break;
			}
			temp = "";
			counter++;
		}
		else
			temp += str[i];
	}
	return tmp;

}

void BD::download() { //загрузить базу данных из файла
	int all = 0;
	string path;
	PRICE* tmp;
	cout << "Введите название файла для загрузки базы данных\n";
	cin >> path;
	cin.ignore(32767, '\n'); //необходимо, т.к. в некоторых сценариях пропускается следующий ввод
	cout << endl;
	ifstream file;
	file.open(path);
	if (!file.is_open()) {
		cout << "ошибка открытия файла\n";
		return;
	}
	clear_all();
	string str;
	while (!file.eof()) {
		getline(file, str);
		all++;
		if (str == "") continue;
		tmp = split(str);
		if ((tmp->name != "" && tmp->shop != "" && tmp->cost != 0) && (!search_to_add(tmp))) {
			push(tmp);
		}
		else
			delete tmp;
	}
	if (!get_size()) {
		cout << "База данных не была загружена\n";
		return;
	}
	cout << "Успешно загружено " << get_size() << " из " << all << " строк" << endl;
	file.close();
}

void BD::upload() { //выгрузить базу данных в файл
	if (is_empty()) {
		cout << "База данных пуста\n";
		return;
	}
	string path;
	cout << "Введите название файла\n";
	cin >> path;
	cin.ignore(32767, '\n');//необходимо, т.к. в некоторых сценариях пропускается следующий ввод
	cout << endl;
	ofstream file;
	file.open(path);
	if (!file.is_open()) {
		cout << "Ошибка открытия файла\n";
		return;
	}
	PRICE* cur = first;
	for (int i = 0; i < get_size(); i++) {
		file << cur->name << ';' << cur->shop << ';' << cur->cost << ';' << endl;
		cur = cur->next;
	}
	cout << "База данных успешно выгружена\n";
	file.close();
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	BD MyBD;
	string name, shop;
	float cost = 0;
	bool flag = 0;
	while (1) {
		cout << "\n0 - загрузка БД\n1 - выгрузка БД\n2 - вывод БД\n3 - добавление элемента в БД\n4 - поиск элемента из БД\n5 - удаление, изменение элемента из БД\n6 - отсортировать БД (по алфавиту/возрастанию цены)\nдругой символ - выход\n";
		switch (_getch()) {
		case '0':
			cout << "\nВыбрана загрузка БД\n";
			MyBD.download();
			break;
		case '1':
			cout << "\nВыбрана выгрузка БД\n";
			MyBD.upload();
			break;
		case '2':
			cout << "\nВыбран вывод БД\n";
			if (MyBD.is_empty()) {
				cout << "База данных пуста\n";
				break;
			}
			MyBD.print_BD();
			break;
		case '3':
			cout << "\nВыбрано добавление элемента в БД\n";
			cout << "Введите название товара:\n";
			name = input_str(name);
			cout << "Введите название магазина:\n";
			shop = input_str(shop);
			cout << "Введите цену товара:\n";
			cost = input_float(name);
			MyBD.add(name, shop, cost);
			break;
		case '4': {
			cout << "\nВыбран поиск элемента из БД\n";
			BD* search = new BD();
			search = MyBD.search_list(search);
			if (search->is_empty()) {
				delete search;
				break;
			}
			search->print_BD();
			delete search;
			break;
		}
		case '5':
			cout << "\nВыбрано удаление, изменение элемента из БД\n";
			MyBD.search();
			break;
		case '6':
			cout << "\nВыбрана сортировка БД\n";
			MyBD.sort();
			break;
		default:
			flag = 1;
			break;
		}
		if (flag)
			break;
	}
	return 0;
}
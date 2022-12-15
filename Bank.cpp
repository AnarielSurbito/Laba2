#include "iostream"
#include "fstream"
#include "string"
#include "Bank.h"

using namespace std;

void UserCard::SetCardNumber(int card_number) {
    CardNumber = card_number;
}
void UserCard::SetCardHolder(string card_holder) {
    CardHolder = card_holder;
}
void UserCard::SetBalance(double balance) {
    UserCard::balance = balance;
}
void UserCard::SetPassword(int password) {
    Password = password;
}
int UserCard::GetCardNumber() const {
    return CardNumber;
}
std::string UserCard::GetCardHolder() const {
    return CardHolder;
}
int UserCard::GetPassword() const {
    return Password;
}
double UserCard::GetBalance() const {
    return balance;
}

void Atm::start() {
    setlocale(LC_ALL, "Russian");
    cout << "������� ��� ������������ (�� ����������):\n";
    string name;
    getline(cin, name);
    if (!exists(name)) {
        int id, password;
        cout << "������� ����� �����\n";
        cin >> id;
        srand(time(0));
        password = (rand() % 9) * 1000 + (rand() % 9) * 100 + (rand() % 9) * 10 + (rand() % 9);
        cout << "��� ������: " << password << endl;
        system("clear");
        registerNewCard(id, name, 0, password);
    }
    setCardNow(deleteSpaces(name));
    if (authorization()) {
        operations();
    }
}

bool Atm::exists(std::string name) {
    name = "database\\" + name + ".txt";
    ifstream f(name.c_str());
    return f.good();
}

void Atm::registerNewCard(int id, string name, double balance, int password) {
    cardNow.SetCardNumber(id);
    cardNow.SetCardHolder(name);
    cardNow.SetBalance(balance);
    cardNow.SetPassword(password);
    save(deleteSpaces(name), cardNow);
}

void Atm::setCardNow(string path) {
    fin.open("database\\" + path + ".txt");
    if (fin.is_open()) {
        string buff;
        getline(fin, buff);
        cardNow.SetCardNumber(atoi(buff.c_str()));
        getline(fin, buff);
        cardNow.SetPassword(atoi(buff.c_str()));
        getline(fin, buff);
        cardNow.SetCardHolder(buff);
        getline(fin, buff);
        cardNow.SetBalance(stod(buff));
    }
    fin.close();
}

bool Atm::authorization() {
    setlocale(LC_ALL, "Russian");
    int password;
    cout << "������� ������ ��� �����������:" << endl;
    cin >> password;
    for (int i = 0; i < 3; i++) {
        if (password == cardNow.GetPassword()) {
            return true;
        }
        else {
            cout << "�������� ������";
        }
    }
    return false;
}

UserCard Atm::operations() {
    setlocale(LC_ALL, "Russian");
    cout << "��������:\n0 - �����\n1 - ��������� ������\n2 - ����� ������ � ��������\n3 - ���������� ������ �� �������� ������� ������������\n4 - ������ ��������\n";
    int kod;
    cin >> kod;
    string path = deleteSpaces(cardNow.GetCardHolder());
    switch (kod) {
    case 0: {
        return cardNow;
    }
    case 1: {
        double money;
        cout << "������� �����, ������� ������ ������:" << endl;
        cin >> money;
        replenishment(path, money);
        break;
    }
    case 2: {
        bool correct_inp = false;
        double money;
        cout << "������� �����, ������� ������ �����:" << endl;
        while (correct_inp == false) {
            cin >> money;
            if (money < cardNow.GetBalance()) {
                correct_inp = true;
            }
            else
                cout << "�������� �����" << endl;
        }
        withdrawal(path, money);
        break;
    }
    case 3: {
        string ownerOfSecondCard = "", empty = "";
        cout << "������� ��� ������������, �������� ������ ��������� ������" << endl;
        getline(cin, empty);
        getline(cin, ownerOfSecondCard);
        double money;
        bool correct_inp = false;
        cout << "������� �����, ������� ������ ���������";
        while (correct_inp == false) {
            cin >> money;
            if (money < cardNow.GetBalance()) {
                correct_inp = true;
            }
            else
                cout << "�������� �����" << endl;
        }
        transfer(path, deleteSpaces(ownerOfSecondCard), money);
        break;
    }
    case 4: {
        cout << "������� ������: " << cardNow.GetBalance() << endl;
        break;
    }
    }
    cout << "�� ������ ����������? (y/n)\n";
    char repeat;
    bool input = false;
    while (input == false) {
        cout << "����������, �������� ���������� �����" << endl;
        cin >> repeat;
        if (repeat == 'y' || repeat == 'n')
            input = true;
    }
    system("cls");
    if (repeat == 'y') {
        return operations();
    }
    else {
        return cardNow;
    }
}

void UserCard::SetInfo(string path) {
    std::ifstream CardInfo;
    CardInfo.open("database\\" + path + ".txt");
    if (CardInfo.is_open()) {
        string buff;
        getline(CardInfo, buff);
        SetCardNumber(atoi(buff.c_str()));
        getline(CardInfo, buff);
        SetPassword(atoi(buff.c_str()));
        getline(CardInfo, buff);
        SetCardHolder(buff);
        getline(CardInfo, buff);
        SetBalance(stod(buff));
    }
    CardInfo.close();
}

UserCard Atm::getCardNow() {
    return cardNow;
}

string Atm::deleteSpaces(string path) {
    string res = "";
    for (int i = 0; i < path.size(); i++) {
        if (path[i] != ' ') {
            res += path[i];
        }
    }
    return res;
}

void Atm::save(string path, UserCard card) {
    fout.open("database\\" + deleteSpaces(path) + ".txt");
    fout << card.GetCardNumber() << endl << card.GetPassword() << endl << card.GetCardHolder() << endl << card.GetBalance();
    fout.close();
}

void Atm::withdrawal(string path, double money) {
    setCardNow(path);
    cardNow.SetBalance(cardNow.GetBalance() - money);
    save(path, cardNow);
}
void Atm::replenishment(string path, double money) {
    setCardNow(path);
    cardNow.SetBalance(cardNow.GetBalance() + money);
    save(path, cardNow);
}
void Atm::transfer(string pathFrom, string pathTo, double money) {
    UserCard to;
    to.SetInfo(pathTo);
    cardNow.SetInfo(pathFrom);
    cardNow.SetBalance(cardNow.GetBalance() - money);
    save(pathFrom, cardNow);
    to.SetBalance(to.GetBalance() + money);
    save(pathTo, to);
}
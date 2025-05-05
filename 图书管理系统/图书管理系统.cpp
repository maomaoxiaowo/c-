#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <conio.h>u666
#include <string>
#include <limits>
#define NOMINMAX  // 禁用Windows的min/max宏
#include <windows.h>  // Windows颜色API

using namespace std;

// 控制台颜色枚举
enum ConsoleColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    BRIGHT_WHITE = 15
};

// 设置控制台颜色函数
void setColor(ConsoleColor text, ConsoleColor background = BLACK) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (background << 4) | text);
}

// 重置颜色到默认
void resetColor() {
    setColor(WHITE);
}

// 带颜色的打印函数
void printColored(const string& text, ConsoleColor color) {
    setColor(color);
    cout << text;
    resetColor();
}

class Book {
public:
    string ISBN;
    string title;
    string author;
    string publisher;
    double price;

    Book(string isbn, string t, string a, string p, double pr)
        : ISBN(isbn), title(t), author(a), publisher(p), price(pr) {
    }

    void display() const {
        setColor(CYAN);
        cout << left << setw(15) << ISBN
            << setw(25) << title
            << setw(20) << author
            << setw(20) << publisher
            << fixed << setprecision(2) << price << endl;
        resetColor();
    }
};

class Library {
private:
    vector<Book> books;
    const string filename = "library.dat";

public:
    void addBook() {
        string isbn, title, author, publisher;
        double price;

        printHeader("\n请输入图书信息：", YELLOW);

        printColored("ISBN: ", GREEN);
        cin >> isbn;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        printColored("书名: ", GREEN);
        getline(cin, title);

        printColored("作者: ", GREEN);
        getline(cin, author);

        printColored("出版社: ", GREEN);
        getline(cin, publisher);

        printColored("价格: ", GREEN);
        cin >> price;

        books.emplace_back(isbn, title, author, publisher, price);
        printColored("\n图书添加成功！\n", GREEN);
        loadingAnimation();
    }

    void searchBook() {
        int choice;
        string keyword;

        printHeader("\n请选择查询方式：", YELLOW);
        printColored("1. ISBN查询\n", CYAN);
        printColored("2. 书名查询\n", CYAN);
        printColored("3. 作者查询\n", CYAN);
        printColored("请输入选项：", GREEN);
        cin >> choice;

        printColored("请输入查询关键词：", GREEN);
        cin.ignore();
        getline(cin, keyword);

        printHeader("\n查询结果：", YELLOW);
        printTableHeader();

        bool found = false;
        for (const auto& book : books) {
            bool match = false;
            switch (choice) {
            case 1: match = (book.ISBN == keyword); break;
            case 2: match = (book.title.find(keyword) != string::npos); break;
            case 3: match = (book.author.find(keyword) != string::npos); break;
            }
            if (match) {
                book.display();
                found = true;
            }
        }

        if (!found) {
            printColored("\n未找到匹配的图书！\n", RED);
        }
    }

    void deleteBook() {
        string isbn;
        printColored("\n请输入要删除的图书ISBN：", RED);
        cin >> isbn;

        auto it = find_if(books.begin(), books.end(),
            [&isbn](const Book& b) { return b.ISBN == isbn; });

        if (it != books.end()) {
            books.erase(it);
            printColored("\n图书删除成功！\n", GREEN);
            loadingAnimation();
        }
        else {
            printColored("\n未找到该图书！\n", RED);
        }
    }

    void modifyBook() {
        string isbn;
        printColored("\n请输入要修改的图书ISBN：", YELLOW);
        cin >> isbn;

        auto it = find_if(books.begin(), books.end(),
            [&isbn](const Book& b) { return b.ISBN == isbn; });

        if (it != books.end()) {
            printHeader("\n请输入新信息（留空保持不变）：", YELLOW);
            cin.ignore();

            string input;
            printColored("新书名: ", CYAN);
            getline(cin, input);
            if (!input.empty()) it->title = input;

            printColored("新作者: ", CYAN);
            getline(cin, input);
            if (!input.empty()) it->author = input;

            printColored("新出版社: ", CYAN);
            getline(cin, input);
            if (!input.empty()) it->publisher = input;

            printColored("新价格: ", CYAN);
            getline(cin, input);
            if (!input.empty()) it->price = stod(input);

            printColored("\n图书信息修改成功！\n", GREEN);
            loadingAnimation();
        }
        else {
            printColored("\n未找到该图书！\n", RED);
        }
    }

    void listAll() {
        printHeader("\n当前馆藏图书列表：", YELLOW);
        printTableHeader();
        for (const auto& book : books) {
            book.display();
        }
        printColored("\n共找到 " + to_string(books.size()) + " 本图书\n", MAGENTA);
    }

    void saveToFile() {
        ofstream outFile(filename);
        if (outFile) {
            for (const auto& book : books) {
                outFile << book.ISBN << ","
                    << book.title << ","
                    << book.author << ","
                    << book.publisher << ","
                    << book.price << "\n";
            }
            printColored("\n数据保存中...", BLUE);
            loadingAnimation();
            printColored("\n数据已保存！\n", GREEN);
        }
    }

    void loadFromFile() {
        ifstream inFile(filename);
        if (inFile) {
            books.clear();
            string line;
            printColored("\n正在加载数据...", BLUE);
            loadingAnimation();
            while (getline(inFile, line)) {
                size_t pos = 0;
                string parts[5];
                for (int i = 0; i < 5; ++i) {
                    size_t next = line.find(',', pos);
                    parts[i] = line.substr(pos, next - pos);
                    pos = next + 1;
                }
                books.emplace_back(parts[0], parts[1], parts[2], parts[3], stod(parts[4]));
            }
            printColored("\n数据加载完成！\n", GREEN);
        }
    }

private:
    void printTableHeader() const {
        setColor(MAGENTA);
        cout << left << setw(15) << "ISBN"
            << setw(25) << "书名"
            << setw(20) << "作者"
            << setw(20) << "出版社"
            << "价格" << endl;
        cout << string(80, '-') << endl;
        resetColor();
    }

    void printHeader(const string& text, ConsoleColor color) {
        setColor(color);
        cout << "\n" << string(50, '=') << endl;
        cout << " " << text << endl;
        cout << string(50, '=') << endl;
        resetColor();
    }

    void loadingAnimation() {
        for (int i = 0; i < 3; ++i) {
            Sleep(300);
            cout << ".";
        }
        cout << endl;
    }
};

void showMenu() {
    system("cls");
    setColor(BLUE);
    cout << "\n" << string(50, '=') << endl;
    cout << "      图书管理系统 v2.0" << endl;
    cout << string(50, '=') << endl;

    printColored("1. 添加图书\n", CYAN);
    printColored("2. 查询图书\n", CYAN);
    printColored("3. 删除图书\n", CYAN);
    printColored("4. 修改图书\n", CYAN);
    printColored("5. 显示所有\n", CYAN);
    printColored("6. 保存数据\n", CYAN);
    printColored("0. 退出系统\n", RED);

    setColor(YELLOW);
    cout << string(50, '-') << endl;
    printColored("请输入选项：", GREEN);
}

int main() {
    Library lib;
    lib.loadFromFile();

    int choice;
    do {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: lib.addBook(); break;
        case 2: lib.searchBook(); break;
        case 3: lib.deleteBook(); break;
        case 4: lib.modifyBook(); break;
        case 5: lib.listAll(); break;
        case 6: lib.saveToFile(); break;
        case 0:
            lib.saveToFile();
            printColored("\n感谢使用！再见！\n", MAGENTA);
            break;
        default:
            printColored("\n无效选项，请重新输入！\n", RED);
            Sleep(1000);
        }
        if (choice != 0) {
            printColored("\n按任意键继续...", BLUE);
            _getch();
        }
    } while (choice != 0);

    return 0;
}

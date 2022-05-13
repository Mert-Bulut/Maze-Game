#include <iostream>
#include <vector>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include <iomanip>

#define maxSize 100
#define down 1
#define rightt 2
#define leftt 4
#define up 8

using namespace std;

vector <int> block_row;
vector <int> block_column;
vector <int> block_direct;

typedef struct point {
    int x;
    int y;
}Point;
Point start, end;

int x_num = 1;
int y_num = 1;
char giris;

void gitXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y + 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void push(int x, int y, int direct) {	// Veriyi üç vektörde tutuyor
	block_row.push_back(x);
	block_column.push_back(y);
	block_direct.push_back(direct);
}

int count(int size) {	//Mevcut konumun etrafındaki duvarların sayısını hesaplayın
	int cnt = 0;
	if (y_num + 1 < size) {//down
		push(x_num, y_num + 1, down);
		cnt++;
	} 
	if (x_num + 1 < size) {//right
		push(x_num + 1, y_num, rightt);
		cnt++;
	} 
	if (y_num - 1 >= 1) {//up
		push(x_num, y_num - 1, up);
		cnt++;
	} 
	if (x_num - 1 >= 1) {//left
		push(x_num - 1, y_num, leftt);
		cnt++;
	} 
	return cnt;
}

void init(int mapArr[][maxSize], int size){	//Tek sayılar için tüm haritayı duvar olarak ayarlarken 
	srand(time(NULL));						//çift sayılar için haritayı duvar yapıyor ama size kadar i ve j'yi rand atamakta.
	int deneme = 0;							//çünkü komple duvar basıldığında çift sayılarda, çift duvar sorununu aşmak adına yapıldı.
	for (int i = 0; i < size + 2; i++)
		for (int j = 0; j < size + 2; j++)
			if (size % 2 == 0)
				if ((i == size && (j > 0 && j < size)) || (j == size && (i > 0 && i < size + 1))) {
					deneme = rand() % 2;
					if (deneme == 0)
						mapArr[i][j] = 0;
					else {
						mapArr[i][j] = 1;
						if (mapArr[1][size] == 1 || mapArr[2][size] == 1 || mapArr[size][size] == 1
							|| mapArr[size - 1][size] == 1 || mapArr[size][size - 1] == 1
							|| mapArr[size][1] == 1 || mapArr[size][2] == 1)
							mapArr[i][j] = 0;
					}
				}
				else 
					mapArr[i][j] = 1;
			else 
				for (int i = 0; i < size + 2; i++) 
					for (int j = 0; j < size + 2; j++) 
						mapArr[i][j] = 1;
}

void haritaBosaltma(int mapArr[][maxSize], int size) {
	start.x = 1;						// Başlangıç noktasını tanımlandı.
	start.y = 1;
	srand((unsigned)time(NULL));
	count(size);
	mapArr[1][2] = 0;
	while (block_row.size()) {			// Döngüye girmek için iki duvara (başlangıç noktasının sağına ve altına) bastırıyor.
		int num = block_row.size();
		int randnum = rand() % num;		// 0 ile num-1 arasında ve vektörde bir alt simge olan rasgele bir sayı üretiyor.
		x_num = block_row[randnum];		// Alttaki iki satır atlanabilir.
		y_num = block_column[randnum];
		switch (block_direct[randnum]) {//Sonraki işlemler için bir yön seçiliyor, başlangıç noktası, komşu blok ve hedef blok 
		case down: {					//üç alan aynı satırda, ardından hedef blok konumuna hareket ediyor.
			x_num = block_row[randnum];
			y_num = block_column[randnum] + 1;
			break;
		}
		case rightt: {
			x_num = block_row[randnum] + 1;
			y_num = block_column[randnum];
			break;
		}
		case leftt: {
			x_num = block_row[randnum] - 1;
			y_num = block_column[randnum];
			break;
		}
		case up: {
			x_num = block_row[randnum];
			y_num = block_column[randnum] - 1;
			break;
		}
		}
		if (mapArr[x_num][y_num] == 1) {							//Hedef blok bir duvar ise
			mapArr[block_row[randnum]][block_column[randnum]] = 0;	//Duvardan geçiyor
			mapArr[x_num][y_num] = 0;								//Hedef bloğu geçiyor
			count(size);											//Mevcut konumun etrafındaki bitişik duvarların sayısını tekrar hesaplıyor ve vektöre kaydediyor
		}
		block_row.erase(block_row.begin() + randnum);				//Bu duvarı silin(Daha önce yapılmış olanlar için ve
		block_column.erase(block_column.begin() + randnum);			// döngüden atlayabilmemizi sağlamak için kullanılamayan duvarı siliyor.)
		block_direct.erase(block_direct.begin() + randnum);
	}
}

void yazdir(int mapArr[][maxSize], int size) {
	init(mapArr, size);
	haritaBosaltma(mapArr, size);
	srand(time(NULL));
	int dene = 0, canavar = 0;
	for (int i = 0; i < size + 2; i++) {
		for (int j = 0; j < size + 2; j++) {
			if (mapArr[i][j] == 0) {
				if ((i == size) && (j == size)) {
					mapArr[i][j] = 'E';
					cout << 'E';
				}
				else {
					dene = rand() % 70;
					if (dene == 0 || dene == 1 || dene == 2 || dene == 3 || dene == 4) {
						mapArr[i][j] = 'C';
						cout << 'C';
					}
					else if (dene == 5) {
						mapArr[i][j] = 'E';
						cout << 'E';
					}
					else {
						canavar = rand() % 5;
						if (canavar == 0) {
							if ((i == 1 && j == size) || (i == size && j == 1)) {
								mapArr[i][j] = 'M';
								cout << 'M';
							}
							else cout << ' ';
						}
						else if (canavar == 1) {
							if ((i == size && j == 1) || (i == 2 && j == size)) {
								mapArr[i][j] = 'M';
								cout << 'M';
							}
							else
								cout << ' ';
						}
						else if (canavar == 2) {
							if ((i == size && j == 1) || (i == 1 && j == size)) {
								mapArr[i][j] = 'M';
								cout << 'M';
							}
							else
								cout << ' ';
						}
						else if (canavar == 3) {
							if ((i == 1 && j == size) || (i == 3 && j == size) || (i == size && j == 1)) {
								mapArr[i][j] = 'M';
								cout << 'M';
							}
							else
								cout << ' ';
						}
						else {
							if ((i == size && j == 1) || (i == 1 && j == size - 2)) {
								mapArr[i][j] = 'M';
								cout << 'M';
							}
							else cout << ' ';
						}
					}
				}
			}
			else cout << char(219);
		}
		cout << endl;
	}
}

void hareket(int mapArr[][maxSize], int size);

void returnyazdir(int mapArr[][maxSize], int size, int xCln, int yCln) {
	system("cls");
	cout << "M yani canavara carptin, lutfen tekrar deneyin..." << endl;
	yazdir(mapArr, size);
	hareket(mapArr, size);
}

void hareket(int mapArr[][maxSize], int size) {
	ofstream file;
	file.open("oyuncuHareketleri.txt");
	file << "-------------" << endl;
	file << "| " << left << setw(3) << "x" << " | " << right << setw(3) << "y" << " |" << endl;
	file << "-------------" << endl;
	file << "| " << left << setw(3) << "0" << " | " << right << setw(3) << "0" << " |" << endl;
	file << "-------------" << endl;
	int x = 1;
	int y = 1;
	int xCln = 1;
	int yCln = 1;
	int gold = 0;

	cout << "Not: Eger monster'a carparsan ayni boyutta farkli bir labirent olusur.." << endl;
	cout << "Denemeye hazir misin?" << endl;
	while (1) {
		gitXY(xCln, yCln);
		cout << " ";
		gitXY(x, y);
		cout << char(254);
		giris = _getch();
		xCln = x;
		yCln = y;

		if (giris == 'w' || giris == 'W') {
			y -= 1;
		}
		else if (giris == 's' || giris == 'S') {
			y += 1;
		}
		else if (giris == 'a' || giris == 'A') {
			x -= 1;
		}
		else if (giris == 'd' || giris == 'D') {
			x += 1;
		}
		if (mapArr[y][x] != 1) {	//Duvara yapılan hareket hamleleri kaydedilmez.
			file << "| " << left << setw(3) << y - 1 << " | " << right << setw(3) << x - 1 << " |" << endl;
			file << "-------------" << endl;
		}

		if (mapArr[y][x] == 1) { // wall
			x = xCln;
			y = yCln;
		}
		if (mapArr[y][x] == 'M') { // monster
			system("color 7");
			returnyazdir(mapArr, size, xCln, yCln);
			gold = 0;
		}
		if (mapArr[y][x] == 'C') { // gold
			mapArr[y][x] = ' ';    // C'yi 1 defa aldıktan sonra orayı boşaltıyor.
			gold++;
		}
		if (mapArr[y][x] == 'E') { // exit
			break;
		}
	}
	system("cls");
	cout << endl << endl;
	cout << "Tebrikler.." << endl;
	cout << gold << " tane altin topladin" << endl;
	system("color A");
	system("pause");
	exit(0);
}

int main() {
	srand(time(0));
	int mapArr[maxSize][maxSize];
	int size;
	cout << "@mrtblt13 iftiharla sunar.. " << endl;
	cout << "Oyuna hos geldiniz.. " << endl;
	cout << "Baslamak icin lutfen labirent boyutunu giriniz(10-100): ";
	cin >> size;
	while (size < 10 || size > 100) {
		cout << "Lutfen 10 - 100 arasinda sayi giriniz... : ";
		cin >> size;
	}
	system("cls");
	cout << " M = MONSTER, C = COIN, E = EXIT, UP = W/w, DOWN = S/s, LEFT = A/a, RIGHT = D/d" << endl;
    init(mapArr,size);
	haritaBosaltma(mapArr, size);
	yazdir(mapArr, size);
	hareket(mapArr, size);
	_getch();
	system("pause");
    return 0;
}

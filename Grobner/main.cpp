#include <iostream>
using namespace std;

class Rectangle {
	int width, height;
public:
	Rectangle(int x, int y) : width(x), height(y) {}
	int area(void) { return width * height; }
};


int main() {
	Rectangle obj(3, 4);
	Rectangle * foo, *bar, *baz;
	foo = &obj;
	bar = new Rectangle(5, 6);
	baz = new Rectangle[2] {{ 2, 5 }, { 3, 6 } };
	cout << "obj's area: " << obj.area() << '\n';
	cout << "*foo's area: " << foo->area() << '\n';
	cout << "*bar's area: " << bar->area() << '\n';
	cout << "baz[0]'s area:" << baz[0].area() << '\n';
	cout << "baz[1]'s area:" << baz[1].area() << '\n';
	delete bar;
	delete[] baz;
	cin.ignore();
}
/*
// overloading class constructors
#include <iostream>
using namespace std;

class Rectangle {
	int width, height;
public:
	Rectangle();
	Rectangle(int, int);
	int area(void) { return (width*height); }
};

Rectangle::Rectangle() {
	width = 5;
	height = 5;
}

Rectangle::Rectangle(int a, int b) {
	width = a;
	height = b;
}

int main() {
	Rectangle rect(3, 4);
	Rectangle rectb;
	cout << "rect area: " << rect.area() << endl;
	cout << "rectb area: " << rectb.area() << endl;
	return 0;
}*/
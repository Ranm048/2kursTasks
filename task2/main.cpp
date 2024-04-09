/*
 V Класс не плосский
 V Конструктор
 V Конструктор Копирования
 V Деструктор
 V Операция присваивания
 V Статические члены класса
 V Константные методы
 V Операция вывода
 V Операция [] или ()
 */
#include <iostream>

class Book {
	private:
		std::string Author;
		std::string Title;
	public:
		//Конструктор
		Book (){
			Author = "Unknown";
			Title = "No title";
		}
		//Конструктор с параметрами
		Book (std::string nm, std::string ttl){
			Author = nm;
			Title = ttl;
		}
		//Конструктор копирования
		Book (const Book &original){
			Author = original.Author;
			Title = original.Title;
		}
		Book& operator= (const Book &book){
			Author = book.Author;
			Title = book.Title;
			return *this;
		}
		~Book(){
		}
	friend std::ostream& operator<< (std::ostream &out, const Book &book);
	friend std::istream& operator>> (std::istream &in, Book &book);
};

std::ostream& operator<< (std::ostream &out, const Book &book){
	out<<book.Title<<" by "<<book.Author<<"\n";
	return out;
}

std::istream& operator>> (std::istream &in, Book &book){
	std::cout<<"Enter the books author : ";
	std::getline(in,book.Author);
	std::cout<<"Enter the title : ";
	std::getline(in,book.Title);
	return in;
}

class BookContainment {
	public:
		virtual void add (int n) {};
		virtual void OutShelf () {};
		virtual Book operator() (int n) = 0;
		virtual const int bookcount () = 0;
};

class BookShelf : public BookContainment{
	private:
		Book *library;
		int counter = 0;
		int Max = 0;
	public:
		BookShelf (int Cnt) {
			Max = Cnt;
			Book * lib = new Book[Cnt];
			library = lib;
		}
		void add (int i){
			if (i > Max) return;
			std::cin>>library[i];
			counter++;
		}
		Book operator() (int n){
			return library[n];
		}

		void OutShelf (){
		for (int i = 0; i<Max; i++){
			std::cout<<library[i];
		}
		}

		const int bookcount () {
			return counter;
		}
		~BookShelf(){
			delete []library;
		}
};
int main () {
	BookContainment* ptr;
	BookShelf shelf(6);
	ptr = &shelf;
	std::cout<<ptr->bookcount()<<"\n";
	shelf.add(4);
	shelf.add(3);
	std::cout<<shelf.bookcount()<<"\n";
	return 0;
}

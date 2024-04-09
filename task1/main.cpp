/*	Вариант 8
 *  Составить описание класса для определения одномерных массивов целых 
 * чисел (векторов). 
 //Предусмотреть: 
 1 возможность обращения к отдельному элементу массива с контролем выхода за пределы массива
 1 возможность задания произвольных границ индексов при создании объекта
 1 возможность выполнения операций поэлементного сложения и вычитания массивов 
   с одинаковыми границами индексов,
 1 умножения и деления всех элементов массива на скаляр
 1 вывода на экран элемента массива по заданному индексу
 1 вывода на экран всего массива
 0 Написать программу, демонстрирующую работу с этим классом
 0 Программа должна содержать меню, позволяющее осуществить проверку всех методов класса. 
*/
#include <iostream>

class Vector {
	private:
		int Start=0;
		int Max;
		int *arr;
	public:
		//Конструктор для нормальных границ
		Vector (int k){
			arr=new int[k];
			Max=k;
		}
		//Конструктор для пользовательских границ
		Vector (int a, int b){
			arr=new int(b-a+2);
			Start=a;
			Max=b+1;
		}
		//Оператор обращения к отдельному элементу
		int& operator[] (int n){
			if ((n<Start)||(n>Max)){
				std::cout<<"Wrong index of Vector\n";
				exit(-1);
			}
			return arr[n-Start];
		}
		//Оператор умножения на скаляр
		Vector operator*= (const int X){
			for (int i=0; i<(Max-Start); i++){
				arr[i]*=X;
			}
			return *this;
		}
		//Оператор деление на скаляр
		Vector operator/= (const int X){
			for (int i=0; i<(Max-Start); i++){
				arr[i]/=X;
			}
			return *this;
		}
		//Оператор сложения двух векторов
		Vector& operator+(Vector B){
			if((Start!=B.Start)||(Max!=B.Max)){
				std::cout<<"Vectors incompatible\n";
				exit(-1);
			}
			for (int i=0; i<(Max-Start); i++){
				arr[i]+=B.arr[i];
			}
			return *this;
		}
		//Оператор вычитания двух векторов
		Vector& operator-(Vector B){
		if((Start!=B.Start)||(Max!=B.Max)){
				std::cout<<"Vectors incompatible\n";
				exit(-1);
			}
			for (int i=0; i<(Max-Start); i++){
				arr[i]-=B.arr[i];
			}
			return *this;
		}

		friend std::ostream& operator<< (std::ostream &output, const Vector &vector);
		friend std::istream& operator>> (std::istream &input, Vector &vector);
};
std::ostream& operator<< (std::ostream &output, const Vector &vector){
	output<<"Vector :";
	for (int i=0; i<(vector.Max-vector.Start); i++){
		output<<vector.arr[i]<<" ";
	}
	output<<"\n";
	return output;
}
std::istream& operator>> (std::istream &input, Vector &vector){
	std::cout<<"Enter Vector :";
	for (int i=0; i<(vector.Max-vector.Start); i++){
		input >> vector.arr[i];
	}
	return input;
}

int main (){
	int mode;
	int yes;

	int n;
	Vector A1(5);

	Vector A2(2, 6);

	Vector X(6);
	Vector Y(6);


	std::cout<<"Wich method would you like to test?(1 - single element; 2 - single element with custom borders; 3 - sum and sub of Vectors; 4 - multiply and divide to scalar; 5 - output the Vector) :";
	std::cin>>mode;
	std::cout<<"\n";
	switch (mode){
		case 1:
			std::cin>>A1;
			std::cout<<"Enter element idex from 0 to 4:";
			std::cin>>n;
			std::cout<<A1[n];
			break;
		case 2:
			std::cin>>A2;
			std::cout<<"Enter element idex (from 2 to 6):";
			std::cin>>n;
			std::cout<<A2[n];
			break;
		case 3:
			std::cin>>X;
			std::cin>>Y;
			std::cout<<"Summ or subtract (sum=1 sub=2) :";
			std::cin>>yes;
			if (yes==1) {std::cout<<"\n"<<X+Y<<"\n"; break;}
			if (yes==2) {std::cout<<"\n"<<X-Y<<"\n"; break;}

			break;
		case 4:
			std::cin>>X;
			std::cout<<"Enter scalar :";
			std::cin>>n;
			std::cout<<"Mul or div (mul=1 div=2) :";
			std::cin>>yes;
			if (yes==1) {X*=n; std::cout<<"\n"<<X<<"\n"; break;}
			if (yes==2) {X/=n; std::cout<<"\n"<<X<<"\n"; break;}
			break;
		case 5:
			std::cin>>X;
			std::cout<<X;
	}
	return 0;
}

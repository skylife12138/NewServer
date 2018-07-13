#include "Prec.h"

Person::Person()
{
	cout << "Person()" << std::endl;
}

Person::~Person()
{
    cout << "~Person()" << std::endl;
}

void Person::show()
{
	cout << "Hello, I'm person" << std::endl;
}

//REFLECT(Allen);

Allen::Allen()
{
	std::cout << "Allen()" << std::endl;
}


Allen::~Allen()
{
	std::cout << "~Allen()" << std::endl;
}

void Allen::show()
{
	std::cout << "Hello, I'm Allen" << std::endl;
}


//REFLECT(Luffy);

Luffy::Luffy()
{
	std::cout << "Luffy()" << std::endl;
}


Luffy::~Luffy()
{
	std::cout << "~Luffy()" << std::endl;
}

void Luffy::show()
{
	std::cout << "Hello, I'm Luffy" << std::endl;
}
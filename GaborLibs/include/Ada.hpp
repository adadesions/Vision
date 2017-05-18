#ifndef ADA_HPP
#define  ADA_HPP

#include <iostream>

namespace ada {
	class Ada
	{
		protected:
			int hp, sp;
		public:
			Ada(int hp, int sp);
			~Ada();
			virtual int getHp();
	};

	Ada::Ada(int hp, int sp){
		this->hp = hp;
		this->sp = sp;
	}
	Ada::~Ada(void){
		std::cout << "Detroyed" << std::endl;
	}
	int Ada::getHp(void){
		std::cout << "Hp(Ada) : " << this->hp << std::endl;
		return this->hp;
	}

	class Factor: public Ada
	{
		public:
			Factor( int hp = 0, int sp = 0 ) : Ada(hp, sp) {};
			~Factor();
			int getHp() {
				std::cout << "Hp(Factor) : " << this->hp << std::endl;
				return this->hp;
			};
	};

	Factor::~Factor(void) {
		std::cout << "Factor obj was destroyed~" << std::endl;
	}

}

#endif

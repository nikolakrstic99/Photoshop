#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <map>

#include "BMP.h"
#include "PAM.h"
#include "Layer.h"
#include "Image.h"
#include "Selection.h"
#include "Rectangle.h"

#include "Operation.h"
#include "Abs.h"
#include "Add.h"
#include "Div.h"
#include "toBlackAndWhite.h"
#include "Add.h"
#include "CompositeOperation.h"
#include "Log.h"
#include "InvSub.h"
#include "Mul.h"
#include "InvDiv.h"
#include "Inversion.h"
#include "Max.h"
#include "Medijana.h"
#include "Min.h"
#include "Power.h"
#include "Sub.h"
#include "toGrey.h"

void printAll();
Operation* pick(int);

class madeImage :public std::exception {
	const char* what()const override {
		return "Vec ste napravili sliku";
	}
};

class noImage :public std::exception {
	const char* what()const override {
		return "Niste napravili sliku";
	}
};

class badIndex :public std::exception {
	const char* what()const override {
		return "Los Indeks";
	}
};

class noLayer :public std::exception {
	const char* what()const override {
		return "Postoji slika ali ne i layer unutar nje";
	}
};

int main(int argc, char** argv) {
	Image i;
	char ch;
	int tmp1, tmp2, tmp3, tmp4, n;
	bool imageMaked = false, active = false;
	std::vector<Rectangle> rectangles;
	Operation* c = nullptr;
	Operation* tmpOperation;
	
	
	//if (argc == 3) {
		/*
		i = Image(argv[1],100,true); imageMaked = true;
		std::string s = i.loadComposite(argv[2]);
		i.doOperation(s);
		i.write("ispis.bmp");
		exit(1);
		*/
	if (argc == 4) {
		i = Image();
		i.setExtension(argv[3]);
		i.read(argv[1]);
		//ovde radi
		std::string s = i.loadComposite(argv[2]);

		std::ofstream file("proba.txt");
		file << s << std::endl;
		file << argv[1] << "     ";
		file << argv[2] << "       ";
		file << argv[3] << "      ";
		file.close();

		i.doOperation(s);
		i.save("projekat2.xml");
	}
	else if (argc == 3) {
		i = Image();
		i.read(argv[1]);
		i.write(argv[2]);
	}
		
		/*
		int opt;
		std::string str, str2;
		
		
		while (1) {
			try {
		std::cout << "GLAVNI MENI" 							<< std::endl
			<< "-------------------------------"			<< std::endl
			<< "1. Ucitajte sliku sa dimenzijama"			<< std::endl
			<< "2. Ucitajte sliku u PAM/BMP formatu"		<< std::endl
			<< "3. Ucitajte projekat"						<< std::endl			
			<< "4. Sacuvajte sliku kao projekat"			<< std::endl
			<< "5. Dodajte selekciju u sliku"				<< std::endl
			<< "6. Obojite selekciju slike"					<< std::endl
			<< "7. Ispisi sliku"							<< std::endl
			<< "8. Dodajte operaciju/e u sliku"				<< std::endl			
			<< "9. Izvrsite kompozitnu operaciju"			<< std::endl
			<< "10. Obrisi Layer"							<< std::endl
			<< "11. Dodaj Layer"							<< std::endl
			<< "12. Obrisi selekciju"						<< std::endl
			<< "13. Sacuvajte kompozitnu fumkciju"			<< std::endl
			<< "0. Izlaz!"									<< std::endl << std::endl
			<<	"Unesite opciju:"							<< std::endl;
			std::cin >> opt;
			
			switch (opt) {
			case 1:
				if (imageMaked) throw madeImage();
				std::cout << "Unesite sirinu pa visinu slike:" << std::endl;
				std::cin >> tmp1; std::cin >> tmp2;
				i = Image(tmp1, tmp2);
				imageMaked = true;
				break;
			case 2:
				if (imageMaked) throw madeImage();
				std::cout << "Unesite ime fajla:" << std::endl;
				std::cin >> str;
				i = Image(str);
				str.clear();
				imageMaked = true;
				break;
			case 3:
				if (imageMaked) throw madeImage();
				std::cout << "Unesite ime fajla:" << std::endl;
				std::cin >> str;
				i = Image();
				i.read(str);
				imageMaked = true;
				break;
			case 4:
				if (imageMaked==false) throw noImage();
				std::cout << "Unesite ime fajla u kom zelite da cuvate";
				std::cin >> str;
				i.save(str);
				break;
			case 5:
				if (imageMaked == false) throw noImage();
				std::cout << "Koliko ima pravougaonika vasa selekcija?";
				std::cin >> n;
				rectangles.clear();
				for (int i = 0; i < n; i++) {
					std::cout << "Unesite vrstu " << i + 1 << ". pravougaonika: ";
					std::cin >> tmp1;
					std::cout << "Unesite kolonu " << i + 1 << ". pravougaonika: ";
					std::cin >> tmp2;
					std::cout << "Unesite sirinu " << i + 1 << ". pravougaonika: ";
					std::cin >> tmp3;
					std::cout << "Unesite visinu " << i + 1 << ". pravougaonika: ";
					std::cin >> tmp4;
					rectangles.push_back(Rectangle(tmp1, tmp2, tmp3, tmp4));
				}
				std::cout << "Kako se zove vasa selekcija?" << std::endl;
				std::cin >> str;
				std::cout << "Da li je vasa selekcija aktivna?" << std::endl;
				std::cin >> tmp1;
				i.addSelection(Selection(str, rectangles,tmp1));
				str.clear();
				break;
			case 6:
				if (imageMaked == false) throw noImage();
				do {
					std::cout << "Unesite crvenu komponentu:(0-255)" << std::endl;
					std::cin >> tmp1;
				} while (tmp1 < 0 || tmp1>255);
				do {
					std::cout << "Unesite zelenu komponentu:(0-255)" << std::endl;
					std::cin >> tmp2;
				} while (tmp2 < 0 || tmp2>255);
				do {
					std::cout << "Unesite plavu komponentu:(0-255)" << std::endl;
					std::cin >> tmp3;
				} while (tmp3 < 0 || tmp3>255);
				do {
					std::cout << "Unesite alpha komponentu:(0-255)" << std::endl;
					std::cin >> tmp4;
				} while (tmp4 < 0 || tmp4>255);
				std::cout << "Naziv koje selekcije zelite da obojite je" << std::endl;
				std::cin >> str;
				i.paintSelection(str, Pixel(tmp1, tmp2, tmp3, tmp4));
				str.clear();
				break;
			case 7:
				if (imageMaked == false) throw noImage();
				if (i.getOne() == nullptr)throw noLayer();
				std::cout << "Unesite ime izlaznog fajla: ";
				std::cin >> str;
				i.write(str);
				break;
			case 8:
				if (imageMaked == false) throw noImage();
				std::cout << "Unesite ime kompozitne operacije: ";
				std::cin >> str;
				c = new CompositeOperation(str);
				tmp1 = 1;
				while (tmp1) {
					printAll();
					std::cin >> tmp1;
					if (tmp1 == 16) {
						std::cout << "Unesite ime kompozitne operacije:" << std::endl;
						std::cin >> str;
							tmpOperation = i.getOperations()[str];
							if (tmpOperation == nullptr)throw noOperation();
							else c->addOperation(tmpOperation);
						
					}else 
						c->addOperation(pick(tmp1));
					std::cout << "Jos operacija(0-ne,1-da: )";
					std::cin >> tmp1;
				}
				i.addOperation(c);
				
				break;
			case 9:
				if (imageMaked == false) throw noImage();
				std::cout << "Unesite ime operacije: ";
				std::cin >> str;
				i.doOperation(str);
				break;
			case 10:
				if (imageMaked == false) throw noImage();
				tmp1 = 0;
				for (Layer l : i.getLayers()) {
					std::cout << tmp1 << ". "<< l.getId()<< ", " << l.getTransparency()<< std::endl;
					tmp1++;
				}
				std::cout << "Unesite poziciji(dubinu) layera koji zelite da obrisete: ";
				std::cin >> tmp1;
				if (tmp1 >= i.getLayers().size() || tmp1 < 0)throw badIndex();
				i.removeLayer(tmp1);
				break;
			case 11:
				if (imageMaked == false) throw noImage();
				std::cout << "Unesite ime datoteke" << std::endl;
				std::cin >> str;
				std::cout << "Unesite aktivnost vaseg sloja: 0-neaktivan" << std::endl;
				std::cin >> tmp1; active = tmp1 == 0 ? false : true;
				do {
					std::cout << "Unesite transparentnost vaseg sloja: 0-100,0-providno" << std::endl;
					std::cin >> tmp1;
				} while (tmp1 > 100 || tmp1 < 0);
				i.addLayer(str, active, tmp1);
				break;
			case 12:
				if (imageMaked == false) throw noImage();
				std::cout << "Kako se zove vasa selekcija?" << std::endl;
				std::cin >> str;
				if (i.getSelections().find(str) == i.getSelections().end()) throw noSelection();
				i.getSelections().erase(str);
				break;
			case 13:
				if (imageMaked == false) throw noImage();
				std::cout << "Kako se zove vasa kompozitna operacija?" << std::endl;
				std::cin >> str;
				if (i.getOperations().find(str) == i.getOperations().end())throw noOperation();

				std::cout << "Kako se zove fajl u koji cutavate kompozitnu f-ju?" << std::endl;
				std::cin >> str2;

				i.saveComposite(str2,str);
				break;
			case 0:
				exit(0);
				break;
			default:
				std::cout<<"Pogresan unos proajte ponovo!\n";
				break;
			}
			std::cout << std::endl << std::endl;
	}
	catch (std::exception& g) {
		std::cout << g.what() << std::endl << std::endl;
	}
}
	
	
	system("pause");
	*/
}


void printAll() {
	std::cout << "1. Add" << std::endl
		<< "2. Sub" << std::endl
		<< "3. InvSub" << std::endl
		<< "4. Mull" << std::endl
		<< "5. Div" << std::endl
		<< "6. InvDiv" << std::endl
		<< "7. Power" << std::endl
		<< "8. Log" << std::endl
		<< "9. Abs" << std::endl
		<< "10. Min" << std::endl
		<< "11. Max" << std::endl
		<< "12. Inversion" << std::endl
		<< "13. Gray" << std::endl
		<< "14. BlackWhite" << std::endl
		<< "15. Medijana" << std::endl
		<< "16. Vec postojecu kompozitnu operaciju" << std::endl
		<< "Unesite broj:";
}

Operation* pick(int n) {
	int tmp;
	Operation* a;
	switch (n) {
	case 1:
		std::cout << "Unesite konstantu";
		std::cin >> tmp;
		a = new Add(tmp);
		return a;
		break;
	case 2:
		std::cout << "Unesite konstantu";
		std::cin >> tmp;
		a = new Sub(tmp);
		return a;
		break;
	case 3:
		std::cout << "Unesite konstantu";
		std::cin >> tmp;
		a = new InvSub(tmp);
		return a;
		break;
	case 4:
		std::cout << "Unesite konstantu";
		std::cin >> tmp;
		a = new Mul(tmp);
		return a;
		break;
	case 5:
		std::cout << "Unesite konstantu";
		std::cin >> tmp;
		a = new Div(tmp);
		return a;
		break;
	case 6:
		std::cout << "Unesite konstantu";
		std::cin >> tmp;
		a = new InvDiv(tmp);
		return a;
		break;
	case 7:
		std::cout << "Unesite konstantu";
		std::cin >> tmp;
		a = new Power(tmp);
		return a;
		break;
	case 8:
		a = new Log();
		return a;
		break;
	case 9:
		a = new Abs();
		return a;
		break;
	case 10:
		std::cout << "Unesite konstantu";
		std::cin >> tmp;
		a = new Min(tmp);
		return a;
		break;
	case 11:
		std::cout << "Unesite konstantu";
		std::cin >> tmp;
		a = new Max(tmp);
		return a;
		break;
	case 12:
		a = new Inversion();
		return a;
		break;
	case 13:
		a = new toGrey();
		return a;
		break;
	case 14:
		a = new toBlackAndWhite();
		return a;
		break;
	case 15:
		a = new Medijana();
		return a;
		break;	
	default:return nullptr;
		break;
	}
}
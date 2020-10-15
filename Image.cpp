#include "Image.h"

Image::Image(std::string s) {											//image sa 1 slojem koji ima sliku
	predefined();
	one = nullptr;
	bool active;
	int tmp1;
	std::cout << "Unesite aktivnost vaseg sloja: 0-neaktivan" << std::endl;
	std::cin >> tmp1; active = tmp1 == 0 ? false : true;
	do {
		std::cout << "Unesite transparentnost vaseg sloja: 0-100,0-providno" << std::endl;
		std::cin >> tmp1;
	} while (tmp1 > 100 || tmp1 < 0);
	Layer* tmp = new Layer(s,active,tmp1);
	layers.push_back(*tmp);
	updateOne(*tmp);
	height = (*one).getHeight();
	width = (*one).getWidth();
}

Image::Image(std::string s, int i, bool b) {
	predefined();
	one = nullptr;
	
	Layer* tmp = new Layer(s, b, i);
	layers.push_back(*tmp);
	updateOne(*tmp);
	height = (*one).getHeight();
	width = (*one).getWidth();
}

Image::Image(int w, int h) :width(w), height(h), layers(0) {			//prazna klasa sa predefinisanim fjama
	predefined();
	one = nullptr;
};

void Image::write(std::string s) {
	std::vector<Pixel> tmp;
	
	if (s.substr(s.length() - 3) == "pam") {
		PAM b(width, height);
		b.setVector((*one).getPixels());
		b.write(s);
	}
	else if (s.substr(s.length() - 3) == "bmp") {
		BMP b(width, height);
		b.setVector((*one).getPixels());
		b.write(s);
	}
	else {
		throw OutputFormat();
	}

}
/*
Image::~Image() {
	delete one;
	layers.clear();
	operations.clear();
	operations.clear();
	hashMap.clear();
}
*/
void Image::addLayer(std::string s, bool _active, int _transparency ) {
	
	Layer l = Layer(s,_active,_transparency);
	
	if (l.getHeight() == height && l.getWidth() == width){
		(*this).updateOne(l);
		layers.push_back(l);	
	}
	else if((l.getHeight() <= height && l.getWidth() <= width)){
		l.extend(width, height, l.getWidth(), l.getHeight() );
		(*this).updateOne(l);
		layers.push_back(l);
	}
	else if ((l.getHeight() >= height && l.getWidth() >= width)) {
		height = l.getHeight() > height ? l.getHeight() : height;
		width = l.getWidth() > width ? l.getWidth() : width;

		layers = extendAll();
		delete one;
		one = nullptr;
		for (int i = 0; i < layers.size(); i++) {
			if (layers[i].getActive())updateOne(layers[i]);
		}
	}
	else {
		height = l.getHeight() > height ? l.getHeight() : height;
		width = l.getWidth() > width ? l.getWidth() : width;
		l.extend(width, height, l.getWidth(), l.getHeight());
		layers = extendAll();
		delete one;
		one = nullptr;
		for (int i = 0; i < layers.size(); i++) {
			if (layers[i].getActive())updateOne(layers[i]);
		}
	}
	(*this).updateOne(l);
	layers.push_back(l);
}

void Image::removeLayer(int n) {

	layers.erase(layers.begin() + n);
	delete one;
	one = nullptr;
	for (int i = 0; i < layers.size(); i++) {
		if (layers[i].getActive())
			updateOne(layers[i]);
	}
}

void Image::updateOne(Layer& l) {
	double a = 0, r = 0, g = 0, b = 0,n = 0, a0, a1;

	std::vector<Pixel> n1 = l.getPixels();
	


	n = (double)l.getTransparency() / 100;
	
	for (Pixel& p : n1)
		p.setTransparency((double)p.getTransparency() * n);
	
	if (one == nullptr) {
		one = new Layer(width,height,true,l.getTransparency());
		one->setPixels(n1);
		one->setHeight(l.getHeight());
		one->setWidth(l.getWidth());
		return;
	}

	std::vector<Pixel> n0 = (*one).getPixels(), tmpPixels;

	for (int k = 0; k < width * height; k++) {
		a0 = (double)n0[k].getTransparency() / 255;
		a1 = (double)n1[k].getTransparency() / 255;
		a = a0  + (1 - a0 ) * a1 ;
		r = (double)n0[k].getRed() * (a0 / a ) + ((double)n1[k].getRed() * (1 - a0) * a1 / a );
		g = (double)n0[k].getGreen() * (a0 / a ) + ((double)n1[k].getGreen() * (1 - a0) * a1 / a );
		b = (double)n0[k].getBlue() * (a0 / a ) + ((double)n1[k].getBlue() * (1 - a0 ) * a1 / a );
		tmpPixels.push_back(Pixel((int) r,(int) g,(int) b, (a*255)));
	}
	

	one->setPixels(tmpPixels);
	tmpPixels.clear();
	double t = one->getTransparency() * (1 + l.getTransparency() / 100) / 100;
	t = t >= 1 ? 1 : t;
	
}

std::vector<Layer> Image::extendAll() {
	std::vector<Layer>tmp;
	for (Layer i : layers) {
		i.extend(width, height, i.getWidth(), i.getHeight());
		tmp.push_back(i);
	}
	return tmp;
}

void Image::addSelection(Selection selection) {
	for(Rectangle r:selection.getRectangles()) {
		if ((r.getRow() + r.getHeight()) > height || (r.getColumn() + r.getWidth()) > width) {
			throw SelectionOutOfRange();
		}
	};
	selections.insert(std::pair<std::string, Selection>(selection.getString(),selection));
}

bool Image::isActiveSelections() {
	for (auto it1 = selections.begin(); it1 != selections.end(); ++it1)
		if (it1->second.isActive())return true;
	return false;	
}

bool Image::isActivePixel(int r, int c) {
	std::set<Rectangle> rectangles;
	for (auto it1 = selections.begin(); it1 != selections.end(); ++it1) {
		rectangles = it1->second.getRectangles();
		if (it1->second.isActive()) {
			for (auto it2 = rectangles.begin(); it2 != rectangles.end(); ++it2) {
				if (r >= it2->getRow() && r <= (it2->getRow() + it2->getHeight()) && c >= it2->getColumn() && c <= (it2->getColumn() + it2->getWidth()))
					return true;
			}
		}
	}
	return false;
}

void Image::paintSelection(std::string s, Pixel p) {
	if (selections.find(s) == selections.end()) throw noSelection();
	for (auto it1 = selections.begin(); it1 != selections.end(); ++it1) {
		if (it1->first == s) {
			std::set<Rectangle> rectangles = it1->second.getRectangles();
			for (auto it2 = rectangles.begin(); it2 != rectangles.end(); ++it2) {
				for (Layer& l : layers) {
					for (int i = it2->getRow(); i < (it2->getRow() + it2->getHeight()); i++)
						for (int j = it2->getColumn(); j < (it2->getColumn() + it2->getWidth()); j++)
							l.pixels[i * width + j] = p;
				}
			}
		}
	}
	
	delete one;
	one = nullptr;
	for (int i = 0; i < layers.size(); i++) {
		if (layers[i].getActive())
			updateOne(layers[i]);
	}
}

void Image::addOperation(Operation* o) {
	operations.insert(std::pair<std::string, Operation*>(o->getName(), o));
}

void Image::predefined() {
	Operation* inv, * blackWhite, * grey, * medijana;
	inv = new Inversion();
	operations.insert(std::pair<std::string, Operation*>("Inversion", inv));
	blackWhite = new toBlackAndWhite();
	operations.insert(std::pair<std::string, Operation*>("toBlackAndWhite", blackWhite));
	grey = new toGrey();
	operations.insert(std::pair<std::string, Operation*>("toGrey", grey));
	medijana = new Medijana();
	operations.insert(std::pair<std::string, Operation*>("Medijana", medijana));
}

void Image::save(std::string str) {
	if (str.substr(str.length() - 3) != "xml") throw OutputFormat();
	std::ofstream file(str);
	int numOfLayer = 1;
	std::string s = "layer.txt";
	std::set<Rectangle> r;
	std::set<Rectangle>::iterator it;

	file << "< ? xml version = 1.0 ? >" <<std::endl;
	
	file << "<Image>" << std::endl;
	file << "<Width>" << width << "</Width>" << std::endl;

	file << "<Height>" << height << "</Height>" <<std::endl;
	file << "<One>" << std::endl;
	file << "<oneWidth>" << one->getWidth() << "</oneWidth>" << std::endl;
	file << "<oneHeight>" << one->getHeight() << "</oneHeight>" << std::endl;
	file << "<oneTransparency>" << one->getTransparency() << "</oneTransparency>" << std::endl;
	file << "<oneActive>" << one->getActive() << "</oneActive>" << std::endl;
	file << "<oneId>" << one->getId() << "</oneId>" << std::endl;
	file << "</One>" << std::endl;
	
	s = std::to_string(numOfLayer) + s;
	numOfLayer++;
	std::ofstream filePixels(s);
	file << "<LayersSize>" << one->getPixels().size() << "</LayersSize>" << std::endl << std::endl;
	for (auto& item : one->getPixels())								//ucitavanje zajednickog layera
	{
		filePixels << item.getRed() << ",";
		filePixels << item.getGreen() << ",";
		filePixels << item.getBlue() << ",";
		filePixels << item.getTransparency() << ",";
	}
	filePixels.close();

	file << "<Layers>" << std::endl;
	file << "<LayersSize>"<< layers.size() << "</LayersSize>" << std::endl;
	for (Layer l:layers) {
		file << "<LayerId>" << l.getId() << "</LayerId>" << std::endl;
		file << "<LayerTransparency>" << l.getTransparency() << "</LayerTransparency>" << std::endl;
		file << "<LayerActive>" << l.getActive() << "</LayerActive>" << std::endl;
		
		s = "layer.txt";													//ucitavanje svih layera
		s = std::to_string(numOfLayer) + s;
		numOfLayer++;
		std::ofstream filePixels(s);
		for (auto& item : one->getPixels())
		{
			filePixels << item.getRed() << ",";
			filePixels << item.getGreen() << ",";
			filePixels << item.getBlue() << ",";
			filePixels << item.getTransparency() << ",";
		}
		filePixels.close();
		
	}
	file << "</Layers>" << std::endl << std::endl;

	file << "<Selection>" << std::endl;
	file << "<SelectionsSize>" << selections.size() << "</SelectionsSize>" << std::endl;
	std::for_each(selections.begin(), selections.end(), [&file,&r,&it](std::pair<std::string, Selection> s) {
		file << "<SelectionName>" << s.second.getString() << "</SelectionName>" << std::endl;
		file << "<SelectionActive>" << s.second.isActive() << "</SelectionActive>" << std::endl;
		file << "<RectanglesSize>" << s.second.getRectangles().size() << "</RectanglesSize>" << std::endl;
		r = s.second.getRectangles();
		file << "<Rectanlges>" << std::endl;
		for (it = r.begin(); it != r.end(); it++) {
			file << "<Width>" << it->getWidth() << "</Width>" << std::endl;
			file << "<Height>" << it->getHeight() << "</Height>" << std::endl;
			file << "<Row>" << it->getRow() << "</Row>" << std::endl;
			file << "<Column>" << it->getColumn() << "</Column>" << std::endl;
		}
		file << "</Rectanlges>" << std::endl;
		
	});
	file << "</Selection>" << std::endl << std::endl;

	file << "<CompositeOperations>" << std::endl;
	file << "<NumberOfComposite>" << operations.size() << "</NumberOfComposite>" <<std::endl<<std::endl;
	for_each(operations.begin(), operations.end(), [&file,this](std::pair<std::string,Operation*> p) {
		
		
		p.second->getXML(file);
		file << std::endl;
	
	});
		
	file << "</CompositeOperations>" << std::endl;
	file << "</Image>";
	
	file.close();
}

void Image::read(std::string str) {
	if (str.substr(str.length() - 3) != "xml") throw InputFormat();
	std::ifstream file(str);
	std::string line, text;
	std::smatch result;
	std::vector<Pixel> pixels;
	std::vector<Layer> tmpLayers;
	std::vector<Rectangle> tmpRectangles;
	Layer tmpLayer;
	int bound, bound2, r, g, b, a, w, h,numOfLayer=1;
	char c;
	getline(file, line);
	std::regex rx("<Image><Width>([0-9]*)</Width><Height>([0-9]*)</Height><One><oneWidth>([0-9]*)</oneWidth><oneHeight>([0-9]*)</oneHeight><oneTransparency>([0-9]*)</oneTransparency><oneActive>([0-1]*)</oneActive><oneId>([0-9]*)</oneId></One>");
	for (int i = 0; i < 10; i++) {
		getline(file, line);
		text += line;
	}
	
	if (regex_match(text, result, rx)) {
		width = stoi(result.str(1));
		height = stoi(result.str(2));
		one = new Layer();
		one->setWidth(stoi(result.str(3))); 
		one->setHeight(stoi(result.str(4)));
		one->setTransparency(stoi(result.str(5)));
		one->setActive(stoi(result.str(6)));
		one->setId(stoi(result.str(7)));
	}
	text.clear();
	std::regex rx1("<LayersSize>([0-9]*)</LayersSize><LayersSize>([0-9]*)</LayersSize>");
	getline(file, line);
	text = line;
	getline(file, line);//prazan red
	getline(file, line);//layers
	getline(file, line);//layersSize
	text += line;
	if (regex_match(text, result, rx1)) {
		bound = stoi(result.str(1));	//broj piksela u layeru
		bound2 = stoi(result.str(2));	//broj layera
	}

	
	std::string s = "layer.txt";
	s = extension + std::to_string(numOfLayer) + s;				//izmena zbog jave
	numOfLayer++;
	std::ifstream filePixels(s);
	for (int i = 0; i < bound; i++) {
		filePixels >> r; filePixels >> c;
		filePixels >> g; filePixels >> c;
		filePixels >> b; filePixels >> c;
		filePixels >> a; filePixels >> c;
		pixels.push_back(Pixel(r, g, b, a));
	}
	filePixels.close();
	one->setPixels(pixels);
	pixels.clear();
	
	for (int i = 0; i < bound2; i++) {
		text.clear();
		for (int i = 0; i < 3; i++) {
			getline(file, line);
			text += line;
		}
		tmpLayer = new Layer();
		std::regex rx2("<LayerId>([0-9])*</LayerId><LayerTransparency>([0-9]*)</LayerTransparency><LayerActive>([0-1]*)</LayerActive>");
		if (regex_match(text, result, rx2)) {
			tmpLayer.setId(stoi(result.str(1)));
			tmpLayer.setTransparency(stoi(result.str(2)));
			tmpLayer.setActive(stoi(result.str(3)));
		}
		tmpLayer.setHeight(one->getHeight());
		tmpLayer.setWidth(one->getWidth());
		
		s = "layer.txt";
		s = extension + std::to_string(numOfLayer) + s;				///izmena zbog jave
		numOfLayer++;
		std::ifstream filePixels(s);
		for (int i = 0; i < bound; i++) {
			filePixels >> r; filePixels >> c;
			filePixels >> g; filePixels >> c;
			filePixels >> b; filePixels >> c;
			filePixels >> a; filePixels >> c;
			pixels.push_back(Pixel(r, g, b, a));
		}
		tmpLayer.setPixels(pixels);
		filePixels.close();
		tmpLayers.push_back(tmpLayer);
	}
	layers = tmpLayers;
	getline(file, line);//kraj layers
	getline(file, line);//preazan red
	getline(file, line);//selection
	getline(file, line);//selectionsSize

	std::regex rx3("<SelectionsSize>([0-9]*)</SelectionsSize>");
	if (regex_match(line, result, rx3)) {
		bound = stoi(result.str(1));
	}
	
	std::string name;
	int active,col;
	for (int i = 0; i < bound; i++) {		//Selekcije
		text.clear();
		getline(file, line);
		text += line;
		getline(file, line);//preazan red
		text += line;
		getline(file, line);
		text += line;
		std::regex rx4("<SelectionName>([^<]*)</SelectionName><SelectionActive>([0-1]*)</SelectionActive><RectanglesSize>([0-9]*)</RectanglesSize>");
		if (regex_match(text, result, rx4)) {
			name = result.str(1);
			active = stoi(result.str(2));
			bound2 = stoi(result.str(3));
		}

		getline(file, line);	//<Rectangles>
		for (int j = 0; j < bound2; j++) {
			text.clear();
			for (int k = 0; k < 4; k++) {
				getline(file, line);
				text += line;
			}
			std::regex rx5("<Width>([0-9]*)</Width><Height>([0-9]*)</Height><Row>([0-9]*)</Row><Column>([0-9]*)</Column>");
			if (regex_match(text, result, rx5)) {
				w = stoi(result.str(1));
				h = stoi(result.str(2));
				r = stoi(result.str(3));
				col = stoi(result.str(4));
			}
			tmpRectangles.push_back(Rectangle(r, col, w, h));
		}
		getline(file, line);	//</Rectangles>
		Selection tmpSelection(name, tmpRectangles,active);
		

		selections.insert(std::pair<std::string, Selection>(name, tmpSelection));
		tmpRectangles.clear();
		//tmpSelection.clear();

	}

	getline(file, line);//</Selection>
	getline(file, line);//prazan red
	getline(file, line);//<CompositeOperations>
	getline(file, line);//<NumberOfComposite>

	

	std::regex rx6("<NumberOfComposite>([0-9])*</NumberOfComposite>");
	fillHashMap();
	if (regex_match(line, result, rx6))
		bound = stoi(result.str(1));
	Operation* o;
	for (int i = 0; i < bound; i++) {
		
		getline(file, line);//prazan red sa vrha
		text.clear();
		getline(file, line);
		text += line;
		getline(file, line);
		text += line;
		std::regex rxBasic("<Op>([^<]*)</Op><Const>([^<]*)</Const>");
		std::regex rxComposite("<OperationName>([^<]*)</OperationName><OperationSize>([0-9]*)</OperationSize>");
		if (regex_match(text, result, rxBasic)) {
			//basic operacija
			line = result.str(1);
			r = stoi(result.str(2));
			o = returnOp(line, r); 
			operations.insert(std::pair<std::string, Operation*>(o->getName(), o));
			delete o;
			o = nullptr;
		}
		else if (regex_match(text, result, rxComposite)) {
			//Composite operacija
			line = result.str(1);
			r = stoi(result.str(2));
			o = dynamic_cast<Operation*>(readComposite(file, line, r));
			operations.insert(std::pair<std::string, Operation*>(o->getName(), o));
			//delete o;
			//o = nullptr;
		}
	}
	file.close();
}

void Image::fillHashMap() {
	hashMap.insert(std::pair<std::string, int>("Add", 1));
	hashMap.insert(std::pair<std::string, int>("Sub", 2));
	hashMap.insert(std::pair<std::string, int >("InvSub", 3));
	hashMap.insert(std::pair<std::string, int>("Mul", 4));
	hashMap.insert(std::pair<std::string, int>("Div", 5));
	hashMap.insert(std::pair<std::string, int >("InvDiv", 6));
	hashMap.insert(std::pair<std::string, int>("Power", 7));
	hashMap.insert(std::pair<std::string, int>("Log", 8));
	hashMap.insert(std::pair<std::string, int>("Abs", 9));
	hashMap.insert(std::pair<std::string, int>("Min", 10));
	hashMap.insert(std::pair<std::string, int>("Max", 11));
	hashMap.insert(std::pair<std::string, int>("Inversion", 12));
	hashMap.insert(std::pair<std::string, int>("toGrey", 13));
	hashMap.insert(std::pair<std::string, int>("toBlackAndWhite", 14));
	hashMap.insert(std::pair<std::string, int>("Medijana", 15));
}

Operation* Image::readComposite(std::ifstream& file, std::string name, int tmp) {
	Operation* o = nullptr;
	Operation* c = new CompositeOperation(name);
	int r;
	std::string text, line;
	std::smatch result;

	for (int i = 0; i < tmp; i++) {
		text.clear();
		getline(file, line);
		text += line;
		getline(file, line);
		text += line;
		std::regex rxBasic("<Op>([^<]*)</Op><Const>([^<]*)</Const>");
		std::regex rxComposite("<OperationName>([^<]*)</OperationName><OperationSize>([0-9]*)</OperationSize>");
		if (regex_match(text, result, rxBasic)) {
			//basic operacija
			line = result.str(1);
			r = stoi(result.str(2));
			o = returnOp(line, r);
			c->addOperation(o);
		}
		else if (regex_match(text, result, rxComposite)) {
			//Composite operacija
			line = result.str(1);
			r = stoi(result.str(2));
			o = dynamic_cast<Operation*>(readComposite(file, line, r));
			c->addOperation(o);
		}
	}
	return c;
};

Operation* Image::returnOp(std::string s, int n) {
	int tmp = hashMap[s];
	switch (tmp) {
	case 1:return new Add(n);
		break;
	case 2:return new Sub(n);
		break;
	case 3:return new InvSub(n);
		break;
	case 4:return new Mul(n);
		break;
	case 5:return new Div(n);
		break;
	case 6:return new InvDiv(n);
		break;
	case 7:return new Power(n);
		break;
	case 8:return new Log();
		break;
	case 9:return new Abs();
		break;
	case 10:return new Min(n);
		break;
	case 11:return new Max(n);
		break;
	case 12:return new Inversion();
		break;
	case 13:return new toGrey();
		break;
	case 14:return new toBlackAndWhite();
		break;
	case 15:return new Medijana();
		break;
	default:return nullptr;
		break;
	}
}

void Image::doOperation(std::string s) {
	Operation* d;
	std::vector<Pixel> tmpPixels;
	if (operations.find(s) == operations.end()) throw noOperation();
	else {
		d = operations[s];
		if (typeid(*d) == typeid(Medijana)) {
			if (isActiveSelections()) {///nad aktivnim selekcijama pikselima
				for (Layer& l : layers) {
					int m = l.getPixels().size();
					for (int i = 0; i < m; i++) {
						if (isActivePixel(i / width, i % width)) {
							d->operation(l.pixels[i]);
							d->scale(l.pixels[i]);
						}
					}
				}
			}
			else {
				for (Layer& l : layers) {
					d->operation(l.pixels, width, height);
					//d->scale(l.pixels[i]);
				}
			}
		}
		else {


			if (isActiveSelections()) {///nad aktivnim selekcijama pikselima
				for (Layer& l : layers) {
					int m = l.getPixels().size();
					for (int i = 0; i < m; i++) {
						if (isActivePixel(i / width, i % width)) {
							d->operation(l.pixels[i]);
							d->scale(l.pixels[i]);
						}
					}
				}
			}
			else {
				for (Layer& l : layers) {
					int m = l.getPixels().size();
					for (int i = 0; i < m; i++) {
						d->operation(l.pixels[i]);
						d->scale(l.pixels[i]);
					}
				}
			}
		}
		delete one;
		one = nullptr;
		for (int i = 0; i < layers.size(); i++) {
			if (layers[i].getActive())updateOne(layers[i]);
		}
	}

}

void Image::saveComposite(std::string str, std::string op) {
	if (str.substr(str.length() - 3) != "fun") throw OutputFormat();
	std::ofstream file(str);
	
	file << "<CompositeOperaion>" << std::endl;
	//file << "<NumberOfComposite>" << operations.size() << "</NumberOfComposite>" << std::endl << std::endl;
	for_each(operations.begin(), operations.end(), [&file, this, op](std::pair<std::string, Operation*> p) {

		if (p.first == op) {
			p.second->getXML(file);
			file << std::endl;
		}
	});

	file << "</CompositeOperaion>" << std::endl;

}

std::string Image::loadComposite(std::string str) {
	if (str.substr(str.length() - 3) != "fun") throw InputFormat();
	std::ifstream file(str);
	std::string line, text;
	int bound, r;
	std::smatch result;
	getline(file, line);//<CompositeOperations>
	
	fillHashMap();
	
	Operation* o;
	//for (int i = 0; i < bound; i++) {

		//getline(file, line);//prazan red sa vrha
		text.clear();
		getline(file, line);
		text += line;
		getline(file, line);
		text += line;
		std::regex rxBasic("<Op>([^<]*)</Op><Const>([^<]*)</Const>");
		std::regex rxComposite("<OperationName>([^<]*)</OperationName><OperationSize>([0-9]*)</OperationSize>");
		if (regex_match(text, result, rxBasic)) {
			//basic operacija
			line = result.str(1);
			r = stoi(result.str(2));
			o = returnOp(line, r);
			operations.insert(std::pair<std::string, Operation*>(o->getName(), o));
			delete o;
			o = nullptr;
		}
		else if (regex_match(text, result, rxComposite)) {
			//Composite operacija
			line = result.str(1);
			r = stoi(result.str(2));
			o = dynamic_cast<Operation*>(readComposite(file, line, r));
			operations.insert(std::pair<std::string, Operation*>(o->getName(), o));
			//delete o;
			//o = nullptr;
		}
		file.close();
		return line;
	//}
}

void Image::setExtension(std::string str) {
	extension = str;
}
#pragma once
#include "Operation.h"
#include <iostream>
#include <map>
#include <algorithm>
#include "Max.h"
#include "Min.h"
#include "Sub.h"
#include "InvSub.h"
#include "Min.h"
#include "Log.h"
#include "Mul.h"
#include "InvDiv.h"
#include "Power.h"
#include "Div.h"
#include "Abs.h"

class CompositeOperation:public Operation {
	std::vector<Operation*> allOperations;
	std::string name;
public:
	CompositeOperation(std::string name) { this->name = name; }

	void addOperation(Operation* o) { allOperations.push_back(o); }
	
	void operation(Pixel& p)override {
		for_each(allOperations.begin(), allOperations.end(), [&p](Operation * o){
			o->op(p);
		});
	}

	void getXML(std::ofstream &file)override {
		
		file << "<OperationName>" << getName() << "</OperationName>" << std::endl;
		file << "<OperationSize>" << getSize() << "</OperationSize>" << std::endl;

		for_each(allOperations.begin(), allOperations.end(), [&file](Operation* o) {
			if (typeid(*o) == typeid(CompositeOperation)) {
				CompositeOperation* o2 = dynamic_cast<CompositeOperation*>(o);
				file << "<OperationName>" << o2->getName() << "</OperationName>" << std::endl;
				file << "<OperationSize>" << o2->getSize() << "</OperationSize>" << std::endl;
				for (int i = 0; i < o->getSize(); i++) {
					o2->getOperations()[i]->getXML(file);
					
				}
			}
			else {
				o->getXML(file);
			}
			
			
		});
		
	}

	int getSize() override{
		return allOperations.size();
	}

	std::vector<Operation*> getOperations() {
		return allOperations;
	}

	void clear() {
		allOperations.clear();
	}

	std::string getName() override {
		return name;
	}
};

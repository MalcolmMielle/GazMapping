#ifndef CONFIDENCE_MAP
#define CONFIDENCE_MAP

#include <iostream>
#include <exception>
#include <stdexcept>
#include <assert.h>
#include <math.h>   
#include <vector>
#include <fstream>

class Confidence_map{

protected :
	double _sigma_omega;
	std::vector<double> _alpha; //Actual Map
	
public:
	Confidence_map(const double& sig_omega) : _sigma_omega(sig_omega){};
	
	//Accessor
	std::vector<double>& getAlpha(){return _alpha;}
	
	//calcul the confidence map as attribute _alpha
	virtual void calculConfidence(std::vector<double>& omega);
	
	//Print
	void printAlpha(){
		for(typename std::vector<double>::iterator it=_alpha.begin();it!=_alpha.end();){
			std::cout << *it << std::endl;
			it++;
		}
	}
	
	void export_data(){
		std::fstream out_alpha("alpha.dat", std::ios_base::out);
		for(int i=0; i <_alpha.size() ; i++){
			out_alpha << i;
			out_alpha << " ";
			out_alpha << _alpha.at(i);
			out_alpha << "\n";
			
		}
		out_alpha.close();
	}
	
};

/*********************FUNCTION IMPLEMENTATION**************/

void Confidence_map::calculConfidence(std::vector< double >& omega){
	_alpha.clear();
	//std::cout << "SIZE IN CONFIDENCE "<<omega.size()<<std::endl;
	for (int i =0; i< omega.size() ; i++){
		
		double coef = - ( omega[i] * omega[i] ) / (_sigma_omega * _sigma_omega);	
		_alpha.push_back(1-exp(coef));
		
	}
}



#endif
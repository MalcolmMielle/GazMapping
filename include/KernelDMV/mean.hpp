#ifndef MEAN
#define MEAN

#include <iostream>
#include <exception>
#include <stdexcept>
#include <assert.h>
#include <math.h>   
#include <vector>

#include "confidence_map.hpp"

class Mean{

protected :
	
	double _r0; // mean concentration for cell with not good enough readings
	std::vector<double> _r_map;
	
public:
	Mean(const double& r) : _r0(r){};
	
	//Accessor
	virtual std::vector<double>& getMeanMap(){return _r_map;}
	virtual double getR() const {return _r0;}
	
	//setters
	virtual void setR(const double& r){_r0 = r;} 
	
	//Map of the means calculated as _r_map attribute
	virtual void calculMean(Confidence_map& cm, std::vector<double>& omega, std::vector<double>& weighted_map);
	
	double getMax(int& i);
	
	void printMean(){
		int i=0;
		for(typename std::vector<double>::iterator it=_r_map.begin();it!=_r_map.end();){
			//std::cout << *it << " value of i "<<i <<std::endl;
			it++;
			i++;
		}
	}
	
	void export_data(){
		std::fstream out_mean("mean.dat", std::ios_base::out);
		for(int i=0; i <_r_map.size() ; i++){
			out_mean << i;
			out_mean << " ";
			out_mean << _r_map.at(i);
			out_mean << "\n";
			
		}
		out_mean.close();
	}
	
};

/***************FUNCTION IMPLEMENTATION*********************/

void Mean::calculMean(Confidence_map& cm, std::vector< double >& omega, std::vector< double >& weighted_map){
	_r_map.clear();
	for(int i= 0;i<omega.size();i++){
		
		//Variable if measurement is good
		double var = cm.getAlpha().at(i)*( weighted_map.at(i)/omega.at(i));
		//variable if measurement is actually really bad
		double var2 = (1 + cm.getAlpha().at(i)) * _r0; 
		
		_r_map.push_back(var+var2);
		
	}
}

double Mean::getMax(int& i){
	double max = 0;
	int j = 0;
	for(typename std::vector<double>::iterator it = _r_map.begin() ; it != _r_map.end() ;){
		if(*it > max){
			max = *it;
			i = j;
		}
		it++;
		j++;
	}
	return max;
}


#endif
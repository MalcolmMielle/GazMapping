#ifndef VARIANCE
#define VARIANCE

#include <iostream>
#include <exception>
#include <stdexcept>
#include <assert.h>
#include <math.h>   
#include <vector>
#include <boost/math/distributions/normal.hpp>

#include "confidence_map.hpp"
#include "mean.hpp"

class Variance{

protected :
	
	std::vector<double> _v_map;
	std::vector<double> _Vk;
	double _sigma;
	double _v0;
	
public:
	Variance(const double& sig) : _sigma(sig), _v0(0){};
	Variance(const double& sig, const double& vo) : _sigma(sig), _v0(vo){};
	
	//Acessor
	virtual std::vector<double>& getVarianceMap(){return _v_map;}
	virtual std::vector<double>& getVK(){return _Vk;}
	virtual double getSigma() const {return _sigma;}
	virtual double getV0() const {return _v0;}
	
	//setter
	virtual void setSigma(const double& sigma){_sigma = sigma;}
	virtual void setV0(const double& v){_v0 = v;}
	
	//Variance Map calculation
	virtual void calculVariance(Mean& mean, Confidence_map& alpha, std::vector<double>& omega, std::vector<double>& weighted_map, std::vector<double>& raw_data, double min, double max, double step);
	//Vk Calculation
	virtual void calculVk(Mean& mean, std::vector<double>& data, double min, double max, double step);
	
	//Get the max and take the id as parameter.
	double getMax(int& i);
	
	virtual void printVarience(){
		for(typename std::vector<double>::iterator it=_v_map.begin();it!=_v_map.end();){
			std::cout << *it << std::endl;
			it++;
		}
	}
	
	virtual void export_data(){
		std::fstream out_variance("variance.dat", std::ios_base::out);
		for(int i=0; i <_v_map.size() ; i++){
			out_variance << i;
			out_variance << " ";
			out_variance << _v_map.at(i);
			out_variance << "\n";
		}
		out_variance.close();
	}
	
};

/***************FUNCTION IMPLEMENTATION*********************/

void Variance::calculVariance(Mean& mean, Confidence_map& alpha, std::vector< double >& omega, std::vector< double >& weighted_map, std::vector< double >& raw_data, double min, double max, double step){
	_v_map.clear();
	calculVk(mean, raw_data, min, max, step);
	for(int i= 0;i<omega.size();i++){
		double var = alpha.getAlpha().at(i) * ( _Vk.at(i) / omega.at(i) );
		double var2 = ( 1 - alpha.getAlpha().at(i) )*_v0;
		_v_map.push_back(var + var2);
		
	}
}

void Variance::calculVk(Mean& mean, std::vector< double >& data, double min, double max, double step){
	_Vk.clear();	
	for(double i = min ; i < max+1 ; i=i+step){
		double sum = 0;
		//Normal distribution
		boost::math::normal nd(0 ,_sigma);
		for(int i_data=0 ; i_data<data.size() ; i_data=i_data+2){
			//FIRST PART OF EQUAtiON
			double dist= data[i_data] -i;
			if (dist <0){
				dist = -dist;
			}
			
			//pdf stands for probibility density function
			double var =  boost::math::pdf(nd, dist);
			
			//SECOND PART
			/*
			* The formula to go from measurement to vector unit is (x - min)/step
			*/			
			double id = (data[i_data] - min) * (1/step) ;
			
			//std::cout << " la donnee " << data[i_data] << " id after " << id << " for i_data " << i_data << " and total size " << data.size() << std::endl;
			
			//Got get the good i now 
			double var2 = mean.getMeanMap().at(id) - mean.getMeanMap().at( (i - min) / step );
			var2 = var2 * var2;
			sum = sum + ( var * var2 );
			
		}
		//std::cout << "out ! "<<i << " with sum " << sum<<std::endl;
		//getchar();
		_Vk.push_back(sum);
	}
}

double Variance::getMax(int& i){
	double max = 0;
	int j = 0;
	for(typename std::vector<double>::iterator it = _v_map.begin() ; it != _v_map.end() ;){
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
#ifndef TEMPORARY_MAP
#define TEMPORARY_MAP

#include <iostream>
#include <exception>
#include <stdexcept>
#include <assert.h>
#include <boost/math/distributions/normal.hpp>

#include <fstream>
/*
 * Class of the two temporory map omega and R.
 * sumMap1D calcul Omega
 * sumMap1D_measurement calcul R.
 * 
 */

class Temp_map{
protected:

	double _sigma;
	std::vector< double > _omega_map;
	std::vector< double > _R_map;
	
public:

	Temp_map(const double& sig) : _sigma(sig){}
	
	//Accessor :
	void setSigma(const double& sig){_sigma=sig;}
	
	double getSigma() const {return _sigma;}
	std::vector<double>& getOmega(){return _omega_map;}
	std::vector<double>& getR(){return _R_map;}
	
	/*
	 * 1D calculation of omega
	 */
	void sumMap1D(std::vector<double>& data, std::vector<double>& end, double min, double max, double step);
	void sumMap1D(std::vector<double>& data, double min, double max, double step);
	void sumMap1D(std::vector<double>& data, std::vector<double>& end, double min, double max, double step, int range);
	
	/*
	 * 1D calculation for the weighted reading
	 */
	void sumMap1D_measurement(std::vector<double>& data, std::vector<double>& end, double min, double max, double step);
	void sumMap1D_measurement(std::vector<double>& data, double min, double max, double step);	
	void sumMap1D_measurement(std::vector<double>& data, std::vector<double>& end, double min, double max, double step, int range);
	
	//All
	void calculate(std::vector<double>& data, double min, double max, double step){
		sumMap1D(data, min, max, step);
		sumMap1D_measurement(data, min, max, step);
	}
	
	void export_data(){
		
		std::fstream out_omega("omega.dat", std::ios_base::out);
		for(int i=0; i <_omega_map.size() ; i++){
			out_omega << i;
			out_omega << " ";
			out_omega << _omega_map.at(i);
			out_omega << "\n";
			
		}
		out_omega.close();
	
		std::fstream out_R("R.dat", std::ios_base::out);
		for(int i=0; i <_R_map.size() ; i++){
			out_R << i;
			out_R << " ";
			out_R << _R_map.at(i);
			out_R << "\n";
			
		}
		out_R.close();
	}
		
	
	
/********************USELESS : FOR MULTIPLE DIMENSION*********************/
/*
 * I just started with the wrong idea that it was more than 1D.
 * So I'm letting this here in case I need it one day
 */

	//Map value calculation
	
};

/*********************FUNCTION IMPLEMENTATION**************/

//min and max are the min and value value measured in distance.
//They are introduced so one can use different dataset without problems.
void Temp_map::sumMap1D(std::vector< double >& data, std::vector< double >& end, double min, double max, double step){
	//For each 1D map point calculate the sum and put it in the end vector
	for(double i=min; i<max+1 ; i=i+step){
		double sum=0;
		double sum_dist=0;
 		boost::math::normal nd(0 ,_sigma); 
		for(int i_data=0 ; i_data<data.size() ; i_data=i_data+2){
			
			double dist= data[i_data] - i;
			if (dist <0){
				dist=-dist;
			}

			double var =  boost::math::pdf(nd, dist);
			sum_dist=sum_dist+dist;
			sum=sum+var;
		}
		end.push_back(sum);
	}
}

void Temp_map::sumMap1D(std::vector< double >& data, double min, double max, double step){
	//For each 1D map point calculate the sum and put it in the end vector
	for(double i=min; i<max+1 ; i=i+step){
		double sum=0;
		double sum_dist=0;
 		boost::math::normal nd(0 ,_sigma); 
		for(int i_data=0 ; i_data<data.size() ; i_data=i_data+2){
			
			double dist= data[i_data] - i;
			if (dist <0){
				dist=-dist;
			}

			double var =  boost::math::pdf(nd, dist);
			sum_dist=sum_dist+dist;
			sum=sum+var;
		}
		_omega_map.push_back(sum);
	}
}


void Temp_map::sumMap1D(std::vector< double >& data, std::vector< double >& end, double min, double max, double step, int range){
	//For each 1D map point calculate the sum and put it in the end vector
	//i represent the actual physical distance. It's a discrete regular line of points.
	for(double i=min; i<max+1 ; i=i+step){
		//std::cout << "we are at " << i <<std::endl;
		double sum=0;
		double sum_dist=0;
 		boost::math::normal nd(0 ,_sigma); 
		
		//Calculate the range
		int i_mid=0;
		int flag=0;
		//First we need to put i_mid at the same "place" as i
		while(data[i_mid] < i && flag==0){
			if (i_mid >= data.size()-2){
				flag=1;
			}
			else{
				i_mid=i_mid+2;
			}		
		}
		
		//Then we check the expection places respecively min and max.
		if(i_mid - range/2 < 0){
			i_mid=0;
		}
		else if(i_mid + range/2 > data.size() ){
			i_mid = data.size() - range;
		}
		else{
			i_mid=i_mid-range/2;
		}
		
		
		// we do 25 measures
		for(int i_data=i_mid ; i_data<i_mid + range ; i_data=i_data+2){
			
			double dist= data[i_data] - i;
			if (dist <0){
				dist=-dist;
			}

			double var =  boost::math::pdf(nd, dist);
			sum_dist=sum_dist+dist;
			sum=sum+var;
			
			//std::cout << "crash at " << i_data+2 << " full size " << data.size()<< " and i_mid +100 " << i_mid+100<< std::endl;
			
		}
		end.push_back(sum);
	}
}


void Temp_map::sumMap1D_measurement(std::vector< double >& data, std::vector< double >& end, double min, double max, double step){
	
	boost::math::normal nd(0 ,_sigma);
	
	//For each 1D map point calculate the sum and put it in the end vector
	for(double i=min; i<max+1 ; i=i+step){
		double sum=0;
		
		for(int i_data=0 ; i_data<data.size() ; i_data=i_data+2){
			
			double dist= data[i_data] -i;
			if (dist <0){
				dist=-dist;
			}
			//Normal distribution
			 
			//TODO unsure of this. Do we really use the data measured?
			double var =  boost::math::pdf(nd, dist) * data[i_data+1];
			sum=sum+var;
		}
		end.push_back(sum);
	}
}

void Temp_map::sumMap1D_measurement(std::vector< double >& data, double min, double max, double step){
	
	boost::math::normal nd(0 ,_sigma);
	
	//For each 1D map point calculate the sum and put it in the end vector
	for(double i=min; i<max+1 ; i=i+step){
		double sum=0;
		
		for(int i_data=0 ; i_data<data.size() ; i_data=i_data+2){
			
			double dist= data[i_data] -i;
			if (dist <0){
				dist=-dist;
			}
			//Normal distribution
			 
			//TODO unsure of this. Do we really use the data measured?
			double var =  boost::math::pdf(nd, dist) * data[i_data+1];
			sum=sum+var;
		}
		_R_map.push_back(sum);
	}
}

void Temp_map::sumMap1D_measurement(std::vector< double >& data, std::vector< double >& end, double min, double max, double step, int range){
	
	boost::math::normal nd(0 ,_sigma);
	
	//For each 1D map point calculate the sum and put it in the end vector
	for(double i=min; i<max+1 ; i=i+step){
		double sum=0;
		
		int i_mid=0;
		int flag=0;
		while(data[i_mid] < i && flag==0){
			if (i_mid >= data.size()-2){
				flag=1;
			}
			else{
				i_mid=i_mid+2;
			}		
		}
		
		if(i_mid - range/2 < 0){
			i_mid=0;
		}
		else if(i_mid + range/2 > data.size() ){
			i_mid=data.size() - range;
		}
		else{
			i_mid=i_mid - range/2;
		}
		// = 25 measures
		for(int i_data=i_mid ; i_data<i_mid + range ; i_data=i_data+2){
			
			double dist= data[i_data] -i;
			if (dist <0){
				dist=-dist;
			}
			//Normal distribution
			 
			//TODO unsure of this. Do we really use the data measured?
			double var =  boost::math::pdf(nd, dist) * data[i_data+1];
			sum=sum+var;
		}
		end.push_back(sum);
	}
}

#endif
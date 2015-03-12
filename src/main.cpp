#include <exception>
#include <stdexcept>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <boost/math/distributions/normal.hpp>
#include <vector>

#include "temp_map.hpp"
#include "confidence_map.hpp"
#include "mean.hpp"
#include "variance.hpp"

#include "utilityfunctions.hpp"


int main(int argc, char * argv[]){
	
	//Read data set :
	std::fstream myfile;
	
	std::cout << "Which dataset do you want to use 1 or 2 ?" <<std::endl;
	
	int inp = 0;
	double min_value=0;
	double max_value=0;
	std::cin >> inp;
	while(inp != 1 && inp !=2){
		std::cout << "choose 1 or 2" << std::endl;
		std::cin >> inp;
	}
	if (inp == 1){
		myfile.open("artTestGdmData1D-01.log", std::ios_base::in);
	}
	else{
		myfile.open("artTestGdmData1D-02.log", std::ios_base::in);
	}

	std::vector<double> locANDmes;
	std::vector<double> locANDmes_non_normalized;
	double a=0;
	int i=0;
	while (myfile >> a)
	{
		locANDmes_non_normalized.push_back(a);
		if(i==0){
			if(a > max_value){
				max_value=a;
			}
			if(a<min_value){
				min_value=a;
			}
			i=1;
		}
		else{
			i=0;
		}
	}
	
	/*
	 * Initialization of hyper variable
	 */
	
	double sigma=0;
	std::cout << "value of sigma from the gauss ? "<<std::endl;
	std::cin >> sigma;
	
	double step=0;
	std::cout << "value of the step ? "<<std::endl;
	std::cin >> step;
	while(step <0){
		std::cout << "Should be more than 0" <<std::endl;
		std::cin >> step;
	}
	
	double m0 = 0;
	std::cout << "value of Ro ? "<<std::endl;
	std::cin >> m0;
	
	double v0 = 0;
	std::cout << "value of v0 ? "<<std::endl;
	std::cin >> v0;
	
	double sigma_omega = 0;
	std::cout << "value of simga_omega for the confidence map ? " <<std::endl;
	std::cin >> sigma_omega;
	
	int norma=-1;
	while (norma != 0 && norma != 1){
		std::cout << "Do you want to normalize the raw data (should be 1 for yes and 0 for no) ? " << std::endl;
		std::cin >> norma;
	}
	
	int range;
	std::cout << "number of point to include in the temporary map calculation ? enter 0 for all " <<std::endl;
	std::cin >> range;
	while(range <0 && range > locANDmes_non_normalized.size()/2 ){
		std::cout << "Should be more than 0 and more than " << locANDmes_non_normalized.size()/2 << std::endl;
		std::cin >> range;
	}
	
	/*
	 * Normalisation
	 */
	
	if(norma == true){
		kerneldmv::normalize(locANDmes_non_normalized, locANDmes);
	}
	else{
		locANDmes.reserve(locANDmes_non_normalized.size());	std::copy(locANDmes_non_normalized.begin(),locANDmes_non_normalized.end(),std::back_inserter(locANDmes));
	}
	
	
	/*
	 * Need to sort the vector
	 */
	kerneldmv::sort_insertion(locANDmes);
	/*
	 * Calcul of the temporary maps
	 */
	Temp_map tmap(sigma);
	
	//Omega
	std::vector<double> omega;
	if(range != 0){
		tmap.sumMap1D(locANDmes, omega, min_value, max_value, step, range);
	}else{
		tmap.sumMap1D(locANDmes, omega, min_value, max_value, step);
	}
	
	//R
	std::vector<double> R;
	if(range != 0){
		tmap.sumMap1D_measurement(locANDmes, R, min_value, max_value, step, range);
	}else{
		tmap.sumMap1D_measurement(locANDmes, R, min_value, max_value, step);
	}
	/*
	 * Calcul of the confidence map
	 */
	Confidence_map cm(sigma_omega);
	cm.calculConfidence(omega);
	
	/*
	 * Calcul of the means map
	 */
	Mean m(m0);
	m.calculMean(cm, omega, R);
	m.printMean();
	
	//std::cout << "checking the size" << " mean " << m.getMeanMap().size() << " omega " << omega.size() <<std::endl;
	std::cout << "the hyper parameters is sigma : "<< sigma<< " and step " << step << std::endl; 
	
	int id = 0;
	double max = m.getMax(id);
	//Convert id to real value : 
	double id_real = id;
	id_real=(id_real/10)+min_value;
	std::cout << "the max value of the mean is : "<< max <<" at "<< id_real <<std::endl;
	
	/*
	 * Calcul of the variances maps
	 */
	Variance v(sigma, v0);
	v.calculVariance(m, cm, omega, R, locANDmes, min_value, max_value, step);
	
	int idv = 0;
	double vmax = v.getMax(idv);
	double idv_real = idv;
	idv_real=(idv_real/10)+min_value;
	std::cout << "the max value of the variance is : "<< vmax <<" at "<< idv_real <<std::endl;
	
	/*OUTPUT FILES for graphical use*/
	
	std::fstream out_omega("omega.dat", std::ios_base::out);
	for(int i=0; i <omega.size() ; i++){
		out_omega << i;
		out_omega << " ";
		out_omega << omega.at(i);
		out_omega << "\n";
		
	}
	out_omega.close();
	
	std::fstream out_R("R.dat", std::ios_base::out);
	for(int i=0; i <R.size() ; i++){
		out_R << i;
		out_R << " ";
		out_R << R.at(i);
		out_R << "\n";
		
	}
	out_R.close();
	
	std::fstream out_alpha("alpha.dat", std::ios_base::out);
	for(int i=0; i <cm.getAlpha().size() ; i++){
		out_alpha << i;
		out_alpha << " ";
		out_alpha << cm.getAlpha().at(i);
		out_alpha << "\n";
		
	}
	out_alpha.close();
	
	std::fstream out_mean("mean.dat", std::ios_base::out);
	for(int i=0; i <m.getMeanMap().size() ; i++){
		out_mean << i;
		out_mean << " ";
		out_mean << m.getMeanMap().at(i);
		out_mean << "\n";
		
	}
	out_mean.close();
	
	std::fstream out_variance("variance.dat", std::ios_base::out);
	for(int i=0; i <v.getVarianceMap().size() ; i++){
		out_variance << i;
		out_variance << " ";
		out_variance << v.getVarianceMap().at(i);
		out_variance << "\n";
		
	}
	out_variance.close();
	
	std::fstream out_varianceKK("varianceKK.dat", std::ios_base::out);
	for(int i=0; i <v.getVK().size() ; i++){
		out_varianceKK << i;
		out_varianceKK << " ";
		out_varianceKK << v.getVK().at(i);
		out_varianceKK << "\n";
		
	}
	out_varianceKK.close();
	
	std::fstream out_nonrawdata("nonrawdata.dat", std::ios_base::out);
	for(int i=0; i <locANDmes.size() ; i++){
		out_nonrawdata << locANDmes.at(i);
		out_nonrawdata << " ";
		i++;
		out_nonrawdata << locANDmes.at(i);
		out_nonrawdata << "\n";
		
	}
	out_nonrawdata.close();
	
	std::fstream out_rawdata("rawdata.dat", std::ios_base::out);
	for(int i=0; i <locANDmes_non_normalized.size() ; i++){
		out_rawdata << locANDmes_non_normalized.at(i);
		out_rawdata << " ";
		i++;
		out_rawdata << locANDmes_non_normalized.at(i);
		out_rawdata << "\n";
		
	}
	out_rawdata.close();
	
	return 0;
	
}

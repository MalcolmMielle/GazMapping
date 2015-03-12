#ifndef KERNELDMV_MAIN_CLASS
#define KERNELDMV_MAIN_CLASS

#include <fstream>

#include "mean.hpp"
#include "variance.hpp"
#include "confidence_map.hpp"
#include "temp_map.hpp"
#include "utilityfunctions.hpp"


class Kernel_DMV{
	
protected:
	
	Mean _mean;
	Variance _variance;
	Confidence_map _conf_map;
	Temp_map _temp_map;
	
	
public:
	Kernel_DMV(const double& sigma_tmp_map, const double& sig_omega_confidence, const double& r_mean, const double& sig_variance, const double& vo_variance) : 
	_mean(r_mean), _variance(sig_variance, vo_variance), 
	_conf_map(sig_omega_confidence), _temp_map(sigma_tmp_map) {};
	
	
	
	//Accessors : 
	virtual void setVarianceSigma(const double& sig){_variance.setSigma(sig);}
	virtual void setVarianceV0(const double& v){_variance.setV0(v);}
	virtual void setMeanR(const double& r){_mean.setR(r);}
	
	virtual const std::vector<double>& getMean(){return _mean.getMeanMap();}
	virtual const std::vector<double>& getVariance(){return _variance.getVarianceMap();}
	
	//Function
	virtual void calculAll(const std::vector<double>& data);
	virtual void exportResults();
	
	
};

inline void Kernel_DMV::calculAll(const std::vector<double>& data)
{
	
	//Process data
	std::vector<double> data_norm;
	
	kerneldmv::normalize(data, data_norm);
	kerneldmv::sort_insertion(data_norm);
	
	double min_distance = data_norm[0];
	double max_distance = data_norm[data_norm.size()-2];
	
	//Calculate tmp_maps
	_temp_map.calculate(data_norm, min_distance, max_distance, 0.01);
	//Calculate confidence
	_conf_map.calculConfidence(_temp_map.getOmega()	);
	//Calculate Mean
	_mean.calculMean(_conf_map, _temp_map.getOmega(), _temp_map.getR() );
	//Calculate variance
	_variance.calculVariance(_mean, _conf_map, _temp_map.getOmega(), _temp_map.getR(), data_norm, min_distance, max_distance, 0.01);	
}

inline void Kernel_DMV::exportResults()
{
	_temp_map.export_data();
	_conf_map.export_data();
	_mean.export_data();
	_variance.export_data();
}




#endif
#include <iostream>
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE MyTest
#include <time.h>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include "temp_map.hpp"
#include "confidence_map.hpp"
#include "mean.hpp"
#include <fstream>

BOOST_AUTO_TEST_CASE(trying)
{
//Read data set :
	std::fstream myfile("artTestGdmData1D-01.log", std::ios_base::in);

	std::vector<double> locANDmes;
	double a;
	int i=0;
	while (myfile >> a && i <10)
	{
		locANDmes.push_back(a);
		i++;
		std::cout << i <<std::endl;
	}
	
	std::cout << locANDmes.size() << std::endl;
	getchar();
	//Calculate first tmp map
	Temp_map tmap(0, 1, 3, 3);
	
	std::vector<double> end;
	tmap.sumMap1D(locANDmes, end, -10, 17, 0.1);
	
	std::cout << "OVER with size end "<< end.size()<<std::endl;
	getchar();
	for(typename std::vector<double>::iterator it=end.begin();it!=end.end();){
		std::cout << *it << " " << std::endl;
		it++;
	}
	
	/***********Confidence map test*********/
	
	Confidence_map cm(1);
	cm.calculConfidence(end);
	std::cout <<std::endl<<"There it is alpha"<<std::endl;
	cm.printAlpha();
	
	
	/**********Testing the mean************/
	Mean mean(2);
	mean.calculMean(cm, end, end);
	mean.printMean();
	
}
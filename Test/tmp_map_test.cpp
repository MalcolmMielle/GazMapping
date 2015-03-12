#include <iostream>
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE MyTest
#include <time.h>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include "temp_map.hpp"
#include <fstream>

BOOST_AUTO_TEST_CASE(trying)
{
	Temp_map map(0, 1, 3, 3);
	Eigen::Vector2d ki(0, 2);
	Eigen::Vector2d kii(0, 0);
	BOOST_CHECK_EQUAL(squaredist(ki, kii), 4);
	ki=-ki;
	BOOST_CHECK_EQUAL(squaredist(ki, kii), 4);
	double test = map.sumMap(Eigen::Vector2d(0,0));
	
	std::cout << "This is the end "<< std::endl << map.getMap() << std::endl;
	
	
	//Read data set :
	std::fstream myfile("artTestGdmData1D-01.log", std::ios_base::in);

	std::vector<double> locANDmes;
	double a=0;
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
	
	/*******TEST weighted *******/
	std::vector<double> weight;
	weight.push_back(1);
	
	//tmap.sumMap1D(locANDmes, end, weight); // Here Should crash because of assert
	weight.push_back(1);
	weight.push_back(1);
	weight.push_back(1);
	weight.push_back(1);
	tmap.sumMap1D_measurement(locANDmes, end, -10, 17, 0.1);
	
	/************************************TEST THAT IT'S GOOD***************/
	std::cout << "TEST petit" <<std::endl;
	std::vector<double> end2;
	std::vector<double> data;
	data.push_back(0.1);
	data.push_back(1);
	data.push_back(0.1);
	data.push_back(1);
	data.push_back(0.1);
	data.push_back(1);
	
	
	std::cout << "size data "<< data.size()<<std::endl;
	getchar();
	
	//For each 1D map point calculate the sum and put it in the end vector
	for(double i=-0.1; i<0.5 ; i=i+0.1){
		double sum=0;
		for(int i_data=0 ; i_data<data.size() ; i_data=i_data+2){
			
			double dist= data[i_data] -i;
			if (dist <0){
				dist=-dist;
			}
			//Normal distribution
			boost::math::normal nd(0 ,1); 
			double var =  boost::math::pdf(nd, dist);
			
			std::cout << "var is "<< var << " with dist : " << dist << " and i " << i_data << std::endl;
			
			sum=sum+var;
		}
		std::cout <<"pushed " << sum << std::endl;
		//getchar();
		end.push_back(sum);
	}
	
	
	
	
	
	
	
}
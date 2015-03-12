#include <iostream>
#include <fstream>
#include <boost/math/distributions/normal.hpp>

int main(int argc, char * argv[])
{
    std::fstream myfile("artTestGdmData1D-02.log", std::ios_base::in);

    double a;
	double max= 0;
	double min=0;
	int i=0;
    while (myfile >> a)
    {
        printf("%f ", a);
		if(i==0){
			if(a > max){
				max=a;
			}
			if(a<min){
				min=a;
			}
			i=1;
		}
		else{
			i=0;
		}
    }
    
    std::cout << std::endl << "max : "<<max<<" min : "<<min<<std::endl;
	
	
	boost::math::normal nd(2,1); 
	for (double i=-3 ; i<3 ; i=i+0.3){
		std::cout << boost::math::pdf(nd, i) <<" and i "<< i <<std::endl;
	}
	

    return 0;
}
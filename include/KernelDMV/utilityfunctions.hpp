#ifndef UTILITYFUNCTIONSMALCOLM
#define UTILITYFUNCTIONSMALCOLM
#include <vector>


namespace kerneldmv{
	
	void normalize(const std::vector<double>& in, std::vector<double>& out){
	
		//Search for max value
		double max=0;
		for(typename std::vector<double>::const_iterator it=in.begin();it!=in.end();){
			it++;
			if(max < *it){
				max = *it;
			}
			it++;
		}
		//Search for min value
		double min =max;
		for(typename std::vector<double>::const_iterator it=in.begin();it!=in.end();){
			it++;
			if(min > *it){
				min = *it;
			}
			it++;
		}
		
		std::cout << "Max value was : "<< max << " and min value was : "<< min <<std::endl;
		
		if(max <= 1){
			std::cout << "no need to normalize, your values are already between 0 and 1"<<std::endl;
			out.reserve(in.size());	std::copy(in.begin(),in.end(),std::back_inserter(out));
		}
		else{
			//Normalized
			for(typename std::vector<double>::const_iterator it=in.begin();it!=in.end();){
				out.push_back(*it);
				it++;
				out.push_back( (*it - min) / ( max - min ) );
				it++;
			}
		}
		
		std::cout << "Max value was : "<< max << " and min value was : "<< min <<std::endl;
	}


	void sort_insertion(std::vector<double>& in){

		for (int i=0; i <in.size(); i=i+2){
			int j = i;
			while(j > 0 && in[j-2] > in[j] ) {
				double loc=in[j];
				double value=in[j+1];
				
				in[j+1]=in[j-1];
				in[j]=in[j-2];
				
				in[j-1]=value;
				in[j-2]=loc;
				
				j=j-2;
			}
		}
	}
}

#endif
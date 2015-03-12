#include "ros/ros.h"
#include "kerneldmv.hpp"
#include "kerneldmv/kerneldmv_srv.h"

bool kernel_serv(kerneldmv::kerneldmv_srv::Request  &req,
         kerneldmv::kerneldmv_srv::Response &res, Kernel_DMV& kernel)
{
	kernel.calculAll(req.data);
	
	res.mean = kernel.getMean();
	res.variance = kernel.getVariance();
	
}

int main(int argc, char **argv)
{
	//ROS declaration
	ROS_INFO("Starting Kernel DMV");
	ros::init(argc, argv, "kerneldmv");
	ros::NodeHandle my_node;
	ros::NodeHandle priv_node("~");
	
	
	//Variables
	double sigma_tmp_map;
	double sig_omega_confidence;
	double r_mean;
	double sig_variance;
	double vo_variance;
	
	priv_node.param<double>("sigma_tmp_map", sigma_tmp_map, 1);
	priv_node.param<double>("sigma_omega_confidence_map", sig_omega_confidence, 1);
	priv_node.param<double>("r_mean", r_mean, 1);
	priv_node.param<double>("sigma_variance", sig_variance, 1);
	priv_node.param<double>("variance_mean", vo_variance, 1);
	
	Kernel_DMV kernel(sigma_tmp_map, sig_omega_confidence, r_mean, sig_variance, vo_variance);
	
	//Since we don't know the calculation time and that it can be quite long, it should be an action lib 
	//NAH should be good with a service
	ros::ServiceServer service = my_node.advertiseService<kerneldmv::kerneldmv_srv::Request, kerneldmv::kerneldmv_srv::Response>("kernel_dmv", boost::bind(kernel_serv, _1, _2, kernel));
	
	
	while(ros::ok()){
		ros::spinOnce();
	}
	
}
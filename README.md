# KERNELDM+V

_Results_

Using `sigma = 1`, I have obtained

* Dataset 1 : 
    * max of mean is 0.786028  at -3.2
    * max of variance is 0.0123847 at 12.3
    
* Dataset 2 : 
	* max of mean is  0.73405 at -1.9
	* max of variance is 0.0294359 at 17 (<- which basically mean mistake)

Hope it's good.

## compile

`cmake . && make` in the main folder.

the main program is named `kerneldmv`.

read.cpp is a small standalone program to get the max and min of a dataset and to check that the normal distrubution was working fine.
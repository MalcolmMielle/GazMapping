# KERNELDM+V

This is an implementation of the KernelDMV algorithm under ROS indigo.

## Service

It's a service that takes in input data as an array of `float64`. The data is organized as position first and then measurement so that it's like `[position 1][measurement 1][position 2][measurement 2][position 3][measurement 3]...[position n][measurement n]`. It returns a message with two array of `float64`. One for the mean and one for the variance.

kerneldmv_serv.serv :

`
float64[] data
---
float64[] mean
float64[] variance
`

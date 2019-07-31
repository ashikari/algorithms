import numpy as np 
import matplotlib.pyplot as plt 




def kmeans(k, data):
	return mask

if __name__ == '__main__':
	#generate data
	num_points = int(np.round(np.random.uniform(100, 1000)))
	num_centers = int(np.round(np.random.uniform(1,5)))

	centers = []
	pt_variances = []
	for center in range(num_centers):
		x_c = np.random.uniform(-5,5)
		y_c = np.random.uniform(-2,2)
		centers.append(np.array([[x_c,y_c]]))

		xx_v = np.random.uniform(0,10)
		yy_v = np.random.uniform(0,10)
		xy_v = np.random.uniform(-1,1)
		pt_variances.append(np.array([[xx_v, xy_v],[xy_v,yy_v]]))


	pts = np.zeros((2, num_points))


	print(centers)
	print(pt_variances)
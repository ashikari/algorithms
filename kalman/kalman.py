import matplotlib.pyplot as plt
import numpy as np
import numpy.linalg as la
from scipy.linalg import expm

class kalman:
	def __init__(self, A, C =None):
		self.A = A
		self.C = C
		self.P = np.eye(A.shape[0])
		self.Q = 0.001*np.eye( A.shape[0] )
		self.R = 10
		self.I = np.eye(A.shape[0])

	def predict(self, Q = None):
		if Q != None:
			self.Q = Q
		#state prediction
		self.x_hat = np.dot(self.A,self.x_hat)
		#covariance propagation
		self.P = self.A@self.P@(self.A.T) + self.Q

	def update(self, z, C = None, R = None):
		if C != None:
			self.C = C
		if R !=None:
			self.R = R

		#innovation
		innovation = z - (self.C@self.x_hat).item()

		#calculate Kalman Gain
		S = self.R + (self.C@self.P)@self.C.T
		K = (self.P@self.C.T)*la.inv(S).item()

		#update state
		self.x_hat = self.x_hat + K*innovation

		#update Covaiance
		self.P = (self.I - K@self.C)@self.P

	def setXHat(self, x_hat):
		self.x_hat = x_hat

if __name__ == '__main__':
	
	#Generate Discrete Model
	#====================================================================
	dt = 0.001
	k = 1.0
	b = 0.4
	m = 2.0

	A_c = np.array([[0.0, 1.0],[-k/m, -b/m]])
	A = np.eye(2) + A_c*dt
	Ad = expm(A_c*dt) #just to verify the discrete model

	#damping ratio
	print("Damping Ratio: " +str( b/(2*m)*np.sqrt(k/m) ) )

	#initial state
	x = np.array([1.0, 3.0]).reshape((2,1))



	#initialize Kalman Filter
	#====================================================================
	#measurment container for history:
	Z = []
	XHat = []
	t_meas =[]

	kf = kalman(A = A, C = np.array([[1, 0]]) )
	kf.setXHat(np.array([[2], [1]]))


	#Run filter and dynamic model
	#====================================================================
	time = np.arange(0, 50, dt)
	for t in range(len(time)):
		x = np.hstack([x, A@x[:,-1].reshape((2,1))] )
		kf.predict()
		if t%100 ==0:
			z = x[0,-1] + np.random.normal(0, 0.1) #measurements
			kf.update(z)
			XHat.append(kf.x_hat[0,0])
			Z.append(z)
			t_meas.append(time[t])

	#Plot Results
	#====================================================================
	plt.plot(time, x[0,:-1], label = "Actual Trajectory", linewidth = 3) #plot the 'real' or 'modeled' Position
	plt.scatter(t_meas, Z, color = 'g', label = "Measurements") #plot the measurements (Position)
	plt.scatter(t_meas, XHat, color = 'k', label = "Filter Results") #plot the filter results

	plt.xlabel('time (s)')
	plt.ylabel('Position (m)')
	plt.title("Kalman Filter Results")
	plt.legend()
	plt.draw()
	plt.waitforbuttonpress(0)
	plt.close()
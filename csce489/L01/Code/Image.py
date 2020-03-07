""" 
Image.py is a Prokudin-Gorskii glass plate image processing library which
leverages numpy and matplot libraries to speed up image computation times.
"""
# Import required libraries
import numpy as np
from matplotlib import pyplot as plt

class Image:
	@staticmethod
	def read_strip(path):
		""""
		Function to retrieve r, g, b planes from Prokudin-Gorskii 
		glass plate images.
		"""
		image = plt.imread(path)
		height = int(image.shape[0] / 3)
		# For images with different bit depth
		scalingFactor = 255 if (np.max(image) <= 255) else 65535
		# Separating the glass image into R, G, and B channels
		b = image[: height, :] / scalingFactor
		g = image[height: 2 * height, :] / scalingFactor
		r = image[2 * height: 3 * height, :] / scalingFactor
		
		r,g,b = Image.auto_crop(r,g,b)
		
		if (r.shape[0] > 1024 or r.shape[1]> 1024):
			r = Image.image_resize(r,[1024,1024])
			g = Image.image_resize(g,[1024,1024])
			b = Image.image_resize(b,[1024,1024])
		elif(r.shape[0] > 256 or r.shape[1] > 256):
			r = Image.image_resize(r,[512,512])
			g = Image.image_resize(g,[512,512])
			b = Image.image_resize(b,[512,512])
		else:
			r = Image.image_resize(r,[128,128])
			g = Image.image_resize(g,[128,128])
			b = Image.image_resize(b,[128,128])		
		return r,g,b
	@staticmethod
	def compositeChannels(r, g, b, axis):
		return np.stack((r, g, b), axis = axis)
	@staticmethod
	def writeTo(path,image,**kwargs):
		op = kwargs.get('op', None)
		if (op == 'grayscale'):
			plt.imsave(path, image, cmap='gray')
		else:
			plt.imsave(path, image)

	@staticmethod
	def circ_shift(channel, shift):
		"""
		circ_shift is a function to shift matrix columns and rows 
		by desired shift values. Is circular in nature so values 
		on the ends will roll over to opposite ends instead of being
		null.
		"""
		shifted = np.roll(channel, shift[0], axis = 0)
		shifted = np.roll(shifted, shift[1], axis = 1)
		return shifted
	@staticmethod
	def find_shift(channel1, channel2):
		"""
		im1 and im2 are both different color channels of a BGR image,
		shift is computed using sum of squared difference as the error
		approach to find the relative alignment offset for given image 
		channels. Exhaustive search range for computing aligment are 
		computed staring at 40 pixels for a 128 by 128 image.
		"""

		first = Image.compute_gradient(channel1)[0]
		second = Image.compute_gradient(channel2)[0]
		x_domain = int(20 * first.shape[1]/128)
		y_domain = int(20 * first.shape[0]/128)
		
		ERR = np.Infinity
		indices = []
		for offset_row in range(-y_domain , y_domain):
			for offset_col in range(-x_domain , x_domain):
				shifted = Image.circ_shift(first,[offset_row,offset_col])
				clip1 = shifted[y_domain:-y_domain,x_domain:-x_domain]
				clip2 = second[y_domain:-y_domain,x_domain:-x_domain]
				SSD = np.sum((clip2 - clip1)**2)
				if (SSD <= ERR):
					ERR = SSD
					indices = [offset_row,offset_col] 
		print(indices)
		return indices 
	

	@staticmethod
	def gausian_pyramid(channel,levels=4):
		"""
		resize function uses the guassian pyramid paridigm to reduce the dimensionality
		of high-resolution glass plate images.
		channel = image given by color channel 
		levels = how many times to half the image size
		"""
		branch = channel.copy()
		newSize = branch.shape
	

		pyramid = np.zeros((levels,  branch.shape[0], branch.shape[1]))
		sizes = np.zeros((levels,2))
		pyramid[0,:,:] = branch 
		sizes[0,:] = np.array(newSize).astype(int)

		
		
		for level in range(1,levels):

			newSize = np.divide(newSize,2).astype(int)
			branch = Image.apply_gausian_filter(branch)
			branch = Image.image_resize(branch,newSize)
			
			pyramid[level,:newSize[0],:newSize[1]] = branch 
			sizes[level,:] = newSize
			
		return pyramid,sizes.astype(int)
			
	@staticmethod
	def image_resize(channel,newSize):
		"""
		resize with naive nearest neighebor interpolation
		"""
		size = channel.shape
		scaleY, scaleX = np.array(newSize)/np.array(size)
		row_interpolate =(np.ceil(
		range(1,1 + int(size[0]*scaleY) )
		/scaleY)-1).astype(int)

		col_interpolate =(np.ceil(
		range(1,1 + int(size[1]*scaleX) )
		/scaleX)-1).astype(int)

		img = np.empty([row_interpolate.shape[0],col_interpolate.shape[0]])
		for i in range(img.shape[0]):
			for j in range(img.shape[1]):
				img[i,j] = channel[row_interpolate[i],col_interpolate[j]]

		return img

	@staticmethod
	def compute_gradient(channel):
		channel = Image.apply_gausian_filter(channel)
		#print(channel.shape)
		image_X,image_Y = Image.apply_sobel_filter(channel)
		gradientX = np.hypot(image_X, 0)
		gradientY = np.hypot(0, image_Y)
		gradient = np.sqrt((gradientX**2)+(gradientY**2))
		direction = np.arctan2(image_Y, image_X)
		return gradient,direction

	@staticmethod
	def auto_crop(r,g,b):
		y_start,x_start,y_end,x_end = Image.compute_bounds([r,g,b])
		newR = r[y_start:y_end,x_start:x_end]
		newG = g[y_start:y_end,x_start:x_end]
		newB = b[y_start:y_end,x_start:x_end]
		return newR, newG, newB
	@staticmethod
	def compute_bounds(channels):

		boundsX_start = np.Infinity
		boundsY_start = np.Infinity
		boundsX_end = -np.Infinity
		boundsY_end = -np.Infinity

		for channel in channels:
			gradient = Image.compute_gradient(channel)[0]
			sum_cols = np.sum(gradient,axis = 0)
			sum_rows = np.sum(gradient,axis = 1)
			avg_rows = np.average(sum_rows)
			avg_cols = np.average(sum_cols)
			max_col = 3 * avg_cols
			max_row = 3 * avg_rows
			cols_to_remove = np.where(sum_cols > max_col)[0]
			rows_to_remove = np.where(sum_rows > max_row)[0]
			diffX = np.diff(cols_to_remove)
			diffY = np.diff(rows_to_remove)
			"""compute bounds to keep"""
			xPos = 0
			yPos = 0
			startX = 0
			startY = 0
			endX = -1
			endY = -1
			if (diffY.size > 0):
				yPos = np.argmax(diffY)
				startY = rows_to_remove[yPos] + 1
				endY = np.max(diffY)
				
			if (diffX.size > 0):
				xPos = np.argmax(diffX)
				startX = cols_to_remove[xPos] + 1
				endX = np.max(diffX)

			
			if((endY > boundsY_end or endY == -1) and boundsY_end != -1) :
				boundsY_end = endY
			if ((endX > boundsX_end or endX == -1 )and boundsX_end != -1):
				boundsX_end = endX
			if(startY < boundsY_start):
				boundsY_start = startY
			if(startX < boundsX_start):
				boundsX_start = startX
		return boundsY_start,boundsX_start,boundsY_end,boundsX_end

	@staticmethod
	def apply_gausian_filter(channel):
		img = channel.copy()
		kernel = Image.gaussian_kernel(img)
		img = Image.convolve(img, kernel)
		return img
	@staticmethod
	def apply_sobel_filter(channel):
		X_kernel = np.array([[-1,0,1],[-2,0,2],[-1,0,1]],np.float32)
		Y_kernel = np.array([[1,2,1],[0,0,0],[-1,-2,-1]],np.float32)
		image_X = Image.convolve(channel, X_kernel)
		image_Y = Image.convolve(channel, Y_kernel)
		return image_X, image_Y
	@staticmethod
	def gaussian_kernel(channel):
		"""
		gaussian_kernel function creates a kernel using gaussian filter
		method folowing general rule of thumb of setting half the kernel
		width to be 3 * sigma, where sigma is defined as the standard 
		deviation of channel.
		capped at kernel of 5*5
		"""
		
		sigma = np.std(channel)
		if sigma > 0.85:
			sigma =0.85 
		filtersize = (2 * int(3 * sigma + 0.5)) + 1
		half = filtersize //2
		u, v = np.mgrid[-half:half+1, -half:half+1]
		normal = 1 / (2.0 * np.pi * sigma**2)
		kernel = np.exp(-((u**2 + v**2)/(2 * sigma**2))) * normal
		kernel = (1/np.sum(kernel)) * kernel
		return kernel

	@staticmethod
	def convolve(channel, kernel):
		"""
		fast and efficent approach to convolve channel with a given kernel.
		implmentation provided at 
		https://stackoverflow.com/questions/19414673/in-numpy-how-to-efficiently-list-all-fixed-size-submatrices
		"""

		s = kernel.shape + tuple(np.subtract(channel.shape, kernel.shape) + 1)	
		strd = np.lib.stride_tricks.as_strided
		subM = strd(channel, shape = s, strides = channel.strides * 2)
		conv2d = np.einsum('ij,ijkl->kl', kernel, subM)
		return conv2d


import numpy as np
from matplotlib import pyplot as plt
from numpy.lib.stride_tricks import as_strided

class Image:
    def apply_Gaussian(img):
        gkernel = Image.kernel(sigma = 1, mode = "gaussian")
        gImg = Image.conv2d(img,gkernel)
        return gImg
    def apply_Laplace(img):
        lKernel = Image.kernel(sigma = 1, mode = "laplacian")
        lImg = Image.conv2d(img,lKernel)
        #normalizedImg = Image.minmaxNormalize(lImg)
        return lImg
    def blend(source, mask, target):
        Gm = im.apply_Gaussian(mask)
        Ls = im.apply_Laplace(source)
        Lt = im.apply_Laplace(target)
        return Ls * Gm + Lt * (1 - Gm)
    def collapse(iPyramid):
        pass
    def interpolate(img,mode):
        """
        scale img up by factor of 2
        """
        row,col,depth = img.shape
        if (mode == 'NEAREST'):  
            channel_r = img[:,:,0]
            rs,cs = channel_r.strides
            r = as_strided(channel_r, (row, 2, col, 2), (rs, 0, cs, 0))

            channel_g = img[:,:,1]
            rs,cs = channel_g.strides
            g = as_strided(channel_g, (row, 2, col, 2), (rs, 0, cs, 0))
        
            channel_b = img[:,:,2]
            rs,cs = channel_b.strides
            b = as_strided(channel_b, (row, 2, col, 2), (rs, 0, cs, 0))  

            return np.stack((r, g, b), axis = 2)
        elif(mode == "BILINEAR"):
            arr = np.zeros((row*2, col*2,depth))
            pass
    def decimate(img):
        """
        scale down img by factor of 2
        by taking every other row and column
        """
        coarse = img[::2,::2,:]
        return coarse
    def minmaxNormalize(img):
        rMax, rMin =  np.amax(img[:,:,0]),  np.amin(img[:,:,0])
        r = (img[:,:,0] - rMin)/(rMax - rMin)
        gMax, gMin =  np.amax(img[:,:,1]),  np.amin(img[:,:,1])
        g = (img[:,:,1] - gMin)/(gMax - gMin)
        bMax, bMin =  np.amax(img[:,:,2]),  np.amin(img[:,:,2])
        b = (img[:,:,2] - bMin)/(bMax - bMin)                
        return np.stack((r, g, b), axis = 2)
    def conv2d(img,kernel):
        r = Image.convChannel(img[:,:,0], kernel)
        g = Image.convChannel(img[:,:,1], kernel)
        b = Image.convChannel(img[:,:,2], kernel)
        return np.stack((r, g, b), axis = 2)
    def convChannel(channel, kernel):
        cHeight,cWidth = channel.shape
        kWidth = kernel.shape[0] #kernel is a square matrix of odd dimensions
        #pad img channel with zeros to support kernel operations on edges
        pad = kWidth//2
        img = np.pad(channel, ((pad, pad), (pad, pad)), 'constant')
        conv2d = np.zeros_like(channel)
        iH,iW =img.shape
        k_flattened = kernel.flatten()
        for i in range(pad,cHeight):
            for j in range(pad,cWidth):
                block = img[i-pad:i+pad+1, j-pad:j+pad+1]
                conv2d[i-pad,j-pad] = np.sum(np.multiply(block.flatten(), k_flattened))
        return conv2d    

    def kernel(sigma = 0.8, mode = "gaussian"):
        filtersize = (2 * int(3 * sigma + 0.5)) + 1
        if(mode ==  "gaussian"):
            gaussianKernel = Image.gaussian(sigma,filtersize)
            return gaussianKernel
        elif(mode == "laplacian"):
            laplacianKernel = Image.log(sigma,filtersize)
            return laplacianKernel
        else:
            print ("invalid mode specified")
            return None       

    def log(sigma,kernelDim):
        half = kernelDim // 2
        x,y = np.mgrid[-half:half+1, -half:half+1]
        s2 = sigma * sigma
        s4 = s2 * s2
        x2 = np.square(x)
        y2 = np.square(y)  
        delta = (x2 + y2 - 2*s2) / s4
        kernel =  np.exp((-x2 - y2) / (2 * s2) )
        kernel *=  delta
        return kernel

    def gaussian(sigma,kernelDim):
        half = kernelDim //2
        x,y = np.mgrid[-half:half+1, -half:half+1]
        s2 = sigma * sigma
        x2 = np.square(x)
        y2 = np.square(y)        
        kernel =  np.exp((-x2 - y2) / (2 * s2) )
        normal = (1 / (2*np.pi * s2))
        # normalize the kerel such that is sums to 1
        kernel *= normal 
        return kernel
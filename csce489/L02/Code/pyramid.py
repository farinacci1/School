
import numpy as np
from matplotlib import pyplot as plt
from numpy.lib.stride_tricks import as_strided

class Image:

    def blend(source, mask, target):
        blended = []
        sPyr = Image.laplPyramid(Image.gausPyramid(source, 4))
        mPyr = Image.gausPyramid(mask, 4)
        tPyr = Image.laplPyramid(Image.gausPyramid(target, 4))
        lvls = len(mPyr)
        for i in range(0,lvls):
            img = sPyr[i] * mPyr[i] + tPyr[i] * (1 - mPyr[i])        
            blended.append(img)
        return blended
    def collapse(blPyr):
        lvl = len(blPyr) - 1
        while(lvl > 0 ):
            upscale = Image.interpolate(blPyr[lvl],'NEAREST')
            compare = blPyr[lvl - 1]
            if upscale.shape[0] > compare.shape[0]:
                upscale = np.delete(upscale,(-1),axis = 0)
            if upscale.shape[1] > compare.shape[1]:
                upscale = np.delete(upscale,(-1),axis = 1)                
            tmp = np.add(upscale,compare )
            blPyr.pop()
            blPyr.pop()
            blPyr.append(tmp)
            output = tmp
            lvl = lvl -1
        return output
    def minmaxNormalize(img):
        rMax, rMin =  np.amax(img[:,:,0]),  np.amin(img[:,:,0])
        r = (img[:,:,0] - rMin)/(rMax - rMin)
        gMax, gMin =  np.amax(img[:,:,1]),  np.amin(img[:,:,1])
        g = (img[:,:,1] - gMin)/(gMax - gMin)
        bMax, bMin =  np.amax(img[:,:,2]),  np.amin(img[:,:,2])
        b = (img[:,:,2] - bMin)/(bMax - bMin)                
        return np.stack((r, g, b), axis = 2)
    def apply_Gaussian(img):
        gkernel = Image.kernel(sigma = 1.2, mode = "gaussian")
        gImg = Image.conv2d(img,gkernel)
        return gImg
    """
    def apply_Laplace(img):
        #lKernel = np.asarray([[0,-1,0],[-1,4,-1],[0,-1,0]]
        lKernel = Image.kernel(sigma = 1.2, mode = "laplacian")
        lImg = Image.conv2d(img,lKernel)
        return lImg   
    """
    def gausPyramid(img, lvls):
        output = [img]
        for i in range(0,lvls):
            tmp = Image.decimate(output[len(output)-1])
            output.append(tmp)
        return output
    def laplPyramid(gausPyr):
        """
        laplace pyramid built using difference of gaussian method
        """
        output = []
        lvls = len(gausPyr)
        for i in range(0,lvls-1):
            first = gausPyr[i]
            second = Image.interpolate(gausPyr[i+1],'NEAREST')
            if(second.shape[0] > first.shape[0]):
                second = np.delete(second,(-1),axis=0)
            if(second.shape[1] > first.shape[1]):
                second = np.delete(second,(-1),axis=1)                
            tmp = first - second
            output.append(tmp)
        output.append(gausPyr.pop())
        return output

    def interpolate(img,mode):
        """
        scale img up by factor of 2
        """
        rows,cols,depth = img.shape
        #print(rows,cols,depth)
        if (mode == 'NEAREST'):  
            upsample = np.zeros((rows * 2 , cols * 2,depth))
            upsample[::2,::2,:] = img
            out = 4*Image.apply_Gaussian(upsample)
            return out
    def decimate(img):
        """
        scale down img by factor of 2
        by taking every other row and column
        """
        filtered = Image.apply_Gaussian(img)
        sub_sample = filtered[::2,::2,:]
        return sub_sample
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
        
        #elif(mode == "laplacian"):
        #    laplacianKernel = Image.log(sigma,filtersize)
        #    return laplacianKernel
        
        else:
            print ("invalid mode specified")
            return None       
    """
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
    """
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
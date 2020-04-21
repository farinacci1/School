""" Assignment 6 - Starter code


"""
import numpy as np
import matplotlib.pyplot as plt
import os
from gsolve import gsolve as gs
import cv2
import random
import math
def Read(path = ""):
    source = (plt.imread(path) *255).astype(int)
    return source
# Based on code by James Tompkin
#
# reads in a directory and parses out the exposure values
# files should be named like: "xxx_yyy.jpg" where
# xxx / yyy is the exposure in seconds. 
def ParseFiles(calibSetName, dir):
    imageNames = os.listdir(os.path.join(dir, calibSetName))
    
    filePaths = []
    exposures = []
    
    for imageName in imageNames:
        exposure = imageName.split('.')[0].split('_')
        exposures.append(int(exposure[0]) / int(exposure[1]))
        filePaths.append(os.path.join(dir, calibSetName, imageName))
    
    # sort by exposure
    sortedIndices = np.argsort(exposures)[::-1]
    filePaths = [filePaths[i] for i in sortedIndices]
    exposures = [exposures[i] for i in sortedIndices]
    
    return filePaths, exposures

def sampler(images,px_idx):
    """
    images = p number of single channel images
    N = number of pixels to sample
    """
    numImages = images.shape[2]
    samples = np.zeros((len(px_idx),numImages),dtype = np.uint8)
    for idx in px_idx:
        for j in range(numImages):
            flat = images[...,j].flatten()
            samples[i,j] = flat[idx]
    return samples
def getHDR(images,G,B,W):
    rows,cols,depth = images.shape
    HDR = np.zeros((rows,cols),dtype=np.float64)
    W = np.asarray(W)
    for i in range(rows):
        for j in range(cols):   
            g = np.array([G[images[i,j,k]] for k in range(depth)])
            w = np.array([W[images[i,j,k]] for k in range(depth)])   
            sum_w = np.sum(w)
            E = g - B
            if(sum_w != 0):
                HDR[i,j] = np.sum(w * E) / sum_w
            else:
                HDR[i,j] = G[images[i,j,depth//2]] - B[depth//2]
    #HDR = np.exp(HDR)
    return HDR
def globalToneMapper(HDR, gamma =0.4):
    HDR = cv2.normalize(HDR, None, 0, 1, cv2.NORM_MINMAX)
    T = HDR **gamma
    return np.clip(T, 0.0, 1.0)
def reinhardToneMapper(HDR,gamma =0.4,alpha=0.25):
    """
    implementation of reinhard global tonemapper
    """
    rows,cols,depth =HDR.shape 
    hdr = cv2.normalize(HDR, None, 0, 1, cv2.NORM_MINMAX)
    hdr = hdr**gamma
    Luminance = cv2.cvtColor(hdr.astype('float32'), cv2.COLOR_BGR2GRAY)
    L_avg = np.exp(np.mean(np.log(Luminance)))
    T = alpha / L_avg * Luminance
    L_tone = T * (1 + (T / (T.max())**2)) / (1 + T)
    M = L_tone / Luminance
    for ch in range(depth):
        hdr[...,ch] = hdr[...,ch] * M
    return np.clip(hdr, 0.0, 1.0)
def localToneMapper(RGB,gamma = 0.5,dR =4):
    rows,cols,depth = RGB.shape
    Intensity = np.empty((rows,cols))
    Chrominance = np.empty((rows,cols,depth))
    RGB = cv2.normalize(RGB, None, 0, 1, cv2.NORM_MINMAX)
    minNonZero = np.min(RGB[RGB != 0])
    RGB[RGB == 0] = minNonZero *.01
    for row in range(rows):
        for col in range(cols):
            Intensity[row,col] = np.mean([RGB[row,col,0],RGB[row,col,1],RGB[row,col,2]])
    for ch in range(depth):
        Chrominance[...,ch] = RGB[...,ch] / Intensity
    L =  np.log(Intensity)
    B = cv2.GaussianBlur(L, (5, 5), 0)
    O = np.max(B)
    D = L - B
    S = dR / (np.max(B) - np.min(B))
    B1 = (B - O) * S
    O1 = 2 **(B1 + D) 
    for ch in range(depth):
        Chrominance[...,ch] = Chrominance[...,ch] * O1
    Chrominance = Chrominance**gamma
    return np.clip(Chrominance, 0.0, 1.0)
    
if __name__ == '__main__':
    # Setting up the input output paths and the parameters
    inputDir = '../Images/'
    outputDir = '../Results/'

    _lambda = 50

    calibSetName = 'Office'

    # Parsing the input images to get the file names and corresponding exposure
    # values
    filePaths, exposures = ParseFiles(calibSetName, inputDir)
    subdir = filePaths[0].split('/')[2]

    numImages = len(filePaths)
    imageArrays = []
    for i in range(0, numImages):
        img = Read(filePaths[i])
        if i == 0:
            imageArrays = np.asarray(img)
        else:
            imageArrays = np.dstack((imageArrays, img))

    """ Task 1 """
    rows,cols,depth = imageArrays.shape
    numChannels = depth // numImages
    HDR = np.zeros((rows,cols,numChannels), 'float32')
    ## Sample the images
    N = (5 * 256)//(numImages - 1)
    dims = imageArrays[0].shape
    px_idx = random.sample(range(0, dims[0]*dims[1]), N)
    B = [math.log(e) for e in exposures]
    w = [z if z <= 0.5 * 255 else 255 - z for z in range(256)]
    for ch in range(numChannels):
        images = imageArrays[:,:,ch::3]
        samples = sampler(images,px_idx)
        g, lE = gs(samples, B, _lambda, w)
        plt.plot(g, np.arange(0,256,1))
        """ Task 2 """
        HDR[..., ch] = getHDR(images,g,B,w)
    plt.ylabel('pixel value Z')
    plt.xlabel('log exposure X') 
    plt.savefig(outputDir + calibSetName+'_CRF.png')   
    #cv2.imshow('image',HDR)
    #cv2.waitKey(0)


    """ Task 3 """
    # Perform both local and global tone-mapping
    globalRein = reinhardToneMapper(HDR,0.4)
    plt.imsave(outputDir+calibSetName + "_reinhard.jpg",globalRein)
    globalIm = globalToneMapper(HDR,0.4)
    plt.imsave(outputDir+calibSetName + "_Global.jpg",globalIm)
    localIm =localToneMapper(HDR,0.4)
    plt.imsave(outputDir+calibSetName + "_Local.jpg",localIm)
    


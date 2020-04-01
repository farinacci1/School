""" Assignment 5 - Starter code


""" 

# Import required libraries
import matplotlib.pyplot as plt
import cv2
import numpy as np
import os

# Read source and mask (if exists) for a given id
def Read(id, path = ""):
    source = plt.imread(path + "image_" + id + ".jpg") / 255
    maskPath = path + "mask_" + id + ".jpg"
    
    if os.path.isfile(maskPath):
        mask = plt.imread(maskPath)
        assert(mask.shape == source.shape), 'size of mask and image does not match'
        mask = (mask > 128)[:, :, 0].astype(int)
    else:
        mask = np.zeros_like(source)[:, :, 0].astype(int)

    return source, mask

def SeamCarve(input, widthFac, heightFac, mask):

    # Main seam carving function. This is done in three main parts: 1)
    # computing the energy function, 2) finding optimal seam, and 3) removing
    # the seam. The three parts are repeated until the desired size is reached.
    assert (widthFac == 1 or heightFac == 1), 'Changing both width and height is not supported!'
    assert (widthFac <= 1 and heightFac <= 1), 'Increasing the size is not supported!'
    img = input
    gray_img = cv2.cvtColor(img.astype('float32'), cv2.COLOR_BGR2GRAY)
    curr_height,curr_width = gray_img.shape
    new_height = int(curr_height * heightFac)
    new_width = int(curr_width*widthFac)
    
    
    
    dims = img.shape
    dc = abs(new_width - curr_width)
    dr = abs(new_height - curr_height)

    indX = -1
    indY = -1
    for i in range(dc):
        M = computeMinimalEnergy(computeEnergyMatrix(gray_img))
        img = removeMinSeam(img,M)
        gray_img = cv2.cvtColor(img.astype('float32'), cv2.COLOR_BGR2GRAY)

    return img, [img.shape[0],img.shape[1]]        
        



def computeEnergyMatrix(img):
    dx = cv2.Sobel(img,cv2.CV_64F,1,0,ksize=5)
    dy = cv2.Sobel(img,cv2.CV_64F,0,1,ksize=5)
    return abs(dx) + abs(dy)

def computeMinimalEnergy(E):
    num_rows,num_cols = E.shape
    mat = np.full((num_rows, num_cols + 2), np.inf)
    mat[0,1:num_cols+1] = E[0,:] 
    for i in range(1,num_rows):
        for j in range(1,num_cols):
            mat[i,j] = E[i,j-1] + min(mat[i-1,j-1],mat[i-1,j],mat[i-1,j+1])
    return mat[:,1:-1]
def removeMinSeam(img,M):
    dimY,dimX = M.shape
    print(M.shape,img.shape)
    indY = -1
    output = np.zeros((dimY,dimX-1,3))
    #get minimum on last row
    minIndex = np.where(M[indY,:] == min(M[indY,:]))[0][0]
    if(minIndex > 0 and minIndex < dimX-1): 
        output[indY,:,:] =np.concatenate((img[indY,:minIndex,:],img[indY,minIndex+1:,:]),axis = 0)
    elif(minIndex == dimX - 1):
        output[indY,:,:] = img[indY:,:-1,:]#exclude last column
    elif(minIndex == 0):
        output[indY,:,:] = img[indY:,1:,:]#exlude first column

    while dimY + indY != 1:
        indY -= 1
        minPos = 0
        if minIndex != 0:
            if min(M[indY,minIndex - 1],M[indY,minIndex]) == M[indY,minIndex - 1] :
                minPos = -1
            if minIndex < dimX - 1:
                if min(M[indY,minIndex + minPos],M[indY,minIndex + 1]) == M[indY,minIndex +1]:
                    minPos = 1
        elif minIndex == 0:
            if(minIndex < dimX - 1):
                if min(M[indY,minIndex],M[indY,minIndex + 1]) == M[indY,minIndex + 1]:
                    minPos = 1
        minIndex += minPos
        if(minIndex > 0 and minIndex < dimX-1): 
            output[indY,:,:] = np.concatenate((img[indY,:minIndex,:],img[indY,minIndex+1:,:]),axis = 0)#exclude min index col
        elif(minIndex == dimX - 1):
            output[indY,:,:] = img[indY,:-1,:]#exclude last column
        elif(minIndex == 0):
            output[indY,:,:] = img[indY,1:,:]#exlude first column
    return output   

# Setting up the input output paths
inputDir = '../Images/'
outputDir = '../Results/'

widthFac = 0.5; # To reduce the width, set this parameter to a value less than 1
heightFac = 1;  # To reduce the height, set this parameter to a value less than 1
N = 4 # number of images

for index in range(1, N + 1):

    input, mask = Read(str(index).zfill(2), inputDir)

    # Performing seam carving. This is the part that you have to implement.
    output, size = SeamCarve(input, widthFac, heightFac, mask)

    # Writing the result
    plt.imsave("{}/result_{}_{}x{}.jpg".format(outputDir, 
                                            str(index).zfill(2), 
                                            str(size[0]).zfill(2), 
                                            str(size[1]).zfill(2)), output)
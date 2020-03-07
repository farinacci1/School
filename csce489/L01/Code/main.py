""" 
Assignment 1
""" 
import numpy as np
from Image import Image as im
if __name__ == '__main__':
    # Setting the input output file path
    imageDir = '../Images/'
    imageName = 'harvesters.tif'
    outDir = '../Results/'    
    # Get r, g, b channels from image strip
    r, g, b = im.read_strip(imageDir + imageName)

    '''task1'''
    '''
    rShift = im.find_shift(r, b)
    gShift = im.find_shift(g,b)
    finalB = b
    finalG = im.circ_shift(g, gShift)
    finalR = im.circ_shift(r, rShift)
    # Putting together the aligned channels to form the color image
    finalImage = im.compositeChannels(finalR, finalG, finalB,2)
    # Writing the image to the Results folder
    im.writeTo(outDir + imageName[:-4] + '.jpg', finalImage)
    '''

    '''task2'''
    
    #pyramid is padded so all levels have same size
    r_pyramid,r_sizes = im.gausian_pyramid(r,4)
    g_pyramid,g_sizes = im.gausian_pyramid(g,4)
    b_pyramid,b_sizes = im.gausian_pyramid(b,4)
    #compute lowest level of pyramid
    width = r_sizes[-1][0]
    unpad_red = r_pyramid[-1,:width,:width]
    unpad_green = g_pyramid[-1,:width,:width]
    unpad_blue = b_pyramid[-1,:width,:width]
    rShift = im.find_shift(unpad_red,unpad_blue)
    gShift = im.find_shift(unpad_green,unpad_blue)
    rShift = np.multiply(rShift,4)
    gShift = np.multiply(gShift,4)


    width = r_sizes[1][0]
    finalB = b_pyramid[1,:width,:width+1]
    finalG = im.circ_shift(g_pyramid[0,:width,:width+1], gShift)
    finalR = im.circ_shift(r_pyramid[0,:width,:width+1], rShift)
    finalImage = im.compositeChannels(finalR, finalG, finalB,2)
    # Putting together the aligned channels to form the color image
    # Writing the image to the Results folder
    im.writeTo(outDir + imageName[:-4] + '.jpg', finalImage)  
    




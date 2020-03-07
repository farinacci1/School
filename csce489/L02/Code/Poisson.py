import numpy as np
import scipy.sparse
from scipy.sparse.linalg import spsolve
class Image:
    def sparse_lapl(rows,cols):
        d = scipy.sparse.lil_matrix((cols,cols))
        d.setdiag(1,-1)
        d.setdiag(-4)
        d.setdiag(1,1)
        A = scipy.sparse.block_diag([d]*rows).tolil()
        A.setdiag(1,1*cols)
        A.setdiag(1,-1*cols)
        return A
    def poisson(source,target,mask,isMix):
        mask_layer = mask[:,:,0]
        rows,cols = mask_layer.shape
        A = Image.sparse_lapl(rows,cols)
        lap = A.tocsc()
        for row in range(1,rows - 1):
            for col in range(1,cols - 1):
                if mask_layer[row, col] == 0:
                    index = col + row * cols
                    A[index,index] = 1
                    A[index,index + 1] = 0
                    A[index,index - 1] = 0
                    A[index,index + cols] = 0
                    A[index,index - cols] = 0
        A = A.tocsc()
        mf = mask_layer.flatten()
        for channel in range(source.shape[2]):
            sf = source[:,:,channel].flatten()
            tf = target[:,:,channel].flatten()
            B = lap.dot(sf)
            B[mf == 0] = tf[mf == 0]
            x = spsolve(A, B)
            x =  Image.minmaxNormalize(x.reshape((rows, cols)))
            target[:, :, channel] = x
        return target

    def minmaxNormalize(channel):
        cMax, cMin =  np.amax(channel),  np.amin(channel)
        normalized = (channel - cMin)/(cMax - cMin)    
        return normalized     
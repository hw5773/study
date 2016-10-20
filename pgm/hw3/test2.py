import numpy as np
from etc import gaussian

def pdf_multivariate_gauss(x, mu, cov):
    '''
    Caculate the multivariate normal density (pdf)

    Keyword arguments:
        x = numpy array of a "d x 1" sample vector
        mu = numpy array of a "d x 1" mean vector
        cov = "numpy array of a d x d" covariance matrix
    '''
    assert(mu.shape[0] > mu.shape[1]), 'mu must be a row vector'
    assert(x.shape[0] > x.shape[1]), 'x must be a row vector'
    assert(cov.shape[0] == cov.shape[1]), 'covariance matrix must be square'
    assert(mu.shape[0] == cov.shape[0]), 'cov_mat and mu_vec must have the same dimensions'
    assert(mu.shape[0] == x.shape[0]), 'mu and x must have the same dimensions'
    part1 = 1 / ( ((2* np.pi)**(len(mu)/2)) * (np.linalg.det(cov)**(1/2)) )
    part2 = (-1/2) * ((x-mu).T.dot(np.linalg.inv(cov))).dot((x-mu))
    return float(part1 * np.exp(part2))

def test_gauss_pdf():
    x = np.array([[0.91],[0.93]])
    mu  = np.array([[0.55317401],[0.57603249]])
    cov = np.matrix([[0.15677495, 0.15768565], [0.15768565, 0.15865947]]) 

    print "1: ", pdf_multivariate_gauss(x, mu, cov)
#	print "2: ", gaussian(x, mu, cov)

if __name__ == '__main__':
    test_gauss_pdf()

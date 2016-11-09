import scipy.io as sio

mat_contents = sio.loadmat("hmm_params.mat")

prior = mat_contents['prior']
transition = mat_contents['transition']
emission = mat_contents['emission']
observation = mat_contents['price_change'][0]

num_of_states = len(prior)
time = 100

# Homomorphic-Unsharp-Masking-in-C

Homogeneous Unsharp Masking (HUM) can be used to estimate the bias field in MR images. Considering Bias Field as a
low-frequency component, the HUM can be written as: u(i) = (v(i)C(N))/LPF(v(i)). Here, u(i) is bias field free image, LPF(.) denotes the low-pass filter, C(N) is the normalizing constant that depends on the low-pass filter - it means if the low-pass filter is an contra- harmonic mean filter,then the constant C(N ) is the  to preserve the average intensity of the image.

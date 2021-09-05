import h5py
fname='M33_1_MultiBeamOTF-M01_F-sub_baseline-doppler.hdf5'#19个beams,横扫加竖扫共38个文件
tdata= h5py.File(fname,'r')
print(list(
ra= tdata['ra'][()]tdata.keys()))
dec= tdata['dec'][()]
Ta= tdata['Ta'][()]
vlsr= tdata['vel'][()]
print(ra)
print(dec)
print(Ta)
print(vlsr)
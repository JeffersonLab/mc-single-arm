Program to calculate the inclusive DIS cross section using the same RC table and interpolation functions as used in mc_single_arm.

User provides angle at which the evaluate cross section, Eprime bins (minimum Eprime in GeV, step size, and number of bins).  The user-provided input filename should be the same mc input files used for the relevant setting - the only information taken from this file is the radaitive corrections file name. However, this ensures that the vertex cross section is evaulated using the same table as the simulated data/ 

Usage:

./calc_dis_xsec
 Enter theta (deg.),  minimum Eprime (GeV),Eprime-step, # of Eprime bins
29.0,1.51,0.01,10
 Enter input filename (assumed to be in infiles dir)
hms_29p05deg_1p531gev_deut_rsidis

Program output:

 ../../infiles/hms_29p05deg_1p531gev_deut_rsidis.inp                             opened
      8565.0      ebeam in MeV
      381        number of momentum bins in RC table
      128         number of theta bins in RC table
      rsidis_4pass_d2cryo18_hms.out    RC table name
 Radiative corrections file in input file
 opening ../../rctables/rsidis_4pass_d2cryo18_hms.out                                    
 number of momentum bins - RAD1         381
 number of theta bins - RAD1         128
 Eprime   theta     xBj      Q2       W         Cross section (ub/GeV/sr)
 1.510    29.000    0.428    1.081    1.524    0.38484E-02
 1.520    29.000    0.434    1.088    1.516    0.38180E-02
 1.530    29.000    0.441    1.095    1.507    0.37897E-02
 1.540    29.000    0.447    1.103    1.498    0.37603E-02
 1.550    29.000    0.453    1.110    1.490    0.37322E-02
 1.560    29.000    0.460    1.117    1.481    0.37025E-02
 1.570    29.000    0.466    1.124    1.472    0.36757E-02
 1.580    29.000    0.473    1.131    1.463    0.36463E-02
 1.590    29.000    0.480    1.138    1.455    0.36202E-02
 1.600    29.000    0.486    1.145    1.446    0.35930E-02

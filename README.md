mc_single_arm
==============
Update: May 15, 2024

Removed cernlib dependency for easir RHEL9/Alma9 compatibility.  Output is now fortran binary file.  Separate applications in "util" directory can make either ntuples or root trees. These applications must be compiled separately. Scripts in top directory will generate fortran binary file, run appropriate application to create ntuple/tree, and delete binary file (run_mc_single_arm_ntup and run_mc_single_arm_tree).

***********

Hall C single arm Monte Carlo for HMS and SHMS 

To compile type "make" in the src directory.
NOTE: Make sure paths to Cern libararies are defined.
On JLab machines, do "setup cernlib/2005".

Main code is mc_single_arm.f

If include files are changed it is best to do "make Clean" and then "make"
since the Makefile is not smart enough to look for dependency

Running code
------------

Easiest way is to use "run_mc_single_arm <filename>"

Alternately:
cd src
mc_single_arm 
(ask for input file name (assumed in infiles subdirectory))

* Input file : infile_name.inp
* Output file is at outfiles/infile_name.out 
* The hbook file is at worksim/infile_name.rzdat
* Hard coded flags in mc_single_arm.f 
** hut_ntuple : Write out ntuple (1 for HMS and 1411 for SHMS) described below. (true)
** spec_ntuple: Write out ntuple 1412 described below (false)
** decay_flag :	Allow decay of particle(false)
* Hard coded flags in shms/mc_shms.f 
** use_coll  : pass particles through the collimator between HB and Q1 (true)

Code flow
--------- 
* Read in the input file
* Starts event loop
* The seed for the random number generator is picked by the time. each run of the code will be different set of random numbers.
* The lab or beam coordinate system has +z along the target towards the beam dump, +y is vertical pointing down and +x is pointing beam left.
* Randomly selects beam-target interaction point.
* The coordinate system of the SHMS is +z_s along the central ray, +x_s is vertical pointing down and +y_s is horizontal point to large angle. 
* Randomly selects particle delta=(p-pcent)/p, dy_s/dz_s amd dx_s/dz_s.
* Drifts to z_s=0 plane
* Calculated multiple scattering the target using the input radiation length
** If target length greater than 3cm assume LH2 cryotarget for multiple scattering calculation.
**  If target length less than 3cm assume simple solid target for multiple scattering calculation.
* calulated mutliple scattering in scattering chamber window, air and spectrometer entrance window.
* Call the mc_shms/mc_hms subroutine to transport the particle through SHMS/HMS apertures and to the focal plane.
* At the focal plane drift the particle through  1st Cerenkov or vacuum pipe (for SHMS only, depending on flag in input file),the drift chamber, scintillators, 2nd cerenkov and calorimeter to check if it hits them . 
* As it passes through the drift chamber randomly change the position according to the presumed DC resolution and fit the positions and calculate the focal plane positions and angles.
* From focal plane calculate the reconstructed target quantities ytar,yptar,xptar and delta using the optics matrix. Note that for now use the known x target position in the optics matrix calculations. To mimic what would be in the analysis, need to calculate xtar from the reconstructed quantities using the raster vertical position and then recalculate the reconstructed quantities with updated xtar.
* If particle makes it to the focal plane and hits all detectors the event variables are put in ntuple.



Sub Directories
---------------
* hms: HMS subroutines
* shms  : SHMS subroutines
* infiles : input files
* outfiles : the output files 
* worksim : rzdat files

Info on infiles
---------------
* Mostly self explanatory
* dp/p down and up should be at least -15.0 and 25.0 
* theta down and up is dy/dz , horizontal angle relative to central ray keep at least -55,55mr
* phi down and up is the dx/dz, vertical angle relative to central ray ,keep at least -50, 50mr
* Remember to keep the Dp/p,theta,phi and ztgt reconstruction cut larger than thrown. 
* The target length just need to set flag if aerogel will be in the detector stack
* Up to the experiment to decide if the 1st Cerenkov detector will be needed for the experiemnt
* If 1st Cerenkov not used then can replace with vacuum pipe. Option of helium bag is availble. this was for study.
* The multifoil optics targets can be implementented by entering the target length as -2 for the optics2 target (two foils) and -3 for optics1 target (three foils).
* the sieve can be used by setting the flag == 1. (See infiles/ examples)
 
Ntuple variables in SHMS hut ntuple ntuple id = 1411 
---------------------
* psxfp  Focal plane vertical position 
* psyfp  Focal plane horizontal position
* psxpfp Focal plane vertical angle=dx/dz
* psypfp Focal plane horizontal angle=dy/dz
* psztari  Initial random position along the target
* psytari  Initial y (horizontal) position at the z=0 plane perpendicular to central ray of SHMS
* psdeltai Initial random  delta = (p-pcent)/pcent
* psyptari Initial random  horizontal angle=dy/dz
* psxptari Initial random  vertical angle=dx/dz
* psztar  Reconstructed position along the target
* psytar   Reconstructed 
* psdelta   Reconstructed 
* psyptar  Reconstructed 
* psxptar Reconstructed 
* psxtari  Initial x (vertical) position at the z=0 plane perpendicular to central ray of SHMS
* fry   Initial random vertical raster position
* xsnum   sieve slit vertical hole number ( hole number at front sieve if use_front_sieve = true)
* ysnum   sieve slit horizontal hole number ( hole number at front sieve if use_front_sieve = true)
* xsieve   sieve slit vertical position (cm)  ( position at front sieve if use_front_sieve = true)
* ysieve   sieve slit horizontal position (cm) ( position at front sieve if use_front_sieve = true)
* stop_id information on where the track ended (0=successful reached detectors)
* psvxi initial x position in the beam following system
* psvyi initial y position in the beam following system

Ntuple variables in HMS hut ntuple ntuple id = 1 
---------------------
* same as SHMS except the variables start with "hs" instead of "ps". e.g. psdelta ---> hsdelta 
